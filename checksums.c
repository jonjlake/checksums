#include "checksums.h"

/* Function implementations */

void gen_checksum_invsum(char *input_string, uint32_t input_size,
		uint32_t num_checksum_bytes)
{
	char sum = 0;
	int i;
	for (i = 0; i < (input_size - num_checksum_bytes); i++)
	{
		sum += input_string[i];
//		printf("Added %d. Sum is %d\n", input_string[i], sum);
	}

//	printf("Sum before inv: %d\n", sum);
	sum = ~sum;
//	printf("Sum after inv: %d\n", sum);

	input_string[input_size - 1] = sum;
}


