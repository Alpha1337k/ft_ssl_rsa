#include <ft_ssl.h>

long memstrchr(uint8_t *bytes, size_t bytes_len, char *str)
{
	size_t str_len = strlen(str);

	for (size_t i = 0; i < bytes_len - str_len; i++)
	{
		if (memcmp(&bytes[i], str, str_len) == 0) {
			return i;
		}
	}
	return -1;
}

#define START_FOUND 0
#define PRIVATE_START_FOUND 1
#define PRIVATE_END_FOUND 2
#define PRIVATE_START "-----BEGIN RSA PRIVATE KEY-----\n"
#define PRIVATE_END "\n-----END RSA PRIVATE KEY-----"

uint8_t *read_pkey(int fd, size_t *len, long *start_idx, long *end_idx)
{
	int status = 0;
	uint8_t buf[512];
	uint8_t *rv = 0;
	size_t rv_len = 0;

	uint8_t state = 0;

	do
	{
		status = read(fd, &buf, 512);
		if (status > 0) {
			size_t new_len = rv_len += status;
			uint8_t *new_rv = malloc(new_len);

			if (rv) {
				memcpy(new_rv, rv, rv_len);
				memcpy(new_rv + rv_len, buf, status);
				rv_len = new_len;
				free(rv);
				rv = new_rv;
			} else {
				memcpy(new_rv, buf, status);
				rv = new_rv;
			}
		}
		*len = rv_len;

		if ((state & PRIVATE_START_FOUND) == 0) {
			start_idx[0] = memstrchr(rv, rv_len, PRIVATE_START);
			if (*start_idx != -1) {
				(*start_idx) += sizeof(PRIVATE_START) - 1;
				state |= PRIVATE_START_FOUND;
			}
		}
		if (state == 1) {
			end_idx[0] = memstrchr(rv, rv_len, PRIVATE_END);
			if (*end_idx != -1) {
				state |= PRIVATE_END_FOUND;
			}
		}

	} while (status > 0 && state != 3);

	if (state != (PRIVATE_START_FOUND | PRIVATE_END_FOUND)) {
		printf("ft_ssl: Error: start and end not found. (%d)\n", state);
		exit(1);
	}

	return rv;	
}


int handle_rsa(rsa_options_t options)
{
	size_t raw_len = 0;
	long start = 0, end = 0;
	uint8_t *pkey_raw = read_pkey(options.in_fd, &raw_len, &start, &end);

	for (size_t i = start; i < end; i++)
	{
		printf("%c", pkey_raw[i]);
	}
	printf("$\n");

	uint8_t *pkey_decoded = base64_decode(&pkey_raw[start], end - start);

	rsa_t pkey = asn_decode_rsa(pkey_decoded);

	printf("modulus:%lu\npublicExponent:%lu\nprivateExponent:%lu\nprime1:%u\nprime2:%u\nexponent1:%u\nexponent2:%u\ncoeffecient:%d\n",
		pkey.modulus,
		pkey.pub_exponent,
		pkey.priv_exponent,
		pkey.primes[0],
		pkey.primes[1],
		pkey.exponents[0],
		pkey.exponents[1],
		pkey.coefficient
	);

}