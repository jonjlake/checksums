#ifndef CHECKSUMS_H
#define CHECKSUMS_H

#include <stdint.h> // uint32_t
#include <stdbool.h>

/*
 * Calculates the inverse checksum of an input string. The input string
 * is unmodified.
 *
 * @param input_string[in]	String for which to calculate the checksum
 * @param input_size[in]	Size of the string (length)
 * @param num_checksum_bytes[in]	Size of checksum to calculate
 *
 * @return calculated inverse checksum
 */
char calculate_checksum_invsum(char *input_string, uint32_t input_size,
		uint32_t num_checksum_bytes);

/*
 * Calculates inverse checksum and puts it at the end of the input string
 */
void insert_checksum_invsum(char *input_string, uint32_t input_size,
		uint32_t num_checksum_bytes);

/*
 * Verifies that the checksum at the end of a string is correct
 */
bool checksum_correct(char *input_string, uint32_t input_size,
		uint32_t num_checksum_bytes);


#endif // CHECKSUMS_H
