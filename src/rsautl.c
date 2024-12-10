#include <ft_ssl.h>

uint64_t		mul_mod(uint64_t a, uint64_t b, uint64_t m)
{
	uint64_t	res;
	uint64_t	temp_b;

	res = 0;
	if (b >= m)
		b = (m > UINT64_MAX / 2u) ? b - m : b % m;
	while (a != 0)
	{
		if (a & 1)
		{
			if (b >= m - res)
				res -= m;
			res += b;
		}
		a >>= 1;
		temp_b = b;
		if (b >= m - b)
			temp_b -= m;
		b += temp_b;
	}
	return (res);
}

size_t			mod_pow(size_t value, size_t exponent, size_t mod)
{
	size_t		power;
	size_t		result;

	result = 1;
	power = value;
	while (exponent > 0)
	{
		if ((exponent & 1) == 1)
			result = mul_mod(result, power, mod);
		power = mul_mod(power, power, mod);
		exponent >>= 1;
	}
	return ((size_t)result);
}

data_chunk_t *crypt(uint64_t exp, uint64_t mod,  data_chunk_t *chunks, size_t len)
{
	data_chunk_t *out = malloc(len * sizeof(data_chunk_t));

	for (size_t i = 0; i < len; i++)
	{
		size_t item = ((uint64_t *)chunks[i].data)[0];

		size_t data = mod_pow(item, exp, mod);
	
		out[i].size = chunks[i].size;
		memcpy(out[i].data, &data, 8);

	}
	
	return out;
}

uint8_t *read_input(int fd, size_t *len)
{
	int status = 0;
	uint8_t buf[512];
	uint8_t *rv = 0;
	size_t rv_len = 0;

	do
	{
		status = read(fd, &buf, 512);

		if (status > 0) {
			size_t new_len = rv_len + status;

			uint8_t *new_rv = malloc(new_len);
		
			if (!new_rv) {
				printf("ft_ssl: Error: malloc fail\n");
				exit(1);
			}

			if (rv) {
				memcpy(new_rv, rv, rv_len);
				memcpy(new_rv + rv_len, buf, status);
				rv_len = new_len;
				free(rv);
				rv = new_rv;
			} else {
				memcpy(new_rv, buf, status);
				rv_len = new_len;
				rv = new_rv;
			}
		}
	} while (status > 0);

	*len = rv_len;
	return rv;
}

uint8_t *dechunk_input(data_chunk_t *in, size_t len, size_t *write_len) {
	size_t byte_len = 0;

	for (size_t i = 0; i < len; i++)
	{
		byte_len += in[i].size;
	}
	
	uint8_t *out = malloc(byte_len);

	size_t out_i = 0;

	*write_len = byte_len;

	for (size_t i = 0; i < len; i++)
	{
		memcpy(&out[out_i], in[i].data, in[i].size);

		// printf("%d %s$\n", in[i].size, &out[out_i]);

		out_i += in[i].size;
	}

	return out;
}

data_chunk_t *chunk_input(uint8_t *in, size_t *len) {
	size_t i = 0;

	size_t resized_len = ceilf(*len / 7.0);

	data_chunk_t *out = calloc(resized_len, sizeof(data_chunk_t));

	if (out == 0) {
		perror("ft_ssl: Error: malloc fail\n");
		exit(1);
	}

	size_t out_i = 0;

	while (i < *len)
	{
		size_t chunk_size = (*len) - i;

		if (chunk_size > 7) {
			chunk_size = 7;
		}

		out[out_i].size = chunk_size;

		memcpy(out[out_i].data, &in[i], chunk_size);

		out_i++;
		i += 7;
	}

	len[0] = resized_len;

	return out;

}

int handle_rsautl(rsautl_options_t options)
{
	priv_rsa_t pkey = parse_private_key(options.in_key, 0);

	size_t in_len = 0;
	uint8_t *in = read_input(options.in_fd, &in_len);

	uint8_t *out = 0;
	size_t write_len = in_len;

	if (options.task == ENCRYPT)
	{
		data_chunk_t *chunked = chunk_input(in, &in_len);

		write_len = in_len * sizeof(data_chunk_t);

		out = (uint8_t *)crypt(pkey.pub_exponent, pkey.modulus, (data_chunk_t *)chunked, in_len);
	}
	else if (options.task == DECRYPT)
	{
		assert(in_len % 9 == 0);

		data_chunk_t *crypt_out = crypt(pkey.priv_exponent, pkey.modulus, (data_chunk_t *)in, in_len / 9);

		out = dechunk_input(crypt_out, in_len / 9, &write_len);
	}

	printf("WRITING %ld\n", write_len);

	if (options.hexdump) {
		hexdump(options.out_fd, out, write_len);
	} else {
		write(options.out_fd, out, write_len);
	}

	return 0;
}
