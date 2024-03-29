#include <ft_ssl.h>

uint64_t asn_decode_varint(uint8_t *stream, size_t *iter, int *error)
{
	if(stream[*iter] != 2) {
		error[0] = 1;
		return 0;
	}

	*(iter) += 1;

	uint8_t len = *((uint8_t *)&stream[*iter]);

	*(iter) += 1;

	if (len > 8) {
		error[0] = 1;
		return 0;
	}

	uint64_t rv = 0;

	for (size_t i = 0; i < len; i++)
	{
		uint8_t new = *((uint8_t *)&stream[*iter]);
		rv |= ((uint64_t)new << (8 * i));
		*(iter) += 1;
	}


	return rv;
}

uint16_t asn_decode_sequence(uint8_t *stream, size_t *iter, int *error)
{
	if(stream[*iter] != 0x30) {
		error[0] = 1;
		return 0;
	}

	*(iter) += 1;

	if(stream[*iter] != 0x82) {
		error[0] = 1;
		return 0;
	}

	*(iter) += 1;

	uint16_t rv = *((uint16_t *)&stream[*iter]);

	*(iter) += 2;

	return rv;
}

#define DECODE_SAFE(cmd) cmd; if (*error != 0) {return rv;}


priv_rsa_t	asn_decode_priv_rsa(uint8_t *stream, int *error)
{
	size_t iter = 0;
	priv_rsa_t rv = {};

	DECODE_SAFE(uint16_t seq_len = asn_decode_sequence(stream, &iter, error))
	DECODE_SAFE(uint64_t version = asn_decode_varint(stream, &iter, error));
	
	(void)seq_len;

	if (version != 0) {
		error[0] = 1;
		return rv;
	}

	DECODE_SAFE(rv.modulus = asn_decode_varint(stream, &iter, error));
	DECODE_SAFE(rv.pub_exponent = asn_decode_varint(stream, &iter, error));
	DECODE_SAFE(rv.priv_exponent = asn_decode_varint(stream, &iter, error));
	DECODE_SAFE(rv.primes[0] = asn_decode_varint(stream, &iter, error));
	DECODE_SAFE(rv.primes[1] = asn_decode_varint(stream, &iter, error));
	DECODE_SAFE(rv.exponents[0] = asn_decode_varint(stream, &iter, error));
	DECODE_SAFE(rv.exponents[1] = asn_decode_varint(stream, &iter, error));
	DECODE_SAFE(rv.coefficient = asn_decode_varint(stream, &iter, error));

	return rv;
}

pub_rsa_t	asn_decode_pub_rsa(uint8_t *stream, int *error)
{
	size_t iter = 0;
	pub_rsa_t rv;

	stream += 22;

	DECODE_SAFE(rv.modulus = asn_decode_varint(stream, &iter, error));
	DECODE_SAFE(rv.pub_exponent = asn_decode_varint(stream, &iter, error));

	return rv;
}

size_t	asn_write_long(uint8_t *buf, long val)
{
	buf[0] = 0x2;
	buf[1] = 0x8;
	long *b = (long *)&buf[2];
	b[0] = val;

	return 10;
}

size_t	asn_write_int(uint8_t *buf, int val)
{
	buf[0] = 0x2;
	buf[1] = 0x4;
	int *b = (int *)&buf[2];
	b[0] = val;

	return 6;
}

uint8_t	*asn_encode_priv_rsa(priv_rsa_t rsa)
{
	uint8_t *buf = calloc(72, 1);
	if (!buf) return 0;

	uint8_t header[] = {0x30,0x82,0x0,0x40,0x2,0x1,0x0};

	memcpy(buf, header, 7);
	size_t buf_idx = 7;

	buf_idx += asn_write_long(&buf[buf_idx], rsa.modulus);
	buf_idx += asn_write_long(&buf[buf_idx], rsa.pub_exponent);
	buf_idx += asn_write_long(&buf[buf_idx], rsa.priv_exponent);
	buf_idx += asn_write_int(&buf[buf_idx], rsa.primes[0]);
	buf_idx += asn_write_int(&buf[buf_idx], rsa.primes[1]);
	buf_idx += asn_write_int(&buf[buf_idx], rsa.exponents[0]);
	buf_idx += asn_write_int(&buf[buf_idx], rsa.exponents[1]);
	buf_idx += asn_write_int(&buf[buf_idx], rsa.coefficient);

	return buf;
}

uint8_t	*asn_encode_pub_rsa(pub_rsa_t rsa)
{
	uint8_t *buf = malloc(44);
	if (!buf) return 0;

	uint8_t header[] = {0x30, 0x5c, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x01, 0x05, 0x00, 0x03, 0x4b, 0x00, 0x30, 0x48};

	memcpy(buf, header, sizeof(header));
	size_t buf_idx = sizeof(header);

	buf_idx += asn_write_long(&buf[buf_idx], rsa.modulus);
	buf_idx += asn_write_long(&buf[buf_idx], rsa.pub_exponent);

	return buf;
}
