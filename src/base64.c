#include <ft_ssl.h>
#include <math.h>

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

uint8_t decode_base64_val(uint8_t byte)
{
	if (byte == '=') return 0;
	if (byte == '+') return 62;
	if (byte == '/') return 63;
	if (byte >= 'a') return 26 + byte - 'a';
	if (byte >= 'A') return byte - 'A';
	if (byte >= '0') return 52 + byte - '0';

	assert(0);
}

uint8_t *base64_encode(uint8_t *bytes, size_t len)
{
	size_t encoded_size = ceilf(len * 1.3333333334);
	encoded_size += len % 4;

	uint8_t *encoded = malloc(sizeof(uint8_t) * encoded_size + 1);
	memset(encoded, 0, encoded_size + 1);

	size_t encoded_idx = 0;
 
	uint64_t byte_offset = 0;
	uint8_t bit_offset = 0;

	while (byte_offset < len)
	{
		uint8_t fetched = get_value(bytes[byte_offset], bit_offset, 8 - bit_offset >= 6 ? 6 : 8 - bit_offset);

		if (bit_offset + 6 > 8)
		{
			uint8_t bits_done = 8 - bit_offset;

			fetched <<= 6 - bits_done;

			byte_offset++;

			if (byte_offset < len) {
				fetched |= get_value(bytes[byte_offset], 0, 6 - bits_done);
				bit_offset = (bit_offset + 6) % 8;
			} else {
				byte_offset++;
			}


		} else {
			bit_offset += 6;
		}
		if (bit_offset == 0) byte_offset++;

		// printf("S: %x %c\n", fetched, get_base64_val(fetched));
		encoded[encoded_idx++] = get_base64_val(fetched);
	}
	// encoded_idx--;

	while (encoded_idx % 4 != 0 && encoded_idx % 4 != 4)
	{
		encoded[encoded_idx++] = '=';
	}

	encoded[encoded_idx] = 0;
	
	return encoded;
}

void print_decoded(uint8_t *decoded, size_t len)
{
	printf("Decoded: ");

	for (size_t i = 0; i < len; i++)
	{
		printf("%.2x ", decoded[i]);
	}

	printf("\n");
}

uint8_t *base64_decode(uint8_t *bytes, size_t len)
{
	uint8_t *decoded = malloc(sizeof(uint8_t) * len / 1 + 1);

	size_t encoded_idx = 0;

	uint64_t byte_offset = 0;
	uint8_t bit_offset = 0;

	size_t bytes_len = strlen((char *)bytes);

	while (byte_offset < bytes_len)
	{
		uint8_t first_mask = 0x3F >> (bit_offset);
		uint8_t remaining_bit_len = 8 - (6 - bit_offset);
		uint8_t second_mask = (uint8_t)(0x3F >> (6 - remaining_bit_len)) << (6 - remaining_bit_len);
		uint8_t first_mask_offset = bit_offset + 2;
		uint8_t second_mask_offset = 6 - remaining_bit_len;

		if (bytes[byte_offset] == '=' || (remaining_bit_len < 6 && bytes[byte_offset + 1] == '=')) {
			break;
		}

		decoded[encoded_idx] = 
			(decode_base64_val(bytes[byte_offset]) & first_mask) << first_mask_offset |
			(decode_base64_val(bytes[byte_offset + 1]) & second_mask) >> second_mask_offset;

		encoded_idx++;
		bit_offset = (bit_offset + 8) % 6;
		if (bit_offset == 0) {
			byte_offset += 2;
		} else {
			byte_offset++;
		}
	}

	return decoded;
}
