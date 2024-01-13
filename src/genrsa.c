#include <ft_ssl.h>

uint64_t modInverse(uint64_t A, uint64_t M) 
{ 
    for (uint64_t X = 1; X < M; X++) 
        if (((A % M) * (X % M)) % M == 1) 
            return X; 
}

long gcdExtended(long a, long b, long* x, long* y)
{
    // Base Case
    if (a == 0) {
        *x = 0;
        *y = 1;
        return b;
    }
 
    long x1, y1; // To store results of recursive call
    long gcd = gcdExtended(b % a, a, &x1, &y1);
 
    // Update x and y using results of recursive
    // call
    *x = y1 - (b / a) * x1;
    *y = x1;
 
    return gcd;
}

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
			printf("+++++\n");
		} else {
			printf(".");
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

	rsa_t rsa;

	while (1)
	{
		rsa.primes[0] = get_prime(rand_fd);
		rsa.primes[1] = get_prime(rand_fd);

		rsa.modulus = (uint64_t)rsa.primes[0] * (uint64_t)rsa.primes[1];

		if (rsa.modulus >> 63) {
			break;
		}
	}
	printf("\nM:%lu\tP1:%u\tP2:%u\n", rsa.modulus, rsa.primes[0], rsa.primes[1]);
	// printf("gcd: %lu\n", gcd(primes[0], primes[1]));

	uint64_t ltf = lcm(rsa.primes[0] - 1, rsa.primes[1] - 1);

	printf("mod: %lu, lcm: %lu\n", rsa.modulus, ltf);

	rsa.pub_exponent = 0x10001;

	rsa.priv_exponent = mut_inv(rsa.pub_exponent, ltf);

	rsa.exponents[0] = rsa.priv_exponent % (rsa.primes[0] - 1);
	rsa.exponents[1] = rsa.priv_exponent % (rsa.primes[1] - 1);

	rsa.coefficient = mut_inv(rsa.primes[1], rsa.primes[0]);

	printf("E: %lu D: %ld\n", rsa.pub_exponent, rsa.priv_exponent);
	printf("EXP: '%u' '%u'\n", rsa.exponents[0], rsa.exponents[1]);
	printf("COE: %d\n", rsa.coefficient);

	print_rsa_private(options.out_fd, rsa);

	return 0;
}
