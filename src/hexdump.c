#include <ft_ssl.h>

void	hexdump(int fd, uint8_t *bytes, size_t len)
{
	char asciimap[17];

	for (size_t i = 0; i < len;)
	{
		dprintf(fd, "%.8lx: ", i);

		memset(asciimap, 0, 17);

		for (size_t x = 0; i < len && x < 16; i++, x++)
		{
			dprintf(fd, "%.2x%c", bytes[i], (x) % 2 == 0 ? ':' : ' ');

			if (bytes[i] >= 0x21 && bytes[i] <= 0x7E)
				asciimap[x] = bytes[i];
			else asciimap[x] = '.';
		}
		
		dprintf(fd, " %s\n", asciimap);
	}
}