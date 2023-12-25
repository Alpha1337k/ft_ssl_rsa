#include <ft_ssl.h>

int main(int argc, char **argv)
{
	// base64_encode("Man", 3);
	// base64_encode("Ma", 2);
	// base64_encode("M", 1);
	// base64_encode("light work.", 11);
	// base64_encode("light work", 10);

	// base64_decode("TWFu", 4);
	// base64_decode("TWE=", 4);
	// base64_decode("TQ==", 4);
	base64_decode("bGlnaHQgdw==", 12);

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
