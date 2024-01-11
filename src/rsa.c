#include <ft_ssl.h>

uint8_t *read_pkey(int fd, size_t *len)
{
	int status = 0;
	uint8_t buf[512];
	uint8_t *rv = 0;
	size_t rv_len = 0;

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
	} while (status > 0);

	*len = rv_len;

	return rv;	
}


int handle_rsa(rsa_options_t options)
{
	size_t raw_len = 0;
	uint8_t *pkey_raw = read_pkey(options.in_fd, &raw_len);

	for (size_t i = 0; i < raw_len; i++)
	{
		printf("%c", pkey_raw[i]);
	}
	printf("\n\n");

	uint8_t *pkey_decoded = base64_decode(pkey_raw, raw_len);

	rsa_t pkey = asn_decode_rsa(pkey_decoded);

	printf("modulus:%lx\npublicExponent:%lx\nprivateExponent:\n%lx\nprime1:\n%x\nprime2:\n%x\nexponent1:\n%x\nexponent2:\n%x\ncoeffecient:\n%x\n",
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