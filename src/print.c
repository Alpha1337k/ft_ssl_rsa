#include <ft_ssl.h>

void	print_rsa_private(int fd, rsa_t rsa)
{
	uint8_t *asn_encoded = asn_encode_rsa(rsa);

	uint8_t	*base64_encoded = base64_encode(asn_encoded, 67);

	write(fd, "-----BEGIN RSA PRIVATE KEY-----\n", 32);
	write(fd, base64_encoded, strlen(base64_encoded));
	write(fd, "\n-----END RSA PRIVATE KEY-----\n", 31);

	free(asn_encoded);
	free(base64_encoded);

}
