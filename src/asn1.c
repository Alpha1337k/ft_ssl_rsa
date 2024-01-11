#include <ft_ssl.h>

uint64_t asn_decode_varint(uint8_t *stream, size_t *iter)
{
	assert(stream[*iter] == 2);


	*(iter) += 1;

	uint8_t len = *((uint8_t *)&stream[*iter]);

	*(iter) += 1;



	assert(len < 8);

	uint64_t rv = 0;

	for (size_t i = 0; i < len; i++)
	{
		rv <<= 8;
		uint8_t new = *((uint8_t *)&stream[*iter]);
		rv |= new;
		*(iter) += 1;
	}


	return rv;
}

uint16_t asn_decode_sequence(uint8_t *stream, size_t *iter)
{
	assert(stream[*iter] == 0x30);

	*(iter) += 1;

	assert(stream[*iter] == 0x82);

	*(iter) += 1;

	uint16_t rv = *((uint16_t *)&stream[*iter]);

	*(iter) += 2;

	return rv;
}


rsa_t	asn_decode_rsa(uint8_t *stream)
{
	size_t iter = 0;
	rsa_t rv;

	uint16_t seq_len = asn_decode_sequence(stream, &iter);
	uint64_t version = asn_decode_varint(stream, &iter);
	assert(version == 0);

	rv.modulus = asn_decode_varint(stream, &iter);
	rv.pub_exponent = asn_decode_varint(stream, &iter);
	rv.priv_exponent = asn_decode_varint(stream, &iter);

	rv.primes[0] = asn_decode_varint(stream, &iter);
	rv.primes[1] = asn_decode_varint(stream, &iter);
	rv.exponents[0] = asn_decode_varint(stream, &iter);
	rv.exponents[1] = asn_decode_varint(stream, &iter);
	rv.coefficient = asn_decode_varint(stream, &iter);

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

uint8_t	*asn_encode_rsa(rsa_t rsa)
{
	uint8_t *buf = malloc(67);
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
