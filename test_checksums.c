#include "checksums.h"
#include "../unit_test_fw/unit_test_fw.h"

/* Std includes */
#include <stdio.h>
#include <stdbool.h>


/* Functions */
JON_TEST_ERR test_insert_checksum_invsum(char *input_string, uint32_t input_size,
		uint32_t num_checksum_bytes, char checksum_exp);
JON_TEST_ERR test_insert_checksum_invsum_correct(void);
JON_TEST_ERR test_insert_checksum_invsum_incorrect(void);
JON_TEST_ERR test_checksum_correct_cs_correct(void);
JON_TEST_ERR test_checksum_correct_cs_incorrect(void);
JON_TEST_ERR test_calculate_checksum_invsum_1(void);

#define FG_TEST_STRING "Hello, checksum!"
#define FG_TEST_STRING_CHECKSUM 75
#define FG_TEST_STRING_WITH_CHECKSUM "Hello, checksum!K"
#define FG_TEST_STRING_WITH_CHECKSUM_INC "Hello, checksum!L"

/* 
 * FG_TEST_STRING_CHECKSUM calculation:
 * 
 * H: 	72
 * e: 	101
 * l: 	108
 * o: 	111
 * ,:	44
 * sp: 	32
 * c: 	99
 * h: 	104 
 * k: 	107
 * s: 	115
 * u: 	117
 * m: 	109
 * !: 	33
 * 
 * -> sum: 1,460 -> 0101 1011 0100 (trunc. at 180?)
 *  	-> But 8-bit 2's complement: 100 1011 + 1 = 75 + 1 = 76 (-76) 
 *  	-> Bit inverse: 75
 */

/* Main */
int main(int argc, char *argv[])
{
	/* Print results here, or create results struct/print? */
	//run_all_tests();
	init_test_suite("Checksum tests");
	
	add_test(test_insert_checksum_invsum_correct, "Correct inverse checksum insertion");
	add_test(test_insert_checksum_invsum_incorrect, "Incorrect inverse checksum insertion");

	add_test(test_checksum_correct_cs_correct, "Correct embedded inverse checksum");
	add_test(test_checksum_correct_cs_incorrect, "Incorrect embedded inverse checksum");
	add_test(test_calculate_checksum_invsum_1, "Calculate inverse checksum");
	run_all_tests();

	delete_test_suite();
	return 0;
}

/* Function implementations */

/* Main test function for "insert_checksum_invsum" function */
JON_TEST_ERR test_insert_checksum_invsum(char *input_string, uint32_t input_size,
		uint32_t num_checksum_bytes, char checksum_exp)
{
	/* These are the checks we do */
	bool string_unmodified, checksum_correct;
	char *input_cpy = (char *)calloc(input_size, sizeof(*input_cpy));
	memcpy(input_cpy, input_string, input_size);

	insert_checksum_invsum(input_string, input_size, num_checksum_bytes);

	string_unmodified = (0 == memcmp(input_string, input_cpy, input_size - 1));

	checksum_correct = (input_string[input_size - 1] == checksum_exp);

	free(input_cpy);

	return string_unmodified && checksum_correct ? JON_TEST_ERR_NO_ERR : JON_TEST_ERR_ERR;
}

JON_TEST_ERR test_insert_checksum_invsum_correct(void)
{
	JON_TEST_ERR ret_err;

	char *input_string = FG_TEST_STRING;
	uint32_t input_size = strlen(input_string) + 1;
	uint32_t num_checksum_bytes = 1;
	char checksum_exp = FG_TEST_STRING_CHECKSUM;	

	return test_insert_checksum_invsum(input_string, input_size, num_checksum_bytes, checksum_exp);
}

JON_TEST_ERR test_insert_checksum_invsum_incorrect(void)
{
	// This tests that we catch an incorrectly generated checksum
	JON_TEST_ERR ret_err;

	char *input_string = FG_TEST_STRING;
	uint32_t input_size = strlen(input_string) + 1; 
	uint32_t num_checksum_bytes = 1;
	char checksum_exp = FG_TEST_STRING_CHECKSUM + 1; // This is wrong. Test that our test catches it

	ret_err = test_insert_checksum_invsum(input_string, input_size, num_checksum_bytes, checksum_exp);

	// Return the opposite of the error
	return JON_TEST_ERR_ERR == ret_err ? JON_TEST_ERR_NO_ERR : JON_TEST_ERR_ERR;
}

/* Main test function for "checksum_correct" function */
/*JON_TEST_ERR test_checksum_correct(char *input_string, uint32_t input_size,
		uint32_t num_checksum_bytes)
{
	
}*/

JON_TEST_ERR test_checksum_correct_cs_correct(void)
{
	char *input_string = FG_TEST_STRING_WITH_CHECKSUM;
	uint32_t input_size = strlen(input_string);
	uint32_t num_checksum_bytes = 1;

	bool retval = checksum_correct(input_string, input_size, 
			num_checksum_bytes);

	return retval ? JON_TEST_ERR_NO_ERR : JON_TEST_ERR_ERR;
}

JON_TEST_ERR test_checksum_correct_cs_incorrect(void)
{
	char *input_string = FG_TEST_STRING_WITH_CHECKSUM_INC;
	uint32_t input_size = strlen(input_string);
	uint32_t num_checksum_bytes = 1;

	bool retval = checksum_correct(input_string, input_size, 
			num_checksum_bytes);

	return retval ? JON_TEST_ERR_ERR : JON_TEST_ERR_NO_ERR;
}

/*JON_TEST_ERR test_calculate_checksum_invsum(char *input_string, 
		uint32_t input_size, uint32_t num_checksum_bytes)
{
}*/

JON_TEST_ERR test_calculate_checksum_invsum_1(void)
{
	char *input_string = FG_TEST_STRING;
	uint32_t input_size = strlen(input_string) + 1;
	uint32_t num_checksum_bytes = 1;
	char checksum_exp = FG_TEST_STRING_CHECKSUM;

	char checksum_calc = calculate_checksum_invsum(input_string,
			input_size, num_checksum_bytes);

	return checksum_calc == checksum_exp ? JON_TEST_ERR_NO_ERR : 
		JON_TEST_ERR_ERR;
}
