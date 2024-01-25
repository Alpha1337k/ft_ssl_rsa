#include <ft_ssl.h>

uint64_t		mul_mod(uint64_t a, uint64_t b, uint64_t m)
{
	uint64_t	res;
	uint64_t	temp_b;

	res = 0;
	if (b >= m)
		b = (m > UINT64_MAX / 2u) ? b - m : b % m;
	while (a != 0)
	{
		if (a & 1)
		{
			if (b >= m - res)
				res -= m;
			res += b;
		}
		a >>= 1;
		temp_b = b;
		if (b >= m - b)
			temp_b -= m;
		b += temp_b;
	}
	return (res);
}

size_t			mod_pow(size_t value, size_t exponent, size_t mod)
{
	size_t		power;
	size_t		result;

	result = 1;
	power = value;
	while (exponent > 0)
	{
		if ((exponent & 1) == 1)
			result = mul_mod(result, power, mod);
		power = mul_mod(power, power, mod);
		exponent >>= 1;
	}
	return ((size_t)result);
}

uint8_t *decrypt(priv_rsa_t key, uint64_t *bytes, size_t len)
{
	uint64_t *rv = malloc(sizeof(uint64_t) * (len / 8));
	if (!rv) {
		printf("ft_ssl: Error: malloc fail\n");
		exit(1);
	}

	for (size_t i = 0; i < len / 8; i++)
	{
		rv[i] = mod_pow(bytes[i], key.priv_exponent, key.modulus);
	}
	
	return (uint8_t *)rv;
}

uint64_t *encrypt(priv_rsa_t key, uint64_t *bytes, size_t len)
{
	uint64_t *rv = malloc(sizeof(uint64_t) * (len / 8));
	if (!rv) {
		printf("ft_ssl: Error: malloc fail\n");
		exit(1);
	}

	for (size_t i = 0; i < len / 8; i++)
	{
		rv[i] = mod_pow(bytes[i], key.pub_exponent,  key.modulus);
	}
	
	return rv;
}

uint8_t *read_input(int fd, size_t *len)
{
	int status = 0;
	uint8_t buf[513];
	uint8_t *rv = 0;
	size_t rv_len = 0;

	do
	{
		status = read(fd, &buf, 512);

		if (status > 0) {
			size_t new_len = rv_len + status;
			if (new_len % 8 != 0) {
				new_len += (8 - new_len % 8);
			}
			uint8_t *new_rv = malloc(new_len);
		
			if (!new_rv) {
				printf("ft_ssl: Error: malloc fail\n");
				exit(1);
			}

			if (rv) {
				memcpy(new_rv, rv, rv_len);
				memcpy(new_rv + rv_len, buf, status);
				rv_len = new_len;
				free(rv);
				rv = new_rv;
			} else {
				memcpy(new_rv, buf, status);
				rv_len = new_len;
				rv = new_rv;
			}
		}
	} while (status > 0);

	*len = rv_len;
	return rv;
}

int handle_rsautl(rsautl_options_t options)
{
	priv_rsa_t pkey = parse_private_key(options.in_key, 0);

	size_t in_len = 0;
	uint8_t *in = read_input(options.in_fd, &in_len);

	uint8_t *out = 0;

	if (options.task == ENCRYPT)
	{
		out = (uint8_t *)encrypt(pkey, (uint64_t *)in, in_len);
	}
	else if (options.task == DECRYPT)
	{
		assert(in_len % 8 == 0);
		out = (uint8_t *)decrypt(pkey, (uint64_t *)in, in_len);
	}

	if (options.hexdump) {
		hexdump(options.out_fd, out, in_len);
	} else {
		write(options.out_fd, out, in_len);
	}

	return 0;
}
