#include <ft_ssl.h>

int handle_private_key(rsa_options_t options)
{
	priv_rsa_t pkey = parse_private_key(options.in_fd, options.passin);

	if (options.modulus) {
		printf("Modulus=%lX\n", pkey.modulus);
	}

	if (options.check) {
		if ((uint64_t)pkey.primes[0] * (uint64_t)pkey.primes[1] == pkey.modulus)
			printf("RSA key ok\n");
		else
			printf("RSA Key error: n does not equal p q\n");
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

	if (options.no_out == 0) {
		fprintf(stderr, "writing RSA key\n");
		if (options.pub_out) {
			pub_rsa_t pub;
			pub.modulus = pkey.modulus;
			pub.pub_exponent = pkey.pub_exponent;

			print_rsa_public(options.out_fd, pub);
		} else {
			char *pass = 0;
			if (options.des && options.passout)
				pass = options.passout;
			else if (options.des)
				pass = "stdin";	

			print_rsa_private(options.out_fd, pkey, pass);
		}
	}
}

int handle_public_key(rsa_options_t options)
{
	pub_rsa_t pkey = parse_public_key(options.in_fd);

	if (options.modulus) {
		printf("Modulus=%lX\n", pkey.modulus);
	}

	if (options.text) {
		printf("RSA Public-Key: (64 bit)\n");
		printf("modulus:%lu\npublicExponent:%lu\n",
			pkey.modulus,
			pkey.pub_exponent
		);
	}

	if (options.no_out == 0) {
		fprintf(stderr, "writing RSA key\n");
		print_rsa_public(options.out_fd, pkey);
	}

}


int handle_rsa(rsa_options_t options)
{
	if (options.pub_in) {
		return handle_public_key(options);
	} else {
		return handle_private_key(options);
	}
}