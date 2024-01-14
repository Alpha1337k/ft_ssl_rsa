#ifndef FT_SSL
#define ft_ssl

#define _POSIX_C_SOURCE  200809L
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <assert.h>
#include <strings.h>

typedef uint64_t uint64_t;

typedef struct genrsa_options_s
{
	int out_fd;
} genrsa_options_t;

typedef struct rsa_options_s
{
	char *inform;
	char *outform;
	int in_fd;
	int out_fd;
	int passin_fd;
	uint8_t des;
	uint8_t check;
	uint8_t text;
	uint8_t no_out;
	uint8_t modulus;
	uint8_t pub_in;
	uint8_t pub_out;
} rsa_options_t;

typedef enum rsault_command_s {
	unknown = -1,
	ENCRYPT,
	DECRYPT
} rsault_command_t;

typedef struct rsautl_options_s
{
	int out_fd;
	int in_fd;
	int in_key;
	int is_pubkey;
	int hexdump;
	rsault_command_t task;
} rsautl_options_t;

typedef struct rsa_s
{
	uint64_t modulus;
	uint64_t pub_exponent;
	uint64_t priv_exponent;
	uint32_t primes[2];
	uint32_t exponents[2];
	int32_t coefficient;
} rsa_t;



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
uint8_t *base64_decode(uint8_t *bytes, size_t len);

rsa_t	asn_decode_rsa(uint8_t *stream);
uint8_t	*asn_encode_rsa(rsa_t rsa);

void	print_rsa_private(int fd, rsa_t rsa);
uint8_t *read_pkey(int fd, size_t *len, long *start_idx, long *end_idx);
void	hexdump(int fd, uint8_t *bytes, size_t len);

#endif