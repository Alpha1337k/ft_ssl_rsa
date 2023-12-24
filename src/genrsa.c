#include <ft_ssl.h>

uint64_t modInverse(uint64_t A, uint64_t M) 
{ 
    for (uint64_t X = 1; X < M; X++) 
        if (((A % M) * (X % M)) % M == 1) 
            return X; 
} 
  

int handle_genrsa(genrsa_options_t options)
{
	int rand_fd = open("/dev/urandom", O_RDONLY);
	if (rand_fd == -1) {
		printf("ft_ssl: Error: failed to open urandom\n");
		exit(1);
	}

	uint64_t fetched = 0;

	uint64_t primes[2] = {61, 53};
	int prime_idx = 0;
	while (prime_idx < 2)
	{
		if (read(rand_fd, &fetched, sizeof(uint64_t)) == -1) {
			printf("ft_ssl: error: Failed to get random bytes\n");
			exit(1);
		}
		if (fetched > 0xFFFFFFFF && is_prime(fetched)) {
			primes[prime_idx++] = fetched;
			printf("+++++\n");
		} else {
			printf(".");
		}
	}

	uint64_t n = primes[0] * primes[1];
	printf("%lu\t%lu\t%lu\n", n, primes[0], primes[1]);
	printf("gcd: %lu\n", gcd(primes[0], primes[1]));

	uint64_t l = lcm(primes[0] - 1, primes[1] - 1);

	printf("lcm: %lu\n", l);

	uint64_t e = 65537;
	uint64_t d = modInverse(e, l);

	printf("E %lu\tD %lu\n", e, d);
}
