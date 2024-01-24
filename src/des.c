#include <ft_ssl.h>

uint8_t sbox[8][4][16] = {
	{	
		{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
        {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
        {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
        {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13},
	},
	{
		{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
		{3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
		{0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
		{13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
	},
	{
		{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
		{13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
		{13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
		{1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
	},
	{
		{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
		{13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
		{10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
		{3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
	},
	{
		{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
		{14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
		{4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
		{11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
	},
	{
		{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
		{10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
		{9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
		{4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
	},
	{
		{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
		{13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
		{1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
		{6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
	},
	{
		{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
		{1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
		{7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
		{2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
	}
};

uint8_t ip[64] = {
	58, 50, 42, 34, 26, 18, 10, 2,
	60, 52, 44, 36, 28, 20, 12, 4,
	62, 54, 46, 38, 30, 22, 14, 6,
	64, 56, 48, 40, 32, 24, 16, 8,
	57, 49, 41, 33, 25, 17, 9, 1,
	59, 51, 43, 35, 27, 19, 11, 3,
	61, 53, 45, 37, 29, 21, 13, 5,
	63, 55, 47, 39, 31, 23, 15, 7
};

uint8_t nip[64] = {
	40, 8, 48, 16, 56, 24, 64, 32,
	39, 7, 47, 15, 55, 23, 63, 31,
	38, 6, 46, 14, 54, 22, 62, 30,
	37, 5, 45, 13, 53, 21, 61, 29,
	36, 4, 44, 12, 52, 20, 60, 28,
	35, 3, 43, 11, 51, 19, 59, 27,
	34, 2, 42, 10, 50, 18, 58, 26,
	33, 1, 41, 9, 49, 17, 57, 25
};

uint8_t keyp[56] = {
	57, 49, 41, 33, 25, 17, 9,
	1, 58, 50, 42, 34, 26, 18,
	10, 2, 59, 51, 43, 35, 27,
	19, 11, 3, 60, 52, 44, 36,
	63, 55, 47, 39, 31, 23, 15,
	7, 62, 54, 46, 38, 30, 22,
	14, 6, 61, 53, 45, 37, 29,
	21, 13, 5, 28, 20, 12, 4
};

uint8_t pc2[48] = {
	14, 17, 11, 24, 1, 5,
	3, 28, 15, 6, 21, 10,
	23, 19, 12, 4, 26, 8,
	16, 7, 27, 20, 13, 2,
	41, 52, 31, 37, 47, 55,
	30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53,
	46, 42, 50, 36, 29, 32
};

uint8_t shifts[16] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

uint8_t expansion[48] = {
		32, 1, 2, 3, 4, 5, 4, 5,
		6, 7, 8, 9, 8, 9, 10, 11,
		12, 13, 12, 13, 14, 15, 16, 17,
		16, 17, 18, 19, 20, 21, 20, 21,
		22, 23, 24, 25, 24, 25, 26, 27,
		28, 29, 28, 29, 30, 31, 32, 1
	};

uint8_t unexpansion[32] = {
	16, 7, 20, 21,
	29, 12, 28, 17,
	1, 15, 23, 26,
	5, 18, 31, 10,
	2,  8, 24, 14,
	32, 27,  3,  9,
	19, 13, 30,  6,
	22, 11,  4, 25
};

uint64_t permute_64(uint64_t val, size_t in_len, uint8_t *table, size_t out_len)
{
	uint64_t rv = 0;

	for (size_t i = 0; i < out_len; i++)
	{
		uint64_t tmp = (val >> (in_len - table[i])) & 0x1;

		rv |= (tmp << (out_len - 1 - i));

		// printf("F: %lu %ld\n", rv, tmp);
	}
	
	return rv;
}

uint64_t	hash_key(char *key)
{
	uint64_t hash = 0;
	uint8_t *hash_bytes = (uint8_t *)&hash;

	for (size_t i = 0; key[i]; i++)
	{
		hash_bytes[i % 7] ^= key[i];
	}
	
	return hash;
}

uint64_t left_rotate(uint64_t val, size_t count, size_t len)
{
	uint64_t mask = ((1l << len) - 1);

	// printf("%lX %lX %lX %lX\n", 
	// 	mask,
	// 	(val << count), 
	// 	(val >> (len - count), 
	// 	((val << count) | (val >> (len - count)))),
	// 	(((val << count) | (val >> (len - count))) & mask)
	// );

	return ((val << count) | (val >> (len - count))) & mask;
}

void print_pos(uint64_t v)
{
	for (size_t i = 0; i < 64; i++)
	{
		printf("%d", (v & (1lu << 63 - i)) != 0);
	}
	printf("\n");
}

uint8_t *des_encrypt(uint64_t *bytes, char *key, size_t long_len)
{
	key = "\xDD\xCC\x36\x27\x18\x09\xBB\xAA";

	uint64_t key_long = ((uint64_t *)key)[0];
	uint64_t hashed_key = permute_64(key_long, 64, keyp, 56);


	print_pos(hashed_key);

	printf("Hashed key: 0x%lX KL: 0x%lX\n", hashed_key, key_long);

	uint64_t keys[16];

	uint64_t key_left = (hashed_key >> 28) & 0xFFFFFFF;
	uint64_t key_right = hashed_key & 0xFFFFFFF;

	for (size_t i = 0; i < 16; i++)
	{
		key_right = left_rotate(key_right, shifts[i], 28);
		key_left = left_rotate(key_left, shifts[i], 28);
		printf("KL: %lx\n", key_left);
		printf("KR: %lx\n", key_right);


		uint64_t concat = (key_left << 28) | key_right; 

		printf("CC: %lx\n", concat);
	
		keys[i] = permute_64(concat, 56, pc2, 48);
		printf("keys[%ld]: 0x%lX\n", i, keys[i]);
	}

	for (size_t i = 0; i < long_len; i++)
	{
		printf("Before IP: %lx\n", bytes[i]);
		uint64_t chunk = permute_64(bytes[i], 64, ip, 64);
		printf("After IP: %lx\n", chunk);
		uint32_t chunk_left = chunk >> 32;
		uint64_t chunk_right = chunk & 0xFFFFFFFF;

		for (size_t n = 0; n < 16; n++)
		{
			uint32_t tmp = chunk_right;

			// printf("RE: %lX\n", chunk_right);
			uint64_t right_expanded = permute_64(chunk_right, 32, expansion, 48);

			// printf("RE: %lX\n", right_expanded);
			right_expanded ^= keys[n];
			right_expanded &= 0xFFFFFFFFFFFF;
			// printf("XORED: %lX %lX\n", right_expanded, keys[n]);

			uint64_t right_subsituted = 0;

			for (size_t c = 0; c < 8; c++)
			{
				uint8_t bits = (right_expanded >> (42 - c * 6)) & 0x3F;
				uint8_t row = (((bits & 0x20) != 0) << 1) | bits & 0x1;
				uint8_t col = (bits & 0x1E) >> 1;

				// printf("RC: %d %d\n\n", row, col);

				right_subsituted <<= 4;
				right_subsituted |= sbox[c][row][col];
			}

			// printf("SBOX: %lX\n", right_subsituted);

			chunk_right = permute_64(right_subsituted, 32, unexpansion, 32);
			// printf("PERM: %lX\n", chunk_right);
			chunk_right &= 0xFFFFFFFF;
			chunk_right ^= chunk_left;

			// printf("XOR2: %lX\n", chunk_right);

			chunk_left = tmp;

			printf("Round %ld L:0x%X R:0x%X\n", n, chunk_left, chunk_right);
			
		}
		
		chunk = ((uint64_t)chunk_right << 32) | chunk_left;
	
		chunk = permute_64(chunk, 64, nip, 64);

		bytes[i] = chunk;

		printf("RESULT: %lX\n", bytes[i]);
	}

	return (uint8_t *)bytes;
}
