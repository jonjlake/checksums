#include "checksums.h"

#include <stdio.h>
//#include <stdint.h>
#include <stdbool.h>

/* Error return enum */
typedef enum JON_TEST_ERR
{
	JON_TEST_ERR_NO_ERR,
	JON_TEST_ERR_ERR,
} JON_TEST_ERR;

typedef struct test_results
{
	uint32_t num_succeeded;
	uint32_t num_failed;
	uint32_t num_unknown;
} test_results;

/* Functions */
void run_all_tests(void);
void run_test(JON_TEST_ERR (*test_fxn)(void), char *test_name);
void gen_checksum_invsum(char *input_string, uint32_t input_size,
		uint32_t num_checksum_bytes);
JON_TEST_ERR test_gen_checksum_invsum(char *input_string, uint32_t input_size,
		uint32_t num_checksum_bytes, char checksum_exp);
JON_TEST_ERR test_gen_checksum_invsum_correct(void);
JON_TEST_ERR test_gen_checksum_invsum_incorrect(void);

/* Main */
int main(int argc, char *argv[])
{
	/* Print results here, or create results struct/print? */
	run_all_tests();

	return 0;
}

/* Function implementations */

void run_all_tests(void)
{
	run_test(test_gen_checksum_invsum_correct, "Correct inverse checksum");
	run_test(test_gen_checksum_invsum_incorrect, "Incorrect inverse checksum");
}

// Add a return here indicating if any failed, or were unrecognized
void run_test(JON_TEST_ERR (*test_fxn)(void), char *test_name)
{
	JON_TEST_ERR ret_err;

	ret_err = test_fxn();

	switch (ret_err)
	{
		case JON_TEST_ERR_NO_ERR:
			printf("Test: %s SUCCEEDED\n", test_name);
			break;
		case JON_TEST_ERR_ERR:
			printf("Test: %s FAILED\n", test_name);
			break;
		default:
			printf("Test: %s RESULT UNRECOGNIZED\n", test_name);
			break;
	}
}

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

JON_TEST_ERR test_gen_checksum_invsum(char *input_string, uint32_t input_size,
		uint32_t num_checksum_bytes, char checksum_exp)
{
	/* These are the checks we do */
	bool string_unmodified, checksum_correct;
	char *input_cpy = (char *)calloc(input_size, sizeof(*input_cpy));
	memcpy(input_cpy, input_string, input_size);

	gen_checksum_invsum(input_string, input_size, num_checksum_bytes);

//	printf("test_gen_checksum_invsum generated checksum %d\n", input_string[input_size - 1]);

	string_unmodified = (0 == memcmp(input_string, input_cpy, input_size - 1));

	checksum_correct = (input_string[input_size - 1] == checksum_exp);

	free(input_cpy);

	return string_unmodified && checksum_correct ? JON_TEST_ERR_NO_ERR : JON_TEST_ERR_ERR;
}

/*
 * H: 72
 * e: 101
 * l: 108
 * o: 111
 * ,: 44
 * space: 32
 * c: 99
 * h: 104 
 * k: 107
 * s: 115
 * u: 117
 * m: 109
 * !: 33
 * -> sum: 1,460 -> 0101 1011 0100 (trunc. at 180?)
 *  	-> But 8-bit 2's complement: 100 1011 + 1 = 75 + 1 = 76 (-76) 
 */
JON_TEST_ERR test_gen_checksum_invsum_correct(void)
{
	JON_TEST_ERR ret_err;

	char *input_string = "Hello, checksum!";
	uint32_t input_size = 17; // chars + 1
	uint32_t num_checksum_bytes = 1;
	char checksum_exp = -76;
	checksum_exp = ~checksum_exp; // 75?

//	printf("invsum_correct cs test: checksum_exp: %d, non-inv: %d\n", checksum_exp, -76);

	return test_gen_checksum_invsum(input_string, input_size, num_checksum_bytes, checksum_exp);
}

JON_TEST_ERR test_gen_checksum_invsum_incorrect(void)
{
	// This tests that we catch an incorrectly generated checksum
	JON_TEST_ERR ret_err;

	char *input_string = "Hello, checksum!";
	uint32_t input_size = 17; 
	uint32_t num_checksum_bytes = 1;
	char checksum_exp = -76; // This is wrong. Test that our test catches it
	//checksum_exp = ~checksum_exp; // 75

	ret_err = test_gen_checksum_invsum(input_string, input_size, num_checksum_bytes, checksum_exp);

	// Return the opposite of the error
	return JON_TEST_ERR_ERR == ret_err ? JON_TEST_ERR_NO_ERR : JON_TEST_ERR_ERR;
}
