#include <ft_ssl.h>

void	print_rsa_private(int fd, priv_rsa_t rsa, char *passout)
{
	uint8_t *asn_encoded = asn_encode_priv_rsa(rsa);
	if (!asn_encoded) {
		printf("ft_ssl: Error: asn encode fail\n");
		exit(1);
	}

	if (passout) {
		char *pass = get_pass(passout, "Enter output password");
		
		if (!pass || strlen(pass) < 4) {
			printf("ft_ssl: Error: pass to small\n");
			free(asn_encoded);
			exit(1);
		} 

		asn_encoded = des((uint64_t *)asn_encoded, pass, 9, 0);

		free(pass);
	}

	uint8_t	*base64_encoded = base64_encode(asn_encoded, 72);
	if (!base64_encoded) {
		printf("ft_ssl: Error: base64 encode fail\n");
		free(asn_encoded);
		exit(1);
	}

	dprintf(fd, PRIVATE_START"%s%s"PRIVATE_END"\n", passout ? ENCRYPT_INFO : "", base64_encoded);

	free(asn_encoded);
	free(base64_encoded);

}

void	print_rsa_public(int fd, pub_rsa_t rsa)
{
	uint8_t *asn_encoded = asn_encode_pub_rsa(rsa);
	if (!asn_encoded) {
		printf("ft_ssl: Error: asn encode fail\n");
		exit(1);
	}

	uint8_t	*base64_encoded = base64_encode(asn_encoded, 44);
	if (!base64_encoded) {
		printf("ft_ssl: Error: base64 encode fail\n");
		free(asn_encoded);
		exit(1);
	}


	dprintf(fd, PUBLIC_START"%s"PUBLIC_END"\n", base64_encoded);

	free(asn_encoded);
	free(base64_encoded);

}
