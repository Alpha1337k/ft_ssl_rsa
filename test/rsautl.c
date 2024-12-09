#include <ft_ssl.h>

void print_hex(uint8_t *bytes, size_t len)
{
	size_t i = 0;

	while (i < len)
	{
		printf("%x ", bytes[i]);
		i++;
	}
	printf("\n");
}

int test(priv_rsa_t key, char *input, size_t len)
{
	size_t chunked_len = len;

	data_chunk_t *chunked = chunk_input(input, &chunked_len);

	data_chunk_t *enc = crypt(key.pub_exponent, key.modulus, chunked, chunked_len);

	data_chunk_t *dec = crypt(key.priv_exponent, key.modulus, enc, chunked_len);

	uint8_t *out = dechunk_input(dec, chunked_len);

	int result = memcmp(input, out, len);

	// printf("%s\n", input);
	// for (size_t i = 0; i < chunked_len; i++)
	// {
	// 	printf("%ld::%u L:%d E:%lx D:%lx C:%lx\n", i, ((uint32_t *)input)[0], enc[0].size, ((uint64_t *)enc[0].data)[0], ((uint64_t *)dec[0].data)[0], ((uint64_t *)chunked[0].data)[0]);
	// 	printf("%ld::%u L:%d E:%ld D:%ld C:%ld\n", i, ((uint32_t *)input)[0], enc[0].size, ((uint64_t *)enc[0].data)[0], ((uint64_t *)dec[0].data)[0], ((uint64_t *)chunked[0].data)[0]);
	// }
	// printf("--\n");
	// print_hex((uint8_t *)chunked, len);
	// printf("--\n");
	// print_hex((uint8_t *)enc, len);
	// printf("--\n");
	// print_hex((uint8_t *)dec, len);
	
	// printf("--\n");
	// print_hex((uint8_t *)input, len);
	// printf("--\n");
	// print_hex((uint8_t *)out, len);

	if (result != 0) {
		printf("Error: mismatch: %d\nIn: %s, Out: %s\n", result, input, (char *)dec);
	} else {
		printf("[OK]\n");
	}
}

int test_chunk(uint8_t *data, size_t len) {
	data_chunk_t *chunked = chunk_input(data, &len);

	size_t chunked_idx = 0;

	for (size_t i = 0; i < len; i++)
	{
		uint8_t *c_data = chunked[chunked_idx].data;

		// printf("%ld %ld %d, %x | %x %x %x %x\n", i, chunked_idx, chunked[chunked_idx].size, data[i], c_data[0], c_data[1], c_data[2], c_data[3]);

		if ((i + 1) % 3 == 0 && i != 0) {
			chunked_idx++;
		}
	}

}

int main()
{
	priv_rsa_t rsa_priv = {
		modulus: 9372599666364928687ull,
		pub_exponent: 65537,
		priv_exponent: 2883557791009266953ull,
		primes: {3177120797, 2039232503},
		exponents: {1837326673, 2039232503},
		coefficient: -1486037343,
	};

	test_chunk("Hello", 5);
	test_chunk("Hel", 3);

	test(rsa_priv, "Hell", 4);
	test(rsa_priv, "\0\0\0\0\0\0\0", 8);
	test(rsa_priv, "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8);
	test(rsa_priv, "HelloWorld\0\0\0\0\0", 16);
	test(rsa_priv, "\xFF""elloWorld\0\0\0\0\0", 16);
	test(rsa_priv, "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\0\0\0\0\0", 16);
}
