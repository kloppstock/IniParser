#include "test.h"
#include "../include/ini_parser.h"
#include <unistd.h>
#include <fcntl.h>

struct data_structure {
	size_t index;
	char str[1000];
};

int callback(const char* section, const char* name, const char* value, void* data) {
	struct data_structure* ds = (struct data_structure*)data;
	
	strcpy(ds->str + ds->index, section);
	ds->index += strlen(section);
	ds->str[ds->index++] = ':';
	ds->str[ds->index++] = ' ';
	strcpy(ds->str + ds->index, name);
	ds->index += strlen(name);
	ds->str[ds->index++] = '=';
	ds->str[ds->index++] = ' ';
	strcpy(ds->str + ds->index, value);
	ds->index += strlen(value);
	ds->str[ds->index++] = '\n';
	ds->str[ds->index] = '\0';

	return 1;
}

size_t load_file(char buffer[1000], const char* path) {
	FILE* fd = fopen(path, "r");
	size_t len = fread(buffer, 1, 999, fd);
	fclose(fd);
	buffer[len] = '\0';
	return len;
}

int callback_empty(const char* section, const char* name, const char* value, void* data) {
	(void)data;
	(void)section;
	(void)name;
	(void)value;
	return 1;
}

int callback_fail(const char* section, const char* name, const char* value, void* data) {
	(void)data;
	(void)section;
	(void)name;
	(void)value;
	return 0;
}

void print_test_info(const char* test) {
	printf("INFO: Executing %s!\n", test);
}

int hex_digit_to_binary_test() {
	print_test_info("hex_digit_to_binary_test()");
	
	char expected1 = 5;
	char expected2 = 0xb;
	char expected3 = -1;

	char input1 = '5';
	char input2 = 'b';
	char input3 = 'B';
	char input4 = 'Y';

	char actual1 = hex_digit_to_binary(input1);
	char actual2 = hex_digit_to_binary(input2);
	char actual3 = hex_digit_to_binary(input3);
	char actual4 = hex_digit_to_binary(input4);

	if(assert_equals_int(expected1, actual1))
		return 1;
	if(assert_equals_int(expected2, actual2))
		return 1;
	if(assert_equals_int(expected2, actual3))
		return 1;
	if(assert_equals_int(expected3, actual4))
		return 1;
	
	return 0;
}

int uctoutf8_test() {
	print_test_info("uctoutf8_test()");
	uint32_t expected1 = 0x3b;
	uint32_t expected2 = 0xce8f;
	uint32_t expected3 = 0xe0a69e;
	uint32_t expected4 = 0xf3a080a0;
	uint32_t expected5 = 0xffffffff;
	
	uint32_t input1 = 0x3b;
	uint32_t input2 = 0x38f;
	uint32_t input3 = 0x99e;
	uint32_t input4 = 0xe0020;
	uint32_t input5 = 0x13abcdef;

	uint32_t actual1 = uctoutf8(input1);
	uint32_t actual2 = uctoutf8(input2);
	uint32_t actual3 = uctoutf8(input3);
	uint32_t actual4 = uctoutf8(input4);
	uint32_t actual5 = uctoutf8(input5);

	if(assert_equals_unsigned_int(expected1, actual1))
		return 1;
	if(assert_equals_unsigned_int(expected2, actual2))
		return 1;
	if(assert_equals_unsigned_int(expected3, actual3))
		return 1;
	if(assert_equals_unsigned_int(expected4, actual4))
		return 1;
	if(assert_equals_unsigned_int(expected5, actual5))
		return 1;
	
	return 0;
}

int is_spacing_test() {
	print_test_info("is_spacing_test()");
	int expected1 = 1;
	int expected2 = 0;

	char input1 = ' ';
	char input2 = '\t';
	char input3 = 'f';

	int actual1 = is_spacing(input1);
	int actual2 = is_spacing(input2);
	int actual3 = is_spacing(input3);

	if(assert_equals_int(expected1, actual1))
		return 1;
	if(assert_equals_int(expected1, actual2))
		return 1;
	if(assert_equals_int(expected2, actual3))
		return 1;
	
	return 0;
}

int is_newline_test() {
	print_test_info("is_newline_test()");
	int expected1 = 1;
	int expected2 = 0;

	char input1 = '\n';
	char input2 = '\r';
	char input3 = 's';

	int actual1 = is_newline(input1);
	int actual2 = is_newline(input2);
	int actual3 = is_newline(input3);

	if(assert_equals_int(expected1, actual1))
		return 1;
	if(assert_equals_int(expected1, actual2))
		return 1;
	if(assert_equals_int(expected2, actual3))
		return 1;
	
	return 0;
}

int is_whitespace_test() {
	print_test_info("is_whitespace_test()");
	int expected1 = 1;
	int expected2 = 0;

	char input1 = '\n';
	char input2 = '\t';
	char input3 = 's';

	int actual1 = is_whitespace(input1);
	int actual2 = is_whitespace(input2);
	int actual3 = is_whitespace(input3);

	if(assert_equals_int(expected1, actual1))
		return 1;
	if(assert_equals_int(expected1, actual2))
		return 1;
	if(assert_equals_int(expected2, actual3))
		return 1;
	
	return 0;
}

int is_special_character_test() {
	print_test_info("is_special_character_test()");
	int expected1 = 1;
	int expected2 = 0;

	char comment = 'c';
	char equals = 'e';

	char input1 = '\\';
	char input2 = '\'';
	char input3 = comment;
	char input4 = equals;
	char input5 = 's';

	int actual1 = is_special_character(input1, comment, equals);
	int actual2 = is_special_character(input2, comment, equals);
	int actual3 = is_special_character(input3, comment, equals);
	int actual4 = is_special_character(input4, comment, equals);
	int actual5 = is_special_character(input5, comment, equals);

	if(assert_equals_int(expected1, actual1))
		return 1;
	if(assert_equals_int(expected1, actual2))
		return 1;
	if(assert_equals_int(expected1, actual3))
		return 1;
	if(assert_equals_int(expected1, actual4))
		return 1;
	if(assert_equals_int(expected2, actual5))
		return 1;
	
	return 0;
}

int is_utf8_part_test() {
	print_test_info("is_utf8_part_test()");
	int expected1 = 1;
	int expected2 = 0;

	char input1 = (char)128;
	char input2 = 127;

	int actual1 = is_utf8_part(input1);
	int actual2 = is_utf8_part(input2);

	if(assert_equals_int(expected1, actual1))
		return 1;
	if(assert_equals_int(expected2, actual2))
		return 1;
	
	return 0;
}

int is_printable_char_test() {
	print_test_info("is_printable_char_test()");
	int expected1 = 1;
	int expected2 = 0;

	char input1 = 'd';
	char input2 = (char)128;
	char input3 = '\t';
	char input4 = 2;

	int actual1 = is_printable_char(input1);
	int actual2 = is_printable_char(input2);
	int actual3 = is_printable_char(input3);
	int actual4 = is_printable_char(input4);

	if(assert_equals_int(expected1, actual1))
		return 1;
	if(assert_equals_int(expected1, actual2))
		return 1;
	if(assert_equals_int(expected1, actual3))
		return 1;
	if(assert_equals_int(expected2, actual4))
		return 1;
	
	return 0;
}

int lskipws_test() {
	print_test_info("lskipws_test()");
	size_t expected1 = 0;
	size_t expected2 = 2;
	
	char input1[20] = "asdf";
	char input2[20] = "  asdf";

	size_t actual1 = lskipws(input1);
	size_t actual2 = lskipws(input2);

	if(assert_equals_unsigned_int(expected1, actual1))
		return 1;
	if(assert_equals_unsigned_int(expected2, actual2))
		return 1;

	return 0;
}

int rskipws_test() {
	print_test_info("rskipws_test()");
	size_t expected1 = 4;
	size_t expected2 = 6;
	size_t expected3 = 0;
	size_t expected4 = 0;
	
	char input1[20] = "asdf";
	char input2[20] = "  asdf  ";
	char input3[20] = "";
	char input4[20] = "   ";

	size_t actual1 = rskipws(input1);
	size_t actual2 = rskipws(input2);
	size_t actual3 = rskipws(input3);
	size_t actual4 = rskipws(input4 + 1);

	if(assert_equals_unsigned_int(expected1, actual1))
		return 1;
	if(assert_equals_unsigned_int(expected2, actual2))
		return 1;
	if(assert_equals_unsigned_int(expected3, actual3))
		return 1;
	if(assert_equals_unsigned_int(expected4, actual4))
		return 1;

	return 0;
}

//TODO: is this max function still needed?
int max_size_t_test() {
	print_test_info("max_size_t_test()");
	size_t expected1 = 25;
	size_t expected2 = 3;
	size_t expected3 = 2;

	size_t input1_f = 25;
	size_t input1_s = 13;
	size_t input2_f = 3;
	size_t input2_s = 3;
	size_t input3_f = 0;
	size_t input3_s = 2;

	size_t actual1 = max_size_t(input1_f, input1_s);
	size_t actual2 = max_size_t(input2_f, input2_s);
	size_t actual3 = max_size_t(input3_f, input3_s);
	
	if(assert_equals_unsigned_int(expected1, actual1))
		return 1;
	if(assert_equals_unsigned_int(expected2, actual2))
		return 1;
	if(assert_equals_unsigned_int(expected3, actual3))
		return 1;

	return 0;
}
/*
TODO: remove this completely:
int read_hex_from_string_test() {
	print_test_info("read_hex_from_string_test()");
	printf("WARNING: %s is deprecated!\n", "read_hex_from_string()");
	
	char expected1 = 0xa;
	char expected2 = -1;
	char expected3 = 0;
	char expected4 = 0xa;
	char expected5 = 0x20;
	char expected6 = -1;
	
	char* input1 = "a";
	char* input2 = "x";
	char* input3 = "00";
	char* input4 = "asdf";
	char* input5 = "0x20";
	char* input6 = NULL;

	size_t input1_pos = 0;
	size_t input3_pos = 0;
	size_t input5_pos = 2;

	size_t expected_pos1 = 1;
	size_t expected_pos3 = 2;
	size_t expected_pos5 = 4;
	
	char actual1 = read_hex_from_string(input1, &input1_pos);
	char actual2 = read_hex_from_string(input2, NULL);
	char actual3 = read_hex_from_string(input3, &input3_pos);
	char actual4 = read_hex_from_string(input4, NULL);
	char actual5 = read_hex_from_string(input5, &input5_pos);
	char actual6 = read_hex_from_string(input6, NULL);

	//check if position is changed
	if(assert_equals_unsigned_int(expected_pos1, input1_pos))
		return 1;
	if(assert_equals_unsigned_int(expected_pos3, input3_pos))
		return 1;
	if(assert_equals_unsigned_int(expected_pos5, input5_pos))
		return 1;

	if(assert_equals_int(expected1, actual1))
		return 1;
	if(assert_equals_int(expected2, actual2))
		return 1;
	if(assert_equals_int(expected3, actual3))
		return 1;
	if(assert_equals_int(expected4, actual4))
		return 1;
	if(assert_equals_int(expected5, actual5))
		return 1;
	if(assert_equals_int(expected6, actual6))
		return 1;
	
	return 0;
}
*/
int read_unicode_from_string_test() {
	print_test_info("read_unicode_from_string_test()");
	uint32_t expected_uc1 = 0xe2a087;
	uint32_t expected_uc2 = 0xFFFFFFFF;
	uint32_t expected_uc6 = 0xFFFFFFFF;
	uint32_t expected_uc7 = 0xF090A9AE;
	uint32_t expected_uc8 = 0xFFFFFFFF;
	size_t expected_pos1 = 6;
	size_t expected_pos2 = 10;
	size_t expected_pos3 = 10;
	size_t expected_pos4 = 0;
	size_t expected_pos5 = 0;
	size_t expected_pos6 = 0;
	size_t expected_pos7 = 6;
	size_t expected_pos8 = 0;

	size_t input_pos1 = 0;
	size_t input_pos2 = 4;
	size_t input_pos3 = 4;
	size_t input_pos4 = 0;
	size_t input_pos5 = 0;
	size_t input_pos6 = 0;
	size_t input_pos7 = 0;
	size_t input_pos8 = 0;
	char* input1 = "002807";
	char* input2 = "asdf002807";
	char* input3 = "asdf002807kk";
	char* input4 = "";
	char* input5 = "xyz";
	char* input6 = "12xy";
	char* input7 = "010A6E";
	char* input8 = "2";

	uint32_t actual0 = read_unicode_from_string(input1, NULL);
	uint32_t actual1 = read_unicode_from_string(input1, &input_pos1);
	uint32_t actual2 = read_unicode_from_string(input2, &input_pos2);
	uint32_t actual3 = read_unicode_from_string(input3, &input_pos3);
	uint32_t actual4 = read_unicode_from_string(input4, &input_pos4);
	uint32_t actual5 = read_unicode_from_string(input5, &input_pos5);
	uint32_t actual6 = read_unicode_from_string(input6, &input_pos6);
	uint32_t actual7 = read_unicode_from_string(input7, &input_pos7);
	uint32_t actual8 = read_unicode_from_string(input8, &input_pos8);
	
	if(assert_equals_unsigned_int(expected_uc1, actual0))
		return 1;
	if(assert_equals_unsigned_int(expected_uc1, actual1))
		return 1;
	if(assert_equals_unsigned_int(expected_uc1, actual2))
		return 1;
	if(assert_equals_unsigned_int(expected_uc1, actual3))
		return 1;
	if(assert_equals_unsigned_int(expected_uc2, actual4))
		return 1;
	if(assert_equals_unsigned_int(expected_uc2, actual5))
		return 1;
	if(assert_equals_unsigned_int(expected_uc6, actual6))
		return 1;
	if(assert_equals_unsigned_int(expected_uc7, actual7))
		return 1;
	if(assert_equals_unsigned_int(expected_uc8, actual8))
		return 1;

	if(assert_equals_unsigned_int(expected_pos1, input_pos1))
		return 1;
	if(assert_equals_unsigned_int(expected_pos2, input_pos2))
		return 1;
	if(assert_equals_unsigned_int(expected_pos3, input_pos3))
		return 1;
	if(assert_equals_unsigned_int(expected_pos4, input_pos4))
		return 1;
	if(assert_equals_unsigned_int(expected_pos5, input_pos5))
		return 1;
	if(assert_equals_unsigned_int(expected_pos6, input_pos6))
		return 1;
	if(assert_equals_unsigned_int(expected_pos7, input_pos7))
		return 1;
	if(assert_equals_unsigned_int(expected_pos8, input_pos8))
		return 1;
	
	return 0;
}

int has_non_whitespace_before_newline_string_test() {
	print_test_info("has_non_whitespace_before_newline_string_test()");

	int expected1 = 0;
	int expected2 = 1;
	int expected3 = 1;
	int expected4 = 0;
	int expected5 = 0;
	size_t expected_pos2 = 0;
	size_t expected_pos3 = 1;
	size_t expected_pos4 = 3;
	size_t expected_pos5 = 1;
    
	char* input1 = "";
	char* input2 = "asdf ";
	char* input3 = " d \n";
	char* input4 = " \nd";
	size_t input_pos2 = 0;
	size_t input_pos3 = 0;
	size_t input_pos4 = 2;
	size_t input_pos5 = 0;

	int actual1 = has_non_whitespace_before_newline(input1, NULL);
	int actual2 = has_non_whitespace_before_newline(input2, &input_pos2);
	int actual3 = has_non_whitespace_before_newline(input3, &input_pos3);
	int actual4 = has_non_whitespace_before_newline(input3, &input_pos4);
	int actual5 = has_non_whitespace_before_newline(input4, &input_pos5);

	if(assert_equals_int(expected1, actual1))
		return 1;
	if(assert_equals_int(expected2, actual2))
		return 1;
	if(assert_equals_int(expected3, actual3))
		return 1;
	if(assert_equals_int(expected4, actual4))
		return 1;
	if(assert_equals_int(expected5, actual5))
		return 1;
	
	if(assert_equals_unsigned_int(expected_pos2, input_pos2))
		return 1;
	if(assert_equals_unsigned_int(expected_pos3, input_pos3))
		return 1;
	if(assert_equals_unsigned_int(expected_pos4, input_pos4))
		return 1;
	if(assert_equals_unsigned_int(expected_pos5, input_pos5))
		return 1;
	
	return 0;
}

int unescape_char_test() {
	print_test_info("unescape_char_test()");
	char comment = '#';
	char equals = '=';
	
	char input1 = 'a';
	char input2 = ' ';
	char input3 = 'x';
	char input4 = 'd';
	char input5 = comment;
	char input6 = equals;
	char input7 = '\t';

	char expected1 = '\a';
	char expected2 = '\n';
	char expected3 = 'x';
	char expected4 = -1;
	char expected5 = comment;
	char expected6 = equals;
	char expected7 = '\n';

	char actual1 = unescape_char(input1, comment, equals);
	char actual2 = unescape_char(input2, comment, equals);
	char actual3 = unescape_char(input3, comment, equals);
	char actual4 = unescape_char(input4, comment, equals);
	char actual5 = unescape_char(input5, comment, equals);
	char actual6 = unescape_char(input6, comment, equals);
	char actual7 = unescape_char(input7, comment, equals);

	if(assert_equals_int(expected1, actual1))
		return 1;
	if(assert_equals_int(expected2, actual2))
		return 1;
	if(assert_equals_int(expected3, actual3))
		return 1;
	if(assert_equals_int(expected4, actual4))
		return 1;
	if(assert_equals_int(expected5, actual5))
		return 1;
	if(assert_equals_int(expected6, actual6))
		return 1;
	if(assert_equals_int(expected7, actual7))
		return 1;
	
	return 0;
}

int stripws_test() {
	print_test_info("stripws_test()");
	char input1[12] = " asdf ";
	char input2[12] = "asdf ";
	char input3[12] = " asdf";
	char input4[12] = " as df";
	char input5[12] = "   ";

	char* expected1 = "asdf";
	char* expected4 = "as df";
	char* expected5 = "";

	char* actual1 = stripws(input1);
	char* actual2 = stripws(input2);
	char* actual3 = stripws(input3);
	char* actual4 = stripws(input4);
	char* actual5 = stripws(input5);

	if(assert_equals_str(expected1, actual1))
		return 1;
	if(assert_equals_str(expected1, actual2))
		return 1;
	if(assert_equals_str(expected1, actual3))
		return 1;
	if(assert_equals_str(expected4, actual4))
		return 1;
	if(assert_equals_str(expected5, actual5))
		return 1;
	
	return 0;
}

int unescape_string_test() {
	print_test_info("unescape_string_test()");
	char comment = '#';
	char equals = '=';

	char input_src1[20] = "asdf";
	char input_src2[20] = "asdf\\n";
	char input_src3[20] = "dd \\x000030";
	char input_src4[20] = "dd \\ ";
	char input_src5[20] = "dd \\ ";
	char input_src6[20] = "dd \\b";
	char input_src7[20] = "dd \\x303";
	char input_src8[20] = "dd \\xFFFFFF";
	char input_src9[20] = "dd \\#";
	char input_src10[20] = "dd #";
	char input_src11[20] = "dd #d";
	char input_src12[20] = "";
	char input_src13[20] = "asdf";
	char input_src14[20] = "dd \\\t";
	char input_src15[20] = "dd \\\td";
	char input_src16[20] = "dd \\\t\rd";

	size_t len1 = strlen(input_src1) + 1;
	size_t len2 = strlen(input_src2) + 1;
	size_t len3 = strlen(input_src3) + 1;
	size_t len4 = strlen(input_src4) + 1;
	size_t len5 = 0;
	size_t len6 = 4;
	size_t len7 = 5;
	size_t len8 = strlen(input_src8) + 1;
	size_t len9 = strlen(input_src9) + 1;
	size_t len10 = strlen(input_src10) + 1;
	size_t len11 = strlen(input_src11) + 1;
	size_t len12 = 20;
	size_t len13 = 2;
	size_t len14 = strlen(input_src14) + 1;
	size_t len15 = strlen(input_src15) + 1;
	size_t len16 = strlen(input_src16) + 1;

	int expected1 = 1;
	int expected2 = 1;
	int expected3 = 1;
	int expected4 = 1;
	int expected5 = 1;
	int expected6 = 0;
	int expected7 = 0;
	int expected8 = 0;
	int expected9 = 1;
	int expected10 = 0;
	int expected11 = 0;
	int expected12 = 1;
	int expected13 = 1;
	int expected14 = 1;
	int expected15 = 0;
	int expected16 = 1;
	
	char* expected_inp1 = "asdf";
	char* expected_inp2 = "asdf\n";
	char* expected_inp3 = "dd 0";
	char* expected_inp4 = "dd \n";
	char* expected_inp5 = "";
	char* expected_inp9 = "dd #";
	char* expected_inp12 = "";
	char* expected_inp13 = "as";
	char* expected_inp14 = "dd \n";
	char* expected_inp16 = "dd \nd";

	int actual1 = unescape_string(input_src1, len1, comment, equals);
	int actual2 = unescape_string(input_src2, len2, comment, equals);
	int actual3 = unescape_string(input_src3, len3, comment, equals);
	int actual4 = unescape_string(input_src4, len4, comment, equals);
	int actual5 = unescape_string(input_src5, len5, comment, equals);
	int actual6 = unescape_string(input_src6, len6, comment, equals);
	int actual7 = unescape_string(input_src7, len7, comment, equals);
	int actual8 = unescape_string(input_src8, len8, comment, equals);
	int actual9 = unescape_string(input_src9, len9, comment, equals);
	int actual10 = unescape_string(input_src10, len10, comment, equals);
	int actual11 = unescape_string(input_src11, len11, comment, equals);
	int actual12 = unescape_string(input_src12, len12, comment, equals);
	int actual13 = unescape_string(input_src13, len13, comment, equals);
	int actual14 = unescape_string(input_src14, len14, comment, equals);
	int actual15 = unescape_string(input_src15, len15, comment, equals);
	int actual16 = unescape_string(input_src16, len16, comment, equals);

	if(assert_equals_int(expected1, actual1))
		return 1;
	if(assert_equals_int(expected2, actual2))
		return 1;
	if(assert_equals_int(expected3, actual3))
		return 1;
	if(assert_equals_int(expected4, actual4))
		return 1;
	if(assert_equals_int(expected5, actual5))
		return 1;
	if(assert_equals_int(expected6, actual6))
		return 1;
	if(assert_equals_int(expected7, actual7))
		return 1;
	if(assert_equals_int(expected8, actual8))
		return 1;
	if(assert_equals_int(expected9, actual9))
		return 1;
	if(assert_equals_int(expected10, actual10))
		return 1;
	if(assert_equals_int(expected11, actual11))
		return 1;
	if(assert_equals_int(expected12, actual12))
		return 1;
	if(assert_equals_int(expected13, actual13))
		return 1;
	if(assert_equals_int(expected14, actual14))
		return 1;
	if(assert_equals_int(expected15, actual15))
		return 1;
	if(assert_equals_int(expected16, actual16))
		return 1;

	if(assert_equals_str(expected_inp1, input_src1))
		return 1;
	if(assert_equals_str(expected_inp2, input_src2))
		return 1;
	if(assert_equals_str(expected_inp3, input_src3))
		return 1;
	if(assert_equals_str(expected_inp4, input_src4))
		return 1;
	if(assert_equals_str(expected_inp5, input_src5))
		return 1;
	//input_src6 doesn't need to be tested because of undefined behavior
	//input_src7 doesn't need to be tested because of undefined behavior
	//input_src8 doesn't need to be tested because of undefined behavior
	if(assert_equals_str(expected_inp9, input_src9))
		return 1;
	//input_src10 doesn't need to be tested because of undefined behavior
	//input_src11 doesn't need to be tested because of undefined behavior
	if(assert_equals_str(expected_inp12, input_src12))
		return 1;
	if(assert_equals_str(expected_inp13, input_src13))
		return 1;
	if(assert_equals_str(expected_inp14, input_src14))
		return 1;
	//input_src15 doesn't need to be tested because of undefined behavior
	if(assert_equals_str(expected_inp16, input_src16))
		return 1;
	
	return 0;
}

int fsize_test() {
	print_test_info("fsize_test()");

	FILE* str1 = fopen("./testfile_empty", "r");
	FILE* str2 = fopen("testfile", "r");

	if(!str1 || !str2) {
		fprintf(stderr, "FATAL ERROR: Couldn't open test files!\n");
		exit(EXIT_FAILURE);
	}

	int error1 = 0;
	int error2 = 0;
	
	size_t expected1 = 0;
	size_t expected2 = 8;

	int expected_error = 0;

	char expected_char1 = 't';
	char expected_char2 = 'e';

	size_t actual1 = fsize(str1, &error1);
	size_t actual2 = fsize(str2, &error2);

	//check errors
	if(assert_equals_int(expected_error, error1))
		return 1;
	if(assert_equals_int(expected_error, error2))
		return 1;

	//check sizes
	if(assert_equals_unsigned_int(expected1, actual1))
		return 1;
	if(assert_equals_unsigned_int(expected2, actual2))
		return 1;

	//check position
	if(assert_equals_int(expected_char1, fgetc(str2)))
		return 1;
	if(assert_equals_int(expected_char2, fgetc(str2)))
		return 1;
	
	return 0;
}

int count_newlines_test() {
	print_test_info("count_newlines_test()");
	char* input1 = "";
	char* input2 = "f\n";
	char* input3 = "\n\r";
	char* input4 = "\r\n";
	char* input5 = "\n\n\r";
	char* input6 = " \n";

	size_t input_pos2 = 0;
	size_t input_pos4 = 0;
	size_t input_pos5 = 0;
	size_t input_pos6 = 1;

	size_t expected1 = 0;
	size_t expected2 = 0;
	size_t expected3 = 1;
	size_t expected4 = 1;
	size_t expected5 = 2;
	size_t expected6 = 1;

	size_t expected_pos2 = 0;
	size_t expected_pos4 = 2;
	size_t expected_pos5 = 3;
	size_t expected_pos6 = 2;

	size_t actual1 = count_newlines(input1, NULL);
	size_t actual2 = count_newlines(input2, &input_pos2);
	size_t actual3 = count_newlines(input3, NULL);
	size_t actual4 = count_newlines(input4, &input_pos4);
	size_t actual5 = count_newlines(input5, &input_pos5);
	size_t actual6 = count_newlines(input6, &input_pos6);

	if(assert_equals_unsigned_int(expected1, actual1))
		return 1;
	if(assert_equals_unsigned_int(expected2, actual2))
		return 1;
	if(assert_equals_unsigned_int(expected3, actual3))
		return 1;
	if(assert_equals_unsigned_int(expected4, actual4))
		return 1;
	if(assert_equals_unsigned_int(expected5, actual5))
		return 1;
	if(assert_equals_unsigned_int(expected6, actual6))
		return 1;

	if(assert_equals_unsigned_int(expected_pos2, input_pos2))
		return 1;
	if(assert_equals_unsigned_int(expected_pos4, input_pos4))
		return 1;
	if(assert_equals_unsigned_int(expected_pos5, input_pos5))
		return 1;
	if(assert_equals_unsigned_int(expected_pos6, input_pos6))
		return 1;
	
	return 0;
}

int skip_whitespaces_test() {
	print_test_info("skip_whitespaces_test()");
	char* input1 = "";
	char* input2 = "s";
	char* input3 = " s";
	char* input4 = " s ";
	char* input5 = "\n \r\n ";
	char* input6 = "asdf  \t ";
	char* input7 = " ";

	size_t input_pos1 = 0;
	size_t input_pos2 = 0;
	size_t input_pos3 = 0;
	size_t input_pos4 = 0;
	size_t input_pos5 = 0;
	size_t input_pos6 = 4;

	unsigned int input_ln4 = 0;
	unsigned int input_ln5 = 3;

	size_t expected1 = 0;
	size_t expected2 = 0;
	size_t expected3 = 1;
	size_t expected4 = 1;
	size_t expected5 = 5;
	size_t expected6 = 4;
	size_t expected7 = 1;

	size_t expected_pos1 = 0;
	size_t expected_pos2 = 0;
	size_t expected_pos3 = 1;
	size_t expected_pos4 = 1;
	size_t expected_pos5 = 5;
	size_t expected_pos6 = 8;

    unsigned int expected_ln4 = 0;
    unsigned int expected_ln5 = 5;

	size_t actual1 = skip_whitespaces(input1, &input_pos1, NULL);
	size_t actual2 = skip_whitespaces(input2, &input_pos2, NULL);
	size_t actual3 = skip_whitespaces(input3, &input_pos3, NULL);
	size_t actual4 = skip_whitespaces(input4, &input_pos4, &input_ln4);
	size_t actual5 = skip_whitespaces(input5, &input_pos5, &input_ln5);
	size_t actual6 = skip_whitespaces(input6, &input_pos6, NULL);
	size_t actual7 = skip_whitespaces(input7, NULL, NULL);

	//check output
	if(assert_equals_unsigned_int(expected1, actual1))
		return 1;
	if(assert_equals_unsigned_int(expected2, actual2))
		return 1;
	if(assert_equals_unsigned_int(expected3, actual3))
		return 1;
	if(assert_equals_unsigned_int(expected4, actual4))
		return 1;
	if(assert_equals_unsigned_int(expected5, actual5))
		return 1;
	if(assert_equals_unsigned_int(expected6, actual6))
		return 1;
	if(assert_equals_unsigned_int(expected7, actual7))
		return 1;

	//check positions
	if(assert_equals_unsigned_int(expected_pos1, input_pos1))
		return 1;
	if(assert_equals_unsigned_int(expected_pos2, input_pos2))
		return 1;
	if(assert_equals_unsigned_int(expected_pos3, input_pos3))
		return 1;
	if(assert_equals_unsigned_int(expected_pos4, input_pos4))
		return 1;
	if(assert_equals_unsigned_int(expected_pos5, input_pos5))
		return 1;
	if(assert_equals_unsigned_int(expected_pos6, input_pos6))
		return 1;

	//check linenumbers
	if(assert_equals_unsigned_int(expected_ln4, input_ln4))
		return 1;
	if(assert_equals_unsigned_int(expected_ln5, input_ln5))
		return 1;
	
	return 0;
}

int skip_comment_test() {
	print_test_info("skip_comment_test()");
	char* input1 = "";
	char* input2 = "asdf";
	char* input3 = "1234";
	char* input4 = "1234\n";
	char* input5 = "12\n34";
	char* input6 = "12\n 34";

	size_t input_pos1 = 0;
	size_t input_pos2 = 0;
	size_t input_pos3 = 2;
	size_t input_pos4 = 4;
	size_t input_pos5 = 0;

	size_t expected1 = 0;
	size_t expected2 = 4;
	size_t expected3 = 2;
	size_t expected4 = 0;
	size_t expected5 = 2;
	size_t expected6 = 2;

	size_t expected_pos1 = 0;
	size_t expected_pos2 = 4;
	size_t expected_pos3 = 4;
	size_t expected_pos4 = 4;
	size_t expected_pos5 = 2;

	size_t actual1 = skip_comment(input1, &input_pos1);
	size_t actual2 = skip_comment(input2, &input_pos2);
	size_t actual3 = skip_comment(input3, &input_pos3);
	size_t actual4 = skip_comment(input4, &input_pos4);
	size_t actual5 = skip_comment(input5, &input_pos5);
	size_t actual6 = skip_comment(input6, NULL);

	if(assert_equals_unsigned_int(expected1, actual1))
		return 1;
	if(assert_equals_unsigned_int(expected2, actual2))
		return 1;
	if(assert_equals_unsigned_int(expected3, actual3))
		return 1;
	if(assert_equals_unsigned_int(expected4, actual4))
		return 1;
	if(assert_equals_unsigned_int(expected5, actual5))
		return 1;
	if(assert_equals_unsigned_int(expected6, actual6))
		return 1;

	if(assert_equals_unsigned_int(expected_pos1, input_pos1))
		return 1;
	if(assert_equals_unsigned_int(expected_pos2, input_pos2))
		return 1;
	if(assert_equals_unsigned_int(expected_pos3, input_pos3))
		return 1;
	if(assert_equals_unsigned_int(expected_pos4, input_pos4))
		return 1;
	if(assert_equals_unsigned_int(expected_pos5, input_pos5))
		return 1;
	
	return 0;
}

int contains_utf8_test() {
	print_test_info("contains_utf_8_test()");
	char* input1 = "";
	char* input2 = "asdf";
	char* input3 = "ä";
	char* input4 = "asäfa";

	int expected1 = 0;
	int expected2 = 0;
	int expected3 = 1;
	int expected4 = 1;

	int actual1 = contains_utf8(input1);
	int actual2 = contains_utf8(input2);
	int actual3 = contains_utf8(input3);
	int actual4 = contains_utf8(input4);

	if(assert_equals_int(expected1, actual1))
		return 1;
	if(assert_equals_int(expected2, actual2))
		return 1;
	if(assert_equals_int(expected3, actual3))
		return 1;
	if(assert_equals_int(expected4, actual4))
		return 1;
	
	return 0;
}

int contains_escape_characters_test() {
	print_test_info("contains_escape_characters_test()");
	char comment = '#';
	char equals = '=';

	char* input1 = "";
	char* input2 = "asdf";
	char* input3 = "asdf\n";
	char* input4 = "asdf#dd";
	char* input5 = "asdf dd";

	int expected1 = 0;
	int expected2 = 0;
	int expected3 = 1;
	int expected4 = 1;
	int expected5 = 1;

	int actual1 = contains_escape_characters(input1, comment, equals);
	int actual2 = contains_escape_characters(input2, comment, equals);
	int actual3 = contains_escape_characters(input3, comment, equals);
	int actual4 = contains_escape_characters(input4, comment, equals);
	int actual5 = contains_escape_characters(input5, comment, equals);
	
	if(assert_equals_int(expected1, actual1))
		return 1;
	if(assert_equals_int(expected2, actual2))
		return 1;
	if(assert_equals_int(expected3, actual3))
		return 1;
	if(assert_equals_int(expected4, actual4))
		return 1;
	if(assert_equals_int(expected5, actual5))
		return 1;
	
	return 0;
}

int read_name_test() {
	print_test_info("read_name_test()");
	char comment = '#';
	char equals = '=';
	
	char input1[20] = "";
	char input2[20] = " asdf ";
	char input3[20] = "asdf";
	char input4[20] = "as]df";
	char input5[20] = "säaf";
	char input6[20] = "ääasdf";
	char input7[20] = "1asdf2";

	char input_name1[20];
	char input_name2[20];
	char input_name3[20];
	char input_name4[20];
	char input_name5[20];
	char input_name6[20];
	char input_name7[20];

	size_t start1 = 0;
	size_t start2 = 0;
	size_t start3 = 0;
	size_t start4 = 0;
	size_t start5 = 0;
	size_t start6 = 4;
	size_t start7 = 1;

	size_t end1 = 0;
	size_t end2 = strlen(input2);
	size_t end3 = strlen(input3) + 2;
	size_t end4 = strlen(input4);
	size_t end5 = strlen(input5);
	size_t end6 = strlen(input6);
	size_t end7 = 5;

	int allow_utf8_1 = 1;
	int allow_utf8_5 = 0;

	int actual1 = read_name(input_name1, input1, start1, end1, comment, equals, allow_utf8_1);
	int actual2 = read_name(input_name2, input2, start2, end2, comment, equals, allow_utf8_1);
	int actual3 = read_name(input_name3, input3, start3, end3, comment, equals, allow_utf8_1);
	int actual4 = read_name(input_name4, input4, start4, end4, comment, equals, allow_utf8_1);
	int actual5 = read_name(input_name5, input5, start5, end5, comment, equals, allow_utf8_5);
	int actual6 = read_name(input_name6, input6, start6, end6, comment, equals, allow_utf8_1);
	int actual7 = read_name(input_name7, input7, start7, end7, comment, equals, allow_utf8_1);

	int expected1 = 1;
	int expected2 = 1;
	int expected3 = 1;
	int expected4 = 0;
	int expected5 = 0;
	int expected6 = 1;
	int expected7 = 1;
	
	char* expected_name1 = "";
	char* expected_name2 = "asdf";
	char* expected_name3 = "asdf";
	//expected_name4 and expected_name5 not needed because the output is undefined
	char* expected_name6 = "asdf";
	char* expected_name7 = "asdf";

	if(assert_equals_int(expected1, actual1))
		return 1;
	if(assert_equals_int(expected2, actual2))
		return 1;
	if(assert_equals_int(expected3, actual3))
		return 1;
	if(assert_equals_int(expected4, actual4))
		return 1;
	if(assert_equals_int(expected5, actual5))
		return 1;
	if(assert_equals_int(expected6, actual6))
		return 1;
	if(assert_equals_int(expected7, actual7))
		return 1;

	if(assert_equals_str(expected_name1, input_name1))
		return 1;
	if(assert_equals_str(expected_name2, input_name2))
		return 1;
	if(assert_equals_str(expected_name3, input_name3))
		return 1;
	if(assert_equals_str(expected_name6, input_name6))
		return 1;
	if(assert_equals_str(expected_name7, input_name7))
		return 1;
	
	return 0;
}

int remove_quotes_test() {
	print_test_info("remove_quotes_test()");
	char input1[20] = "";
	char input2[20] = "asdf";
	char input3[20] = "\" asdf \"";
	char input4[20] = "a";
	char input5[20] = "\"";
	char input6[20] = "\'";
	char input7[20] = "\' asdf \'";
	char input8[20] = "\" asdf \'";

	int expected1 = 1;
	int expected2 = 1;
	int expected3 = 1;
	int expected4 = 1;
	int expected5 = 0;
	int expected6 = 0;
	int expected7 = 1;
	int expected8 = 0;
	
	char* expected_inp1 = "";
	char* expected_inp2 = "asdf";
	char* expected_inp3 = " asdf ";
	char* expected_inp4 = "a";
	char* expected_inp7 = " asdf ";
	
	int actual1 = remove_quotes(input1);
	int actual2 = remove_quotes(input2);
	int actual3 = remove_quotes(input3);
	int actual4 = remove_quotes(input4);
	int actual5 = remove_quotes(input5);
	int actual6 = remove_quotes(input6);
	int actual7 = remove_quotes(input7);
	int actual8 = remove_quotes(input8);

	if(assert_equals_int(expected1, actual1))
		return 1;
	if(assert_equals_int(expected2, actual2))
		return 1;
	if(assert_equals_int(expected3, actual3))
		return 1;
	if(assert_equals_int(expected4, actual4))
		return 1;
	if(assert_equals_int(expected5, actual5))
		return 1;
	if(assert_equals_int(expected6, actual6))
		return 1;
	if(assert_equals_int(expected7, actual7))
		return 1;
	if(assert_equals_int(expected8, actual8))
		return 1;

	if(assert_equals_str(expected_inp1, input1))
		return 1;
	if(assert_equals_str(expected_inp2, input2))
		return 1;
	if(assert_equals_str(expected_inp3, input3))
		return 1;
	if(assert_equals_str(expected_inp4, input4))
		return 1;
	if(assert_equals_str(expected_inp7, input7))
		return 1;
	
	return 0;
}

int read_value_test() {
	print_test_info("read_value_test()");
	char comment = '#';
	char equals = '=';

	char* str1 = "";
	char* str2 = "asdf";
	char* str3 = "asdf \\\nasdf";
	char* str4 = "asdf \\\nasdf\\\nasdf";
	char* str5 = " asdf ";
	char* str6 = "\" asdf \"";
	char* str7 = "\' asdf \"";
	char* str8 = "\' \\x0030Df\'";
	char* str9 = " \\n";
	char* str10 = " asdf\\nasdf";
	char* str11 = "\\x0030df";
	char* str12 = "as]df";
	char* str13 = "as\\\\ndf";

	char value1[20];
	char value2[20];
	char value3[20];
	char value4[20];
	char value5[20];
	char value6[20];
	char value7[20];
	char value8[20];
	char value9[20];
	char value10[20];
	char value11[20];
	char value12[20];
	char value13[20];

	size_t start1 = 0;
	size_t start2 = 0;
	size_t start3 = 0;
	size_t start4 = 0;
	size_t start5 = 0;
	size_t start6 = 0;
	size_t start7 = 0;
	size_t start8 = 0;
	size_t start9 = 0;
	size_t start10 = 2;
	size_t start11 = 0;
	size_t start12 = 0;
	size_t start13 = 0;

	size_t end1 = 0;
	size_t end2 = strlen(str2);
	size_t end3 = strlen(str3);
	size_t end4 = strlen(str4);
	size_t end5 = strlen(str5);
	size_t end6 = strlen(str6);
	size_t end7 = strlen(str7);
	size_t end8 = strlen(str8);
	size_t end9 = strlen(str9);
	size_t end10 = strlen(str10) - 2;
	size_t end11 = 9;
	size_t end12 = strlen(str12);
	size_t end13 = strlen(str13);

	unsigned int ln1 = 0;
	unsigned int ln2 = 2;
	unsigned int ln4 = 3;
	unsigned int ln5 = 0;
	unsigned int ln6 = 0;
	unsigned int ln7 = 0;
	unsigned int ln8 = 0;
	unsigned int ln9 = 0;
	unsigned int ln10 = 0;
	unsigned int ln11 = 0;
	unsigned int ln12 = 0;
	unsigned int ln13 = 0;

	int allow_utf8_1 = 1;
	int allow_utf8_11 = 0;

	int expected1 = 1;
	int expected2 = 1;
	int expected3 = 1;
	int expected4 = 1;
	int expected5 = 1;
	int expected6 = 1;
	int expected7 = 0;
	int expected8 = 1;
	int expected9 = 1;
	int expected10 = 1;
	int expected11 = 0;
	int expected12 = 0;
	int expected13 = 1;
	
	char* expected_value1 = "";
	char* expected_value2 = "asdf";
	char* expected_value3 = "asdf \nasdf";
	char* expected_value4 = "asdf \nasdf\nasdf";
	char* expected_value5 = "asdf";
	char* expected_value6 = " asdf ";
	char* expected_value8 = " ミ";
	char* expected_value9 = "\n";
	char* expected_value10 = "sdf\nas";
	char* expected_value13 = "as\\ndf";

	unsigned int expected_ln1 = 0;
	unsigned int expected_ln2 = 2;
	unsigned int expected_ln4 = 5;
	unsigned int expected_ln5 = 0;
	unsigned int expected_ln6 = 0;
	unsigned int expected_ln7 = 0;
	unsigned int expected_ln8 = 0;
	unsigned int expected_ln9 = 0;
	unsigned int expected_ln10 = 0;
	unsigned int expected_ln11 = 0;
	unsigned int expected_ln12 = 0;
	unsigned int expected_ln13 = 0;

	int actual1 = read_value(value1, str1, start1, end1, comment, equals, &ln1, allow_utf8_1);
	int actual2 = read_value(value2, str2, start2, end2, comment, equals, &ln2, allow_utf8_1);
	int actual3 = read_value(value3, str3, start3, end3, comment, equals, NULL, allow_utf8_1);
	int actual4 = read_value(value4, str4, start4, end4, comment, equals, &ln4, allow_utf8_1);
	int actual5 = read_value(value5, str5, start5, end5, comment, equals, &ln5, allow_utf8_1);
	int actual6 = read_value(value6, str6, start6, end6, comment, equals, &ln6, allow_utf8_1);
	int actual7 = read_value(value7, str7, start7, end7, comment, equals, &ln7, allow_utf8_1);
	int actual8 = read_value(value8, str8, start8, end8, comment, equals, &ln8, allow_utf8_1);
	int actual9 = read_value(value9, str9, start9, end9, comment, equals, &ln9, allow_utf8_1);
	int actual10 = read_value(value10, str10, start10, end10, comment, equals, &ln10, allow_utf8_1);
	int actual11 = read_value(value11, str11, start11, end11, comment, equals, &ln11, allow_utf8_11);
	int actual12 = read_value(value12, str12, start12, end12, comment, equals, &ln12, allow_utf8_11);
	int actual13 = read_value(value13, str13, start13, end13, comment, equals, &ln13, allow_utf8_11);
	
	if(assert_equals_int(expected1, actual1))
		return 1;
	if(assert_equals_int(expected2, actual2))
		return 1;
	if(assert_equals_int(expected3, actual3))
		return 1;
	if(assert_equals_int(expected4, actual4))
		return 1;
	if(assert_equals_int(expected5, actual5))
		return 1;
	if(assert_equals_int(expected6, actual6))
		return 1;
	if(assert_equals_int(expected7, actual7))
		return 1;
	if(assert_equals_int(expected8, actual8))
		return 1;
	if(assert_equals_int(expected9, actual9))
		return 1;
	if(assert_equals_int(expected10, actual10))
		return 1;
	if(assert_equals_int(expected11, actual11))
		return 1;
	if(assert_equals_int(expected12, actual12))
		return 1;
	if(assert_equals_int(expected13, actual13))
		return 1;

	if(assert_equals_str(expected_value1, value1))
		return 1;
	if(assert_equals_str(expected_value2, value2))
		return 1;
	if(assert_equals_str(expected_value3, value3))
		return 1;
	if(assert_equals_str(expected_value4, value4))
		return 1;
	if(assert_equals_str(expected_value5, value5))
		return 1;
	if(assert_equals_str(expected_value6, value6))
		return 1;
	if(assert_equals_str(expected_value8, value8))
		return 1;
	if(assert_equals_str(expected_value9, value9))
		return 1;
	if(assert_equals_str(expected_value10, value10))
		return 1;
	if(assert_equals_str(expected_value13, value13))
		return 1;
	
	if(assert_equals_unsigned_int(expected_ln1, ln1))
		return 1;
	if(assert_equals_unsigned_int(expected_ln2, ln2))
		return 1;
	if(assert_equals_unsigned_int(expected_ln4, ln4))
		return 1;
	if(assert_equals_unsigned_int(expected_ln5, ln5))
		return 1;
	if(assert_equals_unsigned_int(expected_ln6, ln6))
		return 1;
	if(assert_equals_unsigned_int(expected_ln7, ln7))
		return 1;
	if(assert_equals_unsigned_int(expected_ln8, ln8))
		return 1;
	if(assert_equals_unsigned_int(expected_ln9, ln9))
		return 1;
	if(assert_equals_unsigned_int(expected_ln10, ln10))
		return 1;
	if(assert_equals_unsigned_int(expected_ln11, ln11))
		return 1;
	if(assert_equals_unsigned_int(expected_ln12, ln12))
		return 1;
	if(assert_equals_unsigned_int(expected_ln13, ln13))
		return 1;
	
	return 0;
}

int ini_read_test() {
	print_test_info("ini_read_test()");
	char comment = '#';
	char equals = ':';

	struct data_structure ds1;
	struct data_structure ds2;
	struct data_structure ds3;
	struct data_structure ds13;

	ds1.index = 0;
	ds1.str[0] = '\0';
	
	ds2.index = 0;
	ds2.str[0] = '\0';
	
	ds3.index = 0;
	ds3.str[0] = '\0';
	
	ds13.index = 0;
	ds13.str[0] = '\0';
	
	char input1[1000];
	char input2[1000];
	char input3[1000];
	char input4[1000];
	char input5[1000];
	char input6[1000];
	char input7[1000];
	char input8[1000];
	char input9[1000];
	char input10[1000];
	char input11[1000];
	char input12[1000];
	char input13[1000];
	char input14[1000];
	char input15[1000];
	char input16[1000];
	char input17[1000];

	size_t len1 = load_file(input1, "ini_read_test/input1.txt");
	size_t len2 = load_file(input2, "ini_read_test/input2.txt");
	size_t len3 = load_file(input3, "ini_read_test/input3.txt");
	size_t len4 = load_file(input4, "ini_read_test/input4.txt");
	size_t len5 = load_file(input5, "ini_read_test/input5.txt");
	size_t len6 = load_file(input6, "ini_read_test/input6.txt");
	size_t len7 = load_file(input7, "ini_read_test/input7.txt");
	size_t len8 = load_file(input8, "ini_read_test/input8.txt");
	size_t len9 = load_file(input9, "ini_read_test/input9.txt");
	size_t len10 = load_file(input10, "ini_read_test/input10.txt");
	size_t len11 = load_file(input11, "ini_read_test/input11.txt");
	size_t len12 = load_file(input12, "ini_read_test/input12.txt");
	size_t len13 = load_file(input13, "ini_read_test/input13.txt");
	size_t len14 = load_file(input14, "ini_read_test/input14.txt");
	size_t len15 = load_file(input15, "ini_read_test/input15.txt");
	size_t len16 = load_file(input16, "ini_read_test/input16.txt");
	size_t len17 = load_file(input17, "ini_read_test/input17.txt");

	int allow_utf8_1 = 1;
	int allow_utf8_2 = 1;
	int allow_utf8_3 = 0;
	int allow_utf8_4 = 0;
	int allow_utf8_5 = 1;
	int allow_utf8_6 = 1;
	int allow_utf8_7 = 1;
	int allow_utf8_8 = 1;
	int allow_utf8_9 = 1;
	int allow_utf8_10 = 1;
	int allow_utf8_11 = 1;
	int allow_utf8_12 = 1;
	int allow_utf8_13 = 1;
	int allow_utf8_14 = 1;
	int allow_utf8_15 = 1;
	int allow_utf8_16 = 1;
	int allow_utf8_17 = 1;

	int expected1 = -1;
	int expected2 = 0;
	int expected3 = 0;
	int expected4 = 3;
	int expected5 = 12;
	int expected6 = 12;
	int expected7 = 9;
	int expected8 = 5;
	int expected9 = 5;
	int expected10 = 5;
	int expected11 = 12;
	int expected12 = 12;
	int expected13 = 0;
	int expected14 = 9;
	int expected15 = 9;
	int expected16 = 1;
	int expected17 = -1;
	
	char expected_buffer1[1000];
	char expected_buffer2[1000];
	char expected_buffer3[1000];
	char expected_buffer13[1000];

	size_t expected_len1 = load_file(expected_buffer1, "ini_read_test/expected1.txt");
	size_t expected_len2 = load_file(expected_buffer2, "ini_read_test/expected2.txt");
	size_t expected_len3 = load_file(expected_buffer3, "ini_read_test/expected3.txt");
	size_t expected_len13 = load_file(expected_buffer13, "ini_read_test/expected13.txt");

	int actual1 = ini_read(input1, len1, callback, &ds1, comment, equals, allow_utf8_1);
	int actual2 = ini_read(input2, len2, callback, &ds2, comment, equals, allow_utf8_2);
	int actual3 = ini_read(input3, len3, callback, &ds3, comment, equals, allow_utf8_3);
	int actual4 = ini_read(input4, len4, callback_empty, NULL, comment, equals, allow_utf8_4);
	int actual5 = ini_read(input5, len5, callback_empty, NULL, comment, equals, allow_utf8_5);
	int actual6 = ini_read(input6, len6, callback_empty, NULL, comment, equals, allow_utf8_6);
	int actual7 = ini_read(input7, len7, callback_empty, NULL, comment, equals, allow_utf8_7);
	int actual8 = ini_read(input8, len8, callback_empty, NULL, comment, equals, allow_utf8_8);
	int actual9 = ini_read(input9, len9, callback_empty, NULL, comment, equals, allow_utf8_9);
	int actual10 = ini_read(input10, len10, callback_empty, NULL, comment, equals, allow_utf8_10);
	int actual11 = ini_read(input11, len11, callback_empty, NULL, comment, equals, allow_utf8_11);
	int actual12 = ini_read(input12, len12, callback_empty, NULL, comment, equals, allow_utf8_12);
	int actual13 = ini_read(input13, len13, callback, &ds13, comment, equals, allow_utf8_13);
	int actual14 = ini_read(input14, len14, callback_empty, NULL, comment, equals, allow_utf8_14);
	int actual15 = ini_read(input15, len15, callback_empty, NULL, comment, equals, allow_utf8_15);
	int actual16 = ini_read(input16, len16, callback_fail, NULL, comment, equals, allow_utf8_16);
	int actual17 = ini_read(input17, len17, callback_empty, NULL, comment, ']', allow_utf8_17);
	
	if(assert_equals_int(expected1, actual1))
		return 1;
	if(assert_equals_int(expected2, actual2))
		return 1;
	if(assert_equals_int(expected3, actual3))
		return 1;
	if(assert_equals_int(expected4, actual4))
		return 1;
	if(assert_equals_int(expected5, actual5))
		return 1;
	if(assert_equals_int(expected6, actual6))
		return 1;
	if(assert_equals_int(expected7, actual7))
		return 1;
	if(assert_equals_int(expected8, actual8))
		return 1;
	if(assert_equals_int(expected9, actual9))
		return 1;
	if(assert_equals_int(expected10, actual10))
		return 1;
	if(assert_equals_int(expected11, actual11))
		return 1;
	if(assert_equals_int(expected12, actual12))
		return 1;
	if(assert_equals_int(expected13, actual13))
		return 1;
	if(assert_equals_int(expected14, actual14))
		return 1;
	if(assert_equals_int(expected15, actual15))
		return 1;
	if(assert_equals_int(expected16, actual16))
		return 1;
	if(assert_equals_int(expected17, actual17))
		return 1;

	if(assert_equals_unsigned_int(expected_len1, ds1.index))
		return 1;
	if(assert_equals_unsigned_int(expected_len2, ds2.index))
		return 1;
	if(assert_equals_unsigned_int(expected_len3, ds3.index))
		return 1;
	if(assert_equals_unsigned_int(expected_len13, ds13.index))
		return 1;

	if(assert_equals_str(expected_buffer1, ds1.str))
		return 1;
	if(assert_equals_str(expected_buffer2, ds2.str))
		return 1;
	if(assert_equals_str(expected_buffer3, ds3.str))
		return 1;
	if(assert_equals_str(expected_buffer13, ds13.str))
		return 1;
    
	return 0;
}

int check_for_BOM_test() {
	print_test_info("check_for_BOM_test()");
	FILE* input1 = fopen("check_for_BOM_test/input1.txt", "r");
	FILE* input2 = fopen("check_for_BOM_test/input2.txt", "r");
	FILE* input3 = fopen("check_for_BOM_test/input3.txt", "r");

	int expected1 = 0;
	int expected2 = 0;
	int expected3 = 1;

	int actual1 = check_for_BOM(input1);
	int actual2 = check_for_BOM(input2);
	int actual3 = check_for_BOM(input3);

	char expected_c1 = ' ';
	char expected_c2 = '1';
	char expected_c3 = '1';

	char actual_c1 = 32;
	char actual_c2 = 32;
	char actual_c3 = 32;

    fread(&actual_c1, 1, 1, input1);
    fread(&actual_c2, 1, 1, input2);
    fread(&actual_c3, 1, 1, input3);

	if(assert_equals_int(expected1, actual1))
		return 1;
	if(assert_equals_int(expected2, actual2))
		return 1;
	if(assert_equals_int(expected3, actual3))
		return 1;
	
	if(assert_equals_int(expected_c1, actual_c1))
		return 1;
	if(assert_equals_int(expected_c2, actual_c2))
		return 1;
	if(assert_equals_int(expected_c3, actual_c3))
		return 1;
	
	return 0;
}

int ini_read_file_test() {
	print_test_info("ini_read_file_test()");
	char comment = ';';
	char equals = '=';
	
	char* path1 = "";
	char* path2 = "ini_read_file_test/input2.ini";
	char* path3 = "ini_read_file_test/input3.ini";
	char* path4 = "ini_read_file_test/input4.ini";
	char* path5 = "ini_read_file_test/input5.ini";
	char* path6 = "ini_read_file_test/input6.ini";

	struct data_structure ds3;
	struct data_structure ds4;

	ds3.index = 0;
	ds3.str[0] = '\0';
	
	ds4.index = 0;
	ds4.str[0] = '\0';
	
	int allow_utf8_1 = INI_ALLOW_UTF8;
	int allow_utf8_2 = INI_ALLOW_UTF8;
	int allow_utf8_3 = INI_ALLOW_UTF8;
	int allow_utf8_4 = INI_ALLOW_UTF8_ON_BOM;
	int allow_utf8_5 = INI_FORBID_UTF8;
	int allow_utf8_6 = INI_FORBID_UTF8;

	int expected1 = -1;
	int expected2 = -1;
	int expected3 = 0;
	int expected4 = 0;
	int expected5 = 7;
	int expected6 = 7;

	char expected_buffer3[1000];
	char expected_buffer4[1000];
	
	int expected_len3 = load_file(expected_buffer3, "ini_read_file_test/expected3.txt");
	int expected_len4 = load_file(expected_buffer4, "ini_read_file_test/expected4.txt");

	int actual1 = ini_read_file(path1, callback_empty, NULL, comment, equals, allow_utf8_1);
	int actual2 = ini_read_file(path2, callback_empty, NULL, comment, equals, allow_utf8_2);
	int actual3 = ini_read_file(path3, callback, &ds3, comment, equals, allow_utf8_3);
	int actual4 = ini_read_file(path4, callback, &ds4, comment, equals, allow_utf8_4);
	int actual5 = ini_read_file(path5, callback_empty, NULL, comment, equals, allow_utf8_5);
	int actual6 = ini_read_file(path6, callback_empty, NULL, comment, equals, allow_utf8_6);

	
	if(assert_equals_int(expected1, actual1))
		return 1;
	if(assert_equals_int(expected2, actual2))
		return 1;
	if(assert_equals_int(expected3, actual3))
		return 1;
	if(assert_equals_int(expected4, actual4))
		return 1;
	if(assert_equals_int(expected5, actual5))
		return 1;
	if(assert_equals_int(expected6, actual6))
		return 1;

	if(assert_equals_unsigned_int(expected_len3, ds3.index))
		return 1;
	if(assert_equals_unsigned_int(expected_len4, ds4.index))
		return 1;

	if(assert_equals_str(expected_buffer3, ds3.str))
		return 1;
	if(assert_equals_str(expected_buffer4, ds4.str))
		return 1;	
	
	return 0;
}
