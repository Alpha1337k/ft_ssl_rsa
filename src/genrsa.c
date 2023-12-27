#include <ft_ssl.h>

uint64_t modInverse(uint64_t A, uint64_t M) 
{ 
    for (uint64_t X = 1; X < M; X++) 
        if (((A % M) * (X % M)) % M == 1) 
            return X; 
}

int gcdExtended(int a, int b, int* x, int* y)
{
    // Base Case
    if (a == 0) {
        *x = 0;
        *y = 1;
        return b;
    }
 
    int x1, y1; // To store results of recursive call
    int gcd = gcdExtended(b % a, a, &x1, &y1);
 
    // Update x and y using results of recursive
    // call
    *x = y1 - (b / a) * x1;
    *y = x1;
 
    return gcd;
}

int handle_genrsa(genrsa_options_t options)
{
	int rand_fd = open("/dev/urandom", O_RDONLY);
	if (rand_fd == -1) {
		printf("ft_ssl: Error: failed to open urandom\n");
		exit(1);
	}

	uint64_t fetched = 0;

	uint32_t primes[2] = {61, 53};
	int prime_idx = 0;
	while (prime_idx < 2)
	{
		if (read(rand_fd, &fetched, sizeof(uint32_t)) == -1) {
			printf("ft_ssl: error: Failed to get random bytes\n");
			exit(1);
		}
		if (is_prime(fetched)) {
			primes[prime_idx++] = fetched;
			printf("+++++\n");
		} else {
			printf(".");
		}
	}

	uint64_t modulus = primes[0] * primes[1];
	printf("M:%lu\tP1:%u\tP2:%u\n", modulus, primes[0], primes[1]);
	printf("gcd: %lu\n", gcd(primes[0], primes[1]));

	uint64_t l = lcm(primes[0] - 1, primes[1] - 1);

	printf("lcm: %lu\n", l);

	uint64_t e = 17;
	int x, y;

	uint64_t d = gcdExtended(e, l, &x, &y);

	printf("E %lu\tD %lu\n", e, d);
	printf("X %d\tY %d\n", x, y);
}
