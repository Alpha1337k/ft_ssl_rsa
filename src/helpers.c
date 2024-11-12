#include <ft_ssl.h>

#define START_FOUND 1
#define END_FOUND 2


long memstrchr(uint8_t *bytes, long bytes_len, char *str)
{
	size_t str_len = strlen(str);

	if (!bytes || (long)(bytes_len - str_len + 1) < 0) return -1;

	for (size_t i = 0; i < bytes_len - str_len + 1; i++)
	{
		if (memcmp(&bytes[i], str, str_len) == 0) {
			return i;
		}
	}
	return -1;
}

char *get_pass(char *pass_cmd, char *prompt)
{
	char buf[1025];

	memset(buf, 0, 1025);

	if (strncmp(pass_cmd, "stdin", 4) == 0) {
		printf("%s: ", prompt);
		fflush(stdout);
		if (scanf("%1024s", buf) == 0) {
			printf("ft_ssl: error: password not provided");
		}

		return strdup(buf);
	} else if (strncmp(pass_cmd, "pass:", 5) == 0) {
		return strdup(pass_cmd + 5);
	} else if (strncmp(pass_cmd, "env:", 4) == 0) {
		return strdup(getenv(pass_cmd + 4));
	} else if (strncmp(pass_cmd, "file:", 5) == 0) {
		int fd = open(pass_cmd + 5, O_RDONLY);
		if (fd == -1) {
			printf("ft_ssl: Error: could not open password\n");
			exit(1);
		}

		if (read(fd, buf, 1024) == -1) {
			perror("ft_ssl: Error: ");
			exit(1);
		}
		close(fd);

		return strdup(buf);
	}
	printf("ft_ssl: Error: unknown pass option\n");
	exit(1);
	return 0;
}

uint8_t *read_key(
	int fd, 
	long *start_idx,
	long *end_idx,
	char *start_str,
	char *end_str,
	int	*is_encrypted
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
		free(rv);
		exit(1);
	}

	long enc_idx = memstrchr(rv, rv_len, ENCRYPT_INFO);
	if (enc_idx != -1) {
		if (is_encrypted) {
			is_encrypted[0] = 1;
		}
		start_idx[0] += sizeof(ENCRYPT_INFO) - 1;
	}

	return rv;	
}

priv_rsa_t	parse_private_key(int in_fd, char *passin)
{
	long start = 0, end = 0;
	int encrypted = 0;
	uint8_t *pkey_raw = read_key(in_fd, 
		&start, 
		&end,
		PRIVATE_START,
		PRIVATE_END,
		&encrypted
	);

	if (!pkey_raw) {
		printf("ft_ssl: Error: key read failed\n");
		exit(1);
	}


	uint8_t *pkey_decoded = base64_decode(&pkey_raw[start], end - start);
	free(pkey_raw);
	if (!pkey_decoded) {
		printf("ft_ssl: Error: base64 decode failed\n");
		exit(1);
	}

	char *pass = 0;

	if (encrypted != 0)
	{
		if (!passin)
			passin = "stdin";
		pass = get_pass(passin, "Enter passin");
		pkey_decoded = des((uint64_t *)pkey_decoded, pass, 9, 1);
		free(pass);
	}

	int parse_error = 0;
	priv_rsa_t pkey = asn_decode_priv_rsa(pkey_decoded, &parse_error);
	
	free(pkey_decoded);

	if (parse_error)
	{
		printf("ft_ssl: Error: could not parse asn\n");
		exit(1);
	}

	return pkey;
}

pub_rsa_t	parse_public_key(int in_fd)
{
	long start = 0, end = 0;

	uint8_t *key_raw = read_key(
		in_fd, 
		&start, 
		&end,
		PUBLIC_START,
		PUBLIC_END,
		0
	);
	
	if (!key_raw) {
		printf("ft_ssl: Error: key read failed\n");
		exit(1);
	}

	uint8_t *key_decoded = base64_decode(&key_raw[start], end - start);

	free(key_raw);
	
	if (!key_decoded) {
		printf("ft_ssl: Error: base64 decode failed\n");
		exit(1);
	}

	int parse_error = 0;
	pub_rsa_t pkey = asn_decode_pub_rsa(key_decoded, &parse_error);

	free(key_decoded);

	if (parse_error) {
		printf("ft_ssl: Error: could not parse asn\n");
		exit(1);
	}

	return pkey;
}