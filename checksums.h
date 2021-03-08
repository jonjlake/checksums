#ifndef CHECKSUMS_H
#define CHECKSUMS_H

#include <stdint.h> // uint32_t

void gen_checksum_invsum(char *input_string, uint32_t input_size,
		uint32_t num_checksum_bytes);

#endif // CHECKSUMS_H
