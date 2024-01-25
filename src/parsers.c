#include <ft_ssl.h>

void print_error(char *msg) {
	printf("ft_ssl: Error: %s\n", msg);
}

int asserted_open(char *file, int flags, int perms) {
	int fd = open(file, flags, perms);

	if (fd == -1) {
		print_error("Failed to open file.");
		exit(1);
	}

	return fd;
}

void assert_len(char *name, size_t i, size_t len) 
{
	if (i + 1 >= len) {
		printf("ft_ssl: Error: too few arguments for %s\n", name);
		exit(1);
	}
}

rsautl_options_t parse_rsautl(int argc, char **argv) {
	rsautl_options_t rv;

	memset(&rv, 0, sizeof(rv));

	rv.in_fd = STDIN_FILENO;
	rv.out_fd = STDOUT_FILENO;
	rv.task = unknown;
	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "-in") == 0) {
			assert_len("-in", i, argc);
			rv.in_fd = asserted_open(argv[i + 1], O_RDONLY, 0644);
		} else if (strcmp(argv[i], "-out") == 0) {
			assert_len("-out", i, argc);
			rv.out_fd = asserted_open(argv[i + 1], O_CREAT | O_TRUNC | O_WRONLY, 0644);
		} else if (strcmp(argv[i], "-inkey") == 0) {
			assert_len("-inkey", i, argc);
			rv.in_key = asserted_open(argv[i + 1], O_RDONLY, 0644);
		} else if (strcmp(argv[i], "-encrypt") == 0) {
			rv.task = ENCRYPT;
		} else if (strcmp(argv[i], "-decrypt") == 0) {
			rv.task = DECRYPT;
		} else if (strcmp(argv[i], "-hexdump") == 0) {
			rv.hexdump = 1;
		}
	}

	if (rv.in_key == 0) {
		printf("ft_ssl: Error: no key provided\n");
		exit(1);
	}

	if (rv.task == unknown) {
		printf("ft_ssl: Error: no task provided\n");
		exit(1);
	}

	return rv;
}

rsa_options_t parse_rsa(int argc, char **argv) {
	rsa_options_t rv;

	memset(&rv, 0, sizeof(rv));

	rv.in_fd = STDIN_FILENO;
	rv.out_fd = STDOUT_FILENO;
	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "-inform") == 0) {
			assert_len("-inform", i, argc);

			if (strcmp(argv[i + 1], "PEM") != 0) {
				print_error("Only PEM is allowed.");
				exit(1);
			}

			rv.inform = argv[i + 1];
		} else if (strcmp(argv[i], "-outform") == 0) {
			assert_len("-outform", i, argc);

			if (strcmp(argv[i + 1], "PEM") != 0) {
				print_error("Only PEM is allowed.");
				exit(1);
			}

			rv.outform = argv[i + 1];
		} else if (strcmp(argv[i], "-in") == 0) {
			assert_len("-in", i, argc);
			rv.in_fd = asserted_open(argv[i + 1], O_RDONLY, 0644);
		} else if (strcmp(argv[i], "-passin") == 0) {
			assert_len("-passin", i, argc);
			rv.passin = argv[i + 1];
		} else if (strcmp(argv[i], "-passout") == 0) {
			assert_len("-passout", i, argc);
			rv.passout = argv[i + 1];
		} else if (strcmp(argv[i], "-out") == 0) {
			assert_len("-out", i, argc);
			rv.out_fd = asserted_open(argv[i + 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
		} else if (strcmp(argv[i], "-passout") == 0) {
			assert_len("-passout", i, argc);
			rv.in_fd = asserted_open(argv[i + 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
		} else if (strcmp(argv[i], "-des") == 0) {
			rv.des = 1;
		} else if (strcmp(argv[i], "-text") == 0) {
			rv.text = 1;
		} else if (strcmp(argv[i], "-noout") == 0) {
			rv.no_out = 1;
		} else if (strcmp(argv[i], "-modulus") == 0) {
			rv.modulus = 1;
		} else if (strcmp(argv[i], "-pubin") == 0) {
			rv.pub_in = 1;
		} else if (strcmp(argv[i], "-pubout") == 0) {
			rv.pub_out = 1;
		} else if (strcmp(argv[i], "-check") == 0) {
			rv.check = 1;
		}
	}
	
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
				printf("ft_ssl: Error: failed to open %s\n", argv[i + 1]);
				exit(1);
			}
			rv.out_fd = fd;
		}
	}
	
	return rv;
}