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

#define PRIVATE_START "-----BEGIN RSA PRIVATE KEY-----\n"
#define PRIVATE_END "\n-----END RSA PRIVATE KEY-----"
#define PUBLIC_START "-----BEGIN PUBLIC KEY-----\n"
#define PUBLIC_END "\n-----END PUBLIC KEY-----"
#define ENCRYPT_INFO "Proc-Type: 4,ENCRYPTED\nDEK-Info: DES-CBC\n"

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
	char *passin;
	char *passout;
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

typedef struct priv_rsa_s
{
	uint64_t modulus;
	uint64_t pub_exponent;
	uint64_t priv_exponent;
	uint32_t primes[2];
	uint32_t exponents[2];
	int32_t coefficient;
} priv_rsa_t;

typedef struct pub_rsa_s
{
	uint64_t modulus;
	uint64_t pub_exponent;
} pub_rsa_t;



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

uint8_t	*asn_encode_priv_rsa(priv_rsa_t rsa);
uint8_t	*asn_encode_pub_rsa(pub_rsa_t rsa);

void	print_rsa_private(int fd, priv_rsa_t rsa, char *passout);
void	print_rsa_public(int fd, pub_rsa_t rsa);

uint8_t *read_key(int fd, long *start_idx,long *end_idx,char *start_str,char *end_str, int *is_encrypted);
void	hexdump(int fd, uint8_t *bytes, size_t len);

priv_rsa_t	asn_decode_priv_rsa(uint8_t *stream, int *error);
pub_rsa_t	asn_decode_pub_rsa(uint8_t *stream, int *error);

uint8_t *des(uint64_t *bytes, char *key, size_t long_len, uint8_t decrypt);

char *get_pass(char *pass_cmd, char *prompt);

priv_rsa_t	parse_private_key(int in_fd, char *passin);
pub_rsa_t	parse_public_key(int in_fd);

#endif