#include <ft_ssl.h>

int main(int argc, char **argv)
{

	if (argc == 1) {
		printf("usage: ft_ssl command [command opts] [command args]\n");
		return 1;
	}

	if (strcmp(argv[1], "genrsa") == 0) {
		return handle_genrsa(parse_genrsa(argc - 2, &argv[2]));
	} else if (strcmp(argv[1], "rsa") == 0) {
		return handle_rsa(parse_rsa(argc - 2, &argv[2]));
	} else if (strcmp(argv[1], "rsautl") == 0) {
		return handle_rsautl(parse_rsautl(argc - 2, &argv[2]));

	} else {
		printf("ft_ssl: Error: '%s' is an invalid command.\n", argv[1]);
		printf("Standard commands:\n\tgenrsa\n\trsa\n\trsautil\n");
		return 1;
	}

}
