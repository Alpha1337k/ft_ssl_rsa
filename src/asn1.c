#include <ft_ssl.h>


uint32_t asn_decode_u32(uint8_t *stream, size_t *iter)
{
	assert(stream[*iter] == 2);


	*(iter) += 1;

	assert(stream[*iter] == 4);

	*(iter) += 1;

	uint32_t rv = *((uint32_t *)&stream[*iter]);

	*(iter) += 4;

	return rv;
}

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

uint64_t asn_decode_u64(uint8_t *stream, size_t *iter)
{
	assert(stream[*iter] == 2);


	*(iter) += 1;

	assert(stream[*iter] == 8);

	*(iter) += 1;

	uint64_t rv = *((uint64_t *)&stream[*iter]);

	*(iter) += 8;

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

	rv.modulus = asn_decode_u64(stream, &iter);
	rv.pub_exponent = asn_decode_varint(stream, &iter);
	rv.priv_exponent = asn_decode_u64(stream, &iter);

	rv.primes[0] = asn_decode_u32(stream, &iter);
	rv.primes[1] = asn_decode_u32(stream, &iter);
	rv.exponents[0] = asn_decode_u32(stream, &iter);
	rv.exponents[1] = asn_decode_u32(stream, &iter);
	rv.coefficient = asn_decode_u32(stream, &iter);

	return rv;
}
