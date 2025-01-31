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

int test_binary(char *input, size_t len)
{
	char *encoded = base64_encode(input, len);

	char *decoded = base64_decode(encoded, strlen(input));

	int result = memcmp(input, decoded, len);

	if (result != 0) {
		printf("Error: mismatch: %d\nIn: %s, Out: %s\n", result, input, decoded);
		print_hex(input, len);
		printf("--\n");
		print_hex(decoded, len);
	} else {
		printf("[OK]\n");
	}	
}

int test(char *input, char *encoded_real)
{
	char *encoded = base64_encode(input, strlen(input));

	if (encoded_real && strcmp(encoded, encoded_real)) {
		printf("diff real: %d\n", strcmp(encoded, encoded_real));
	}

	char *decoded = base64_decode(encoded, strlen(input));

	int result = strcmp(input, decoded);

	if (result != 0) {
		printf("Error: mismatch: %d\nIn: %s, Out: %s\n", result, input, decoded);
	} else {
		printf("[OK]\n");
	}
}

int main()
{
	test("AAaaAAaaAAaaAAaaAAaaAA", "QUFhYUFBYWFBQWFhQUFhYUFBYWFBQQ==");
	test("", 0);

	test("\xFF\xFF\xFF\xFF\x01\x01\x02", 0);

	priv_rsa_t rsa_priv = {
		modulus: 10,
		pub_exponent: (size_t)-1,
		priv_exponent: (size_t)-1,
		primes: {(uint32_t)-2, (uint32_t)-1},
		exponents: {(uint32_t)-2, (uint32_t)-1},
		coefficient: (int32_t)-1,
	};

	uint8_t *asn_encoded = asn_encode_priv_rsa(rsa_priv);

	test_binary(asn_encoded, 72);
}