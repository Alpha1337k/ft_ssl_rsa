#include <ft_ssl.h>

uint8_t get_value(uint8_t byte, uint8_t offset, uint8_t len)
{
	byte <<= offset;
	byte >>= 8 - len;

	return byte;
}

uint8_t get_base64_val(uint8_t byte)
{
	if (byte == 62) return '+';
	if (byte == 63) return '/';
	if (byte >= 52) return '0' + byte - 52;
	if (byte >= 26) return 'a' + byte - 26;
	return 'A' + byte;
	
}

uint8_t *base64_encode(uint8_t *bytes, size_t len)
{
	size_t encoded_size = len * 1.3333334;
	encoded_size += len % 4;

	uint8_t *encoded = malloc(sizeof(uint8_t) * encoded_size);

	size_t encoded_idx = 0;

	uint8_t byte_offset = 0;
	uint8_t bit_offset = 0;

	while (byte_offset < len)
	{
		uint8_t fetched = get_value(bytes[byte_offset], bit_offset, 8 - bit_offset >= 6 ? 6 : 8 - bit_offset);

		printf("F1 | BO:%d BY:%d V:%x R:%x\n", bit_offset, byte_offset, bytes[byte_offset], fetched);

		if (bit_offset + 6 > 8)
		{
			uint8_t bits_done = 8 - bit_offset;

			fetched <<= 6 - bits_done;

			byte_offset++;

			if (byte_offset < len) {
				printf("F2 | BD:%d BO:%d V:%x R:%x F:%x\n", bits_done, bit_offset, bytes[byte_offset], get_value(bytes[byte_offset], 0, 6 - bits_done), fetched);
				fetched |= get_value(bytes[byte_offset], 0, 6 - bits_done);
				bit_offset = (bit_offset + 6) % 8;
			} else {
				byte_offset++;
			}


		} else {
			bit_offset += 6;
		}
		if (bit_offset == 0) byte_offset++;

		printf("F3: R:%x %c\n", fetched, get_base64_val(fetched));

		encoded[encoded_idx++] = get_base64_val(fetched);

		printf("\n");
	}

	while (encoded_idx % 4 != 0)
	{
		encoded[encoded_idx++] = '=';
	}
	
	
	
	printf("RESULT: %s\n", encoded);
	return encoded;
}

uint8_t *base64_decode(uint8_t *bytes, size_t len)
{
	uint8_t *decoded = malloc(sizeof(uint8_t) * len * 1.33333334);
}
