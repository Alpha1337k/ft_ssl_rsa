#include <ft_ssl.h>

void	print_rsa_private(int fd, priv_rsa_t rsa, char *passout)
{
	uint8_t *asn_encoded = asn_encode_priv_rsa(rsa);

	if (passout) {
		char *pass = get_pass(passout, "Enter output password");
		
		if (!pass) {
		} 

		asn_encoded = des((uint64_t *)asn_encoded, pass, 9, 0);
	}

	uint64_t *key_long = (uint64_t *)asn_encoded;

	for (size_t i = 0; i < 9; i++)
	{
		printf("0x%lx ", key_long[i]);
	}
	printf("\n");

	uint8_t	*base64_encoded = base64_encode(asn_encoded, 72);

	dprintf(fd, PRIVATE_START"%s%s"PRIVATE_END"\n", passout ? ENCRYPT_INFO : "", base64_encoded);

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
