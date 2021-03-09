#include "checksums.h"

/* Function implementations */

char calculate_checksum_invsum(char *input_string, uint32_t input_size,
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
	
	return sum;
}

void insert_checksum_invsum(char *input_string, uint32_t input_size,
		uint32_t num_checksum_bytes)
{
	char checksum = calculate_checksum_invsum(input_string, input_size, num_checksum_bytes);
	input_string[input_size - 1] = checksum;
}

bool checksum_correct(char *input_string, uint32_t input_size,
		uint32_t num_checksum_bytes)
{
	char checksum = calculate_checksum_invsum(input_string, input_size, num_checksum_bytes);

	return checksum == input_string[input_size - 1];
}

