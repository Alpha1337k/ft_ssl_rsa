#include <ft_ssl.h>
int is_divisible_by(uint64_t num, uint64_t cmp)
{
	return num % cmp == 0;
}

int is_prime(uint64_t num)
{
	if (is_divisible_by(num, 2) || is_divisible_by(num, 3)) return 0;

	uint64_t cmp = 5;
	while (cmp * cmp < num)
	{
		if (is_divisible_by(num, cmp) || is_divisible_by(num, cmp + 2)) return 0;
		
		cmp+=6;
	}
	return 1;
}

uint64_t gcd(uint64_t a, uint64_t b)
{
	uint64_t tmp;
	while (b != 0)
	{
		tmp = b;
		b = a % b;
		a = tmp;
	}
	return a;
}

uint64_t lcm(uint64_t a, uint64_t b)
{
	return a * (b / gcd(a,b));
}

// d ≡ e−1 (mod λ(n));
// 1 = (e * d) % n
// 1 = 17 * n % 780
// d * 3233 = 1 % 780
// 
