#include <ft_ssl.h>

void	print_rsa_private(int fd, priv_rsa_t rsa)
{
	uint8_t *asn_encoded = asn_encode_priv_rsa(rsa);

	uint8_t	*base64_encoded = base64_encode(asn_encoded, 67);

	dprintf(fd, PRIVATE_START"%s"PRIVATE_END"\n", base64_encoded);

	free(asn_encoded);
	free(base64_encoded);

}

void	print_rsa_public(int fd, pub_rsa_t rsa)
{
	uint8_t *asn_encoded = asn_encode_pub_rsa(rsa);

	uint8_t	*base64_encoded = base64_encode(asn_encoded, 44);

	dprintf(fd, PUBLIC_START"%s"PUBLIC_END"\n", base64_encoded);

	free(asn_encoded);
	free(base64_encoded);

}
