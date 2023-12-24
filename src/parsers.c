#include <ft_ssl.h>

rsautl_options_t parse_rsautl(int argc, char **argv) {
	rsautl_options_t rv;

	return rv;
}

rsa_options_t parse_rsa(int argc, char **argv) {
	rsa_options_t rv;

	return rv;
}

genrsa_options_t parse_genrsa(int argc, char **argv)
{
	genrsa_options_t rv;


	rv.out_fd = STDOUT_FILENO;
	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "-out") == 0) {
			if (i + 1 >= argc) {
				printf("ft_ssl: Error: too few arguments for -out\n");
				exit(1);
			}
			int fd = open(argv[i + 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
			if (fd == -1) {
				printf("ft_ssl: Error: failed to open %s", argv[i + 1]);
				exit(1);
			}
			rv.out_fd = fd;
		}
	}
	
	return rv;
}