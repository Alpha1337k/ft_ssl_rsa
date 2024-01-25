#include <ft_ssl.h>

long mut_inv(long e, long mod)
{
	long t = 0, nT = 1, r = mod, nR = e, q = 0, lT = 0, lR = 0;

	while (nR != 0)
	{
		q = r / nR;
		lT = t;
		lR = r;
		t = nT;
		r = nR;

		nT = lT - q * nT;
		nR = lR - q * nR;
	}

	if (r != 1) {
		printf("ft_ssl: error: inv_mul: not coprime.\n");
	}
	if (t < 0) {
		t += mod;
	}

	return t;
}

uint32_t get_prime(int rand_fd) {
	uint32_t fetched = 0;
	while (1)
	{
		if (read(rand_fd, &fetched, sizeof(uint32_t)) == -1) {
			printf("ft_ssl: error: Failed to get random bytes\n");
			exit(1);
		}
		if (is_prime(fetched)) {
			return fetched;
		}
	}
	return 0;
}

int handle_genrsa(genrsa_options_t options)
{
	int rand_fd = open("/dev/urandom", O_RDONLY);
	if (rand_fd == -1) {
		printf("ft_ssl: Error: failed to open urandom\n");
		exit(1);
	}

	fprintf(stderr, "Generating RSA private key, 64 bit long modulus (2 primes)\n");

	priv_rsa_t rsa;

	while (1)
	{
		rsa.primes[0] = get_prime(rand_fd);
		rsa.primes[1] = get_prime(rand_fd);

		rsa.modulus = (uint64_t)rsa.primes[0] * (uint64_t)rsa.primes[1];

		printf(".");

		if (rsa.modulus >> 63) {
			printf("+++++\n");
			break;
		}
	}
	uint64_t ltf = lcm(rsa.primes[0] - 1, rsa.primes[1] - 1);

	rsa.pub_exponent = 0x10001;

	fprintf(stderr, "e is 65537 (0x10001)\n");

	rsa.priv_exponent = mut_inv(rsa.pub_exponent, ltf);

	rsa.exponents[0] = rsa.priv_exponent % (rsa.primes[0] - 1);
	rsa.exponents[1] = rsa.priv_exponent % (rsa.primes[1] - 1);

	rsa.coefficient = mut_inv(rsa.primes[1], rsa.primes[0]);

	print_rsa_private(options.out_fd, rsa, 0);

	return 0;
}
