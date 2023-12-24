#ifndef FT_SSL
#define ft_ssl

#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>

typedef uint64_t uint64_t;

typedef struct genrsa_options_s
{
	int out_fd;
} genrsa_options_t;

typedef struct rsa_options_s
{
	int out_fd;
} rsa_options_t;

typedef struct rsautl_options_s
{
	int out_fd;
} rsautl_options_t;


rsa_options_t parse_rsa(int argc, char **argv);
genrsa_options_t parse_genrsa(int argc, char **argv);
rsautl_options_t parse_rsautl(int argc, char **argv);

int handle_genrsa(genrsa_options_t options);
int handle_rsa(rsa_options_t options);
int handle_rsautl(rsautl_options_t options);

int is_prime(uint64_t num);

uint64_t gcd(uint64_t a, uint64_t b);
uint64_t lcm(uint64_t a, uint64_t b);

uint8_t *base64_encode(uint8_t *bytes, size_t len);

#endif