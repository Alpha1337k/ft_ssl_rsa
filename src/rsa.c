#include <ft_ssl.h>

long memstrchr(uint8_t *bytes, long bytes_len, char *str)
{
	size_t str_len = strlen(str);

	if (!bytes || (long)(bytes_len - str_len + 1) < 0) return -1;

	for (int i = 0; i < bytes_len - str_len + 1; i++)
	{
		if (memcmp(&bytes[i], str, str_len) == 0) {
			return i;
		}
	}
	return -1;
}

char *get_pass(rsa_options_t options) {
	char *rv;
	char buf[1025];

	memset(buf, 0, 1025);

	if (options.passin == 0) {
		scanf("Enter password: %1024s", buf);

		return strdup(buf);
	}

	if (strncmp(options.passin, "pass:", 5) == 0) {
		return strdup(options.passin + 5);
	} else if (strncmp(options.passin, "env:", 4) == 0) {
		return strdup(getenv(options.passin + 4));
	} else if (strncmp(options.passin, "file:", 5) == 0) {
		
		int fd = open(options.passin + 5, O_RDONLY);
		if (fd == -1) {
			printf("ft_ssl: Error: could not open password\n");
			exit(1);
		}

		read(fd, buf, 1024);
		close(fd);

		return strdup(buf);
	}
	printf("ft_ssl: Error: unknown pass option\n");
	return 0;
}

#define START_FOUND 1
#define END_FOUND 2


uint8_t *read_key(
	int fd, 
	long *start_idx,
	long *end_idx,
	char *start_str,
	char *end_str
	)
{
	int status = 0;
	uint8_t buf[513];
	uint8_t *rv = 0;
	size_t rv_len = 0;

	uint8_t state = 0;

	do
	{
		status = read(fd, &buf, 512);

		if (status >= 0) {
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

		if ((state & START_FOUND) == 0) {
			start_idx[0] = memstrchr(rv, rv_len, start_str);
			if (*start_idx != -1) {
				(*start_idx) += strlen(start_str);
				state |= START_FOUND;
			}
		}
		if (state == 1) {
			end_idx[0] = memstrchr(rv, rv_len, end_str);
			if (*end_idx != -1) {
				state |= END_FOUND;
			}
		}

	} while (status > 0 && state != 3);

	if (state != (START_FOUND | END_FOUND)) {
		printf("ft_ssl: Error: invalid key. (%d, %d)\n", state, status);
		exit(1);
	}

	return rv;	
}

int handle_private_key(rsa_options_t options)
{
	long start = 0, end = 0;

	uint8_t *key_raw = read_key(
		options.in_fd, 
		&start, 
		&end,
		PRIVATE_START,
		PRIVATE_END
		);
	uint8_t *key_decoded = base64_decode(&key_raw[start], end - start);

	priv_rsa_t pkey = asn_decode_priv_rsa(key_decoded);


	if (options.no_out == 0) {
		fprintf(stderr, "writing RSA key\n");
		if (options.pub_out) {
			pub_rsa_t pub;
			pub.modulus = pkey.modulus;
			pub.pub_exponent = pkey.pub_exponent;

			print_rsa_public(options.out_fd, pub);
		} else {
			print_rsa_private(options.out_fd, pkey);
		}
	}

	if (options.modulus) {
		printf("Modulus=%lX\n", pkey.modulus);
	}

	if (options.check) {
		printf("RSA key ok\n");
	}

	if (options.text) {
		printf("RSA Private-Key: (64 bit, 2 primes) %d\n", options.text);
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

	free(key_raw);
	free(key_decoded);
}

int handle_public_key(rsa_options_t options)
{
	size_t raw_len = 0;
	long start = 0, end = 0;

	uint8_t *key_raw = read_key(
		options.in_fd, 
		&start, 
		&end,
		PUBLIC_START,
		PUBLIC_END
		);

	uint8_t *key_decoded = base64_decode(&key_raw[start], end - start);

	pub_rsa_t pkey = asn_decode_pub_rsa(key_decoded);

	if (options.modulus) {
		printf("Modulus=%lX\n", pkey.modulus);
	}

	if (options.check) {
		printf("RSA key ok\n");
	}

	if (options.text) {
		printf("RSA Private-Key: (64 bit)\n");
		printf("modulus:%lu\npublicExponent:%lu\n",
			pkey.modulus,
			pkey.pub_exponent
		);
	}

	if (options.no_out == 0) {
		fprintf(stderr, "writing RSA key\n");
		print_rsa_public(options.out_fd, pkey);
	}

	free(key_raw);
	free(key_decoded);
}


int handle_rsa(rsa_options_t options)
{
	if (options.pub_in) {
		return handle_public_key(options);
	} else {
		return handle_private_key(options);
	}
}