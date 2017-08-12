#include <fcntl.h>
#include <unistd.h>

#include "../include/ini_helper.h"
#include "../include/ini_parser.h"
#include "test.h"

size_t load_file(char buffer[1000], const char *path) {
  FILE *fd = fopen(path, "r");
  size_t len = fread(buffer, 1, 999, fd);
  fclose(fd);
  buffer[len] = '\0';
  return len;
}

void print_test_info(const char *test) {
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

  if (assert_equals_int(expected1, actual1))
    return 1;
  if (assert_equals_int(expected2, actual2))
    return 1;
  if (assert_equals_int(expected2, actual3))
    return 1;
  if (assert_equals_int(expected3, actual4))
    return 1;

  return 0;
}

int uctoutf8_test() {
  print_test_info("uctoutf8_test()");
  struct four_bytes expected1 = {{0x3b, 0, 0, 0}};
  struct four_bytes expected2 = {{0x8f, 0xce, 0, 0}};
  struct four_bytes expected3 = {{0x9e, 0xa6, 0xe0, 0}};
  struct four_bytes expected4 = {{0xa0, 0x80, 0xa0, 0xf3}};
  struct four_bytes expected5 = {{0xff, 0xff, 0xff, 0xff}};

  struct four_bytes input1 = {{0x3b, 0, 0, 0}};
  struct four_bytes input2 = {{0x8f, 3, 0, 0}};
  struct four_bytes input3 = {{0x9e, 0x09, 0, 0}};
  struct four_bytes input4 = {{0x20, 0, 0x0e, 0}};
  struct four_bytes input5 = {{0xef, 0xcd, 0xab, 0x13}};

  struct four_bytes actual1 = uctoutf8(input1);
  struct four_bytes actual2 = uctoutf8(input2);
  struct four_bytes actual3 = uctoutf8(input3);
  struct four_bytes actual4 = uctoutf8(input4);
  struct four_bytes actual5 = uctoutf8(input5);

  
  if (assert_equals_mem(expected1.bytes, actual1.bytes, 4))
    return 1;  
  if (assert_equals_mem(expected2.bytes, actual2.bytes, 4))
    return 1;
  if (assert_equals_mem(expected3.bytes, actual3.bytes, 4))
    return 1;
  if (assert_equals_mem(expected4.bytes, actual4.bytes, 4))
    return 1;
  if (assert_equals_mem(expected5.bytes, actual5.bytes, 4))
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

  if (assert_equals_int(expected1, actual1))
    return 1;
  if (assert_equals_int(expected1, actual2))
    return 1;
  if (assert_equals_int(expected2, actual3))
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

  if (assert_equals_int(expected1, actual1))
    return 1;
  if (assert_equals_int(expected1, actual2))
    return 1;
  if (assert_equals_int(expected2, actual3))
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

  if (assert_equals_int(expected1, actual1))
    return 1;
  if (assert_equals_int(expected1, actual2))
    return 1;
  if (assert_equals_int(expected2, actual3))
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

  if (assert_equals_int(expected1, actual1))
    return 1;
  if (assert_equals_int(expected1, actual2))
    return 1;
  if (assert_equals_int(expected1, actual3))
    return 1;
  if (assert_equals_int(expected1, actual4))
    return 1;
  if (assert_equals_int(expected2, actual5))
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

  if (assert_equals_int(expected1, actual1))
    return 1;
  if (assert_equals_int(expected2, actual2))
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

  if (assert_equals_int(expected1, actual1))
    return 1;
  if (assert_equals_int(expected1, actual2))
    return 1;
  if (assert_equals_int(expected1, actual3))
    return 1;
  if (assert_equals_int(expected2, actual4))
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

  if (assert_equals_unsigned_int(expected1, actual1))
    return 1;
  if (assert_equals_unsigned_int(expected2, actual2))
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

  if (assert_equals_unsigned_int(expected1, actual1))
    return 1;
  if (assert_equals_unsigned_int(expected2, actual2))
    return 1;
  if (assert_equals_unsigned_int(expected3, actual3))
    return 1;
  if (assert_equals_unsigned_int(expected4, actual4))
    return 1;

  return 0;
}

int read_unicode_from_string_test() {
  print_test_info("read_unicode_from_string_test()");
  struct four_bytes expected_uc1 = {{0x87, 0xa0, 0xe2, 0}};
  struct four_bytes expected_uc2 = {{0xFF, 0xFF, 0xFF, 0xFF}};
  struct four_bytes expected_uc6 = {{0xFF, 0xFF, 0xFF, 0xFF}};
  struct four_bytes expected_uc7 = {{0xAE, 0xA9, 0x90, 0xF0}};
  struct four_bytes expected_uc8 = {{0xFF, 0xFF, 0xFF, 0xFF}};
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
  char *input1 = "002807";
  char *input2 = "asdf002807";
  char *input3 = "asdf002807kk";
  char *input4 = "";
  char *input5 = "xyz";
  char *input6 = "12xy";
  char *input7 = "010A6E";
  char *input8 = "2";

  struct four_bytes actual0 = read_unicode_from_string(input1, NULL);
  struct four_bytes actual1 = read_unicode_from_string(input1, &input_pos1);
  struct four_bytes actual2 = read_unicode_from_string(input2, &input_pos2);
  struct four_bytes actual3 = read_unicode_from_string(input3, &input_pos3);
  struct four_bytes actual4 = read_unicode_from_string(input4, &input_pos4);
  struct four_bytes actual5 = read_unicode_from_string(input5, &input_pos5);
  struct four_bytes actual6 = read_unicode_from_string(input6, &input_pos6);
  struct four_bytes actual7 = read_unicode_from_string(input7, &input_pos7);
  struct four_bytes actual8 = read_unicode_from_string(input8, &input_pos8);

  if (assert_equals_mem(expected_uc1.bytes, actual0.bytes, 4))
    return 1;
  if (assert_equals_mem(expected_uc1.bytes, actual1.bytes, 4))
    return 1;
  if (assert_equals_mem(expected_uc1.bytes, actual2.bytes, 4))
    return 1;
  if (assert_equals_mem(expected_uc1.bytes, actual3.bytes, 4))
    return 1;
  if (assert_equals_mem(expected_uc2.bytes, actual4.bytes, 4))
    return 1;
  if (assert_equals_mem(expected_uc2.bytes, actual5.bytes, 4))
    return 1;
  if (assert_equals_mem(expected_uc6.bytes, actual6.bytes, 4))
    return 1;
  if (assert_equals_mem(expected_uc7.bytes, actual7.bytes, 4))
    return 1;
  if (assert_equals_mem(expected_uc8.bytes, actual8.bytes, 4))
    return 1;
  
  if (assert_equals_unsigned_int(expected_pos1, input_pos1))
    return 1;
  if (assert_equals_unsigned_int(expected_pos2, input_pos2))
    return 1;
  if (assert_equals_unsigned_int(expected_pos3, input_pos3))
    return 1;
  if (assert_equals_unsigned_int(expected_pos4, input_pos4))
    return 1;
  if (assert_equals_unsigned_int(expected_pos5, input_pos5))
    return 1;
  if (assert_equals_unsigned_int(expected_pos6, input_pos6))
    return 1;
  if (assert_equals_unsigned_int(expected_pos7, input_pos7))
    return 1;
  if (assert_equals_unsigned_int(expected_pos8, input_pos8))
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

  char *input1 = "";
  char *input2 = "asdf ";
  char *input3 = " d \n";
  char *input4 = " \nd";
  size_t input_pos2 = 0;
  size_t input_pos3 = 0;
  size_t input_pos4 = 2;
  size_t input_pos5 = 0;

  int actual1 = has_non_whitespace_before_newline(input1, NULL);
  int actual2 = has_non_whitespace_before_newline(input2, &input_pos2);
  int actual3 = has_non_whitespace_before_newline(input3, &input_pos3);
  int actual4 = has_non_whitespace_before_newline(input3, &input_pos4);
  int actual5 = has_non_whitespace_before_newline(input4, &input_pos5);

  if (assert_equals_int(expected1, actual1))
    return 1;
  if (assert_equals_int(expected2, actual2))
    return 1;
  if (assert_equals_int(expected3, actual3))
    return 1;
  if (assert_equals_int(expected4, actual4))
    return 1;
  if (assert_equals_int(expected5, actual5))
    return 1;

  if (assert_equals_unsigned_int(expected_pos2, input_pos2))
    return 1;
  if (assert_equals_unsigned_int(expected_pos3, input_pos3))
    return 1;
  if (assert_equals_unsigned_int(expected_pos4, input_pos4))
    return 1;
  if (assert_equals_unsigned_int(expected_pos5, input_pos5))
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

  if (assert_equals_int(expected1, actual1))
    return 1;
  if (assert_equals_int(expected2, actual2))
    return 1;
  if (assert_equals_int(expected3, actual3))
    return 1;
  if (assert_equals_int(expected4, actual4))
    return 1;
  if (assert_equals_int(expected5, actual5))
    return 1;
  if (assert_equals_int(expected6, actual6))
    return 1;
  if (assert_equals_int(expected7, actual7))
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

  char *expected1 = "asdf";
  char *expected4 = "as df";
  char *expected5 = "";

  char *actual1 = stripws(input1);
  char *actual2 = stripws(input2);
  char *actual3 = stripws(input3);
  char *actual4 = stripws(input4);
  char *actual5 = stripws(input5);

  if (assert_equals_str(expected1, actual1))
    return 1;
  if (assert_equals_str(expected1, actual2))
    return 1;
  if (assert_equals_str(expected1, actual3))
    return 1;
  if (assert_equals_str(expected4, actual4))
    return 1;
  if (assert_equals_str(expected5, actual5))
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
  char input_src17[20] = "\\x0030DF";

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
  size_t len17 = strlen(input_src16) + 1;

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
  int expected17 = 1;

  char *expected_inp1 = "asdf";
  char *expected_inp2 = "asdf\n";
  char *expected_inp3 = "dd 0";
  char *expected_inp4 = "dd \n";
  char *expected_inp5 = "";
  char *expected_inp9 = "dd #";
  char *expected_inp12 = "";
  char *expected_inp13 = "as";
  char *expected_inp14 = "dd \n";
  char *expected_inp16 = "dd \nd";
  char *expected_inp17 = "ミ";

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
  int actual17 = unescape_string(input_src17, len17, comment, equals);

  if (assert_equals_int(expected1, actual1))
    return 1;
  if (assert_equals_int(expected2, actual2))
    return 1;
  if (assert_equals_int(expected3, actual3))
    return 1;
  if (assert_equals_int(expected4, actual4))
    return 1;
  if (assert_equals_int(expected5, actual5))
    return 1;
  if (assert_equals_int(expected6, actual6))
    return 1;
  if (assert_equals_int(expected7, actual7))
    return 1;
  if (assert_equals_int(expected8, actual8))
    return 1;
  if (assert_equals_int(expected9, actual9))
    return 1;
  if (assert_equals_int(expected10, actual10))
    return 1;
  if (assert_equals_int(expected11, actual11))
    return 1;
  if (assert_equals_int(expected12, actual12))
    return 1;
  if (assert_equals_int(expected13, actual13))
    return 1;
  if (assert_equals_int(expected14, actual14))
    return 1;
  if (assert_equals_int(expected15, actual15))
    return 1;
  if (assert_equals_int(expected16, actual16))
    return 1;
  if (assert_equals_int(expected17, actual17))
    return 1;

  if (assert_equals_str(expected_inp1, input_src1))
    return 1;
  if (assert_equals_str(expected_inp2, input_src2))
    return 1;
  if (assert_equals_str(expected_inp3, input_src3))
    return 1;
  if (assert_equals_str(expected_inp4, input_src4))
    return 1;
  if (assert_equals_str(expected_inp5, input_src5))
    return 1;
  // input_src6 doesn't need to be tested because of undefined behavior
  // input_src7 doesn't need to be tested because of undefined behavior
  // input_src8 doesn't need to be tested because of undefined behavior
  if (assert_equals_str(expected_inp9, input_src9))
    return 1;
  // input_src10 doesn't need to be tested because of undefined behavior
  // input_src11 doesn't need to be tested because of undefined behavior
  if (assert_equals_str(expected_inp12, input_src12))
    return 1;
  if (assert_equals_str(expected_inp13, input_src13))
    return 1;
  if (assert_equals_str(expected_inp14, input_src14))
    return 1;
  // input_src15 doesn't need to be tested because of undefined behavior
  if (assert_equals_str(expected_inp16, input_src16))
    return 1;
  if (assert_equals_str(expected_inp17, input_src17))
    return 1;

  return 0;
}

int fsize_test() {
  print_test_info("fsize_test()");

  FILE *str1 = fopen("./testfile_empty", "r");
  FILE *str2 = fopen("testfile", "r");

  if (!str1 || !str2) {
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

  // check errors
  if (assert_equals_int(expected_error, error1))
    return 1;
  if (assert_equals_int(expected_error, error2))
    return 1;

  // check sizes
  if (assert_equals_unsigned_int(expected1, actual1))
    return 1;
  if (assert_equals_unsigned_int(expected2, actual2))
    return 1;

  // check position
  if (assert_equals_int(expected_char1, fgetc(str2)))
    return 1;
  if (assert_equals_int(expected_char2, fgetc(str2)))
    return 1;

  return 0;
}

int count_newlines_test() {
  print_test_info("count_newlines_test()");
  char *input1 = "";
  char *input2 = "f\n";
  char *input3 = "\n\r";
  char *input4 = "\r\n";
  char *input5 = "\n\n\r";
  char *input6 = " \n";

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

  if (assert_equals_unsigned_int(expected1, actual1))
    return 1;
  if (assert_equals_unsigned_int(expected2, actual2))
    return 1;
  if (assert_equals_unsigned_int(expected3, actual3))
    return 1;
  if (assert_equals_unsigned_int(expected4, actual4))
    return 1;
  if (assert_equals_unsigned_int(expected5, actual5))
    return 1;
  if (assert_equals_unsigned_int(expected6, actual6))
    return 1;

  if (assert_equals_unsigned_int(expected_pos2, input_pos2))
    return 1;
  if (assert_equals_unsigned_int(expected_pos4, input_pos4))
    return 1;
  if (assert_equals_unsigned_int(expected_pos5, input_pos5))
    return 1;
  if (assert_equals_unsigned_int(expected_pos6, input_pos6))
    return 1;

  return 0;
}

int skip_whitespaces_test() {
  print_test_info("skip_whitespaces_test()");
  char *input1 = "";
  char *input2 = "s";
  char *input3 = " s";
  char *input4 = " s ";
  char *input5 = "\n \r\n ";
  char *input6 = "asdf  \t ";
  char *input7 = " ";

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

  // check output
  if (assert_equals_unsigned_int(expected1, actual1))
    return 1;
  if (assert_equals_unsigned_int(expected2, actual2))
    return 1;
  if (assert_equals_unsigned_int(expected3, actual3))
    return 1;
  if (assert_equals_unsigned_int(expected4, actual4))
    return 1;
  if (assert_equals_unsigned_int(expected5, actual5))
    return 1;
  if (assert_equals_unsigned_int(expected6, actual6))
    return 1;
  if (assert_equals_unsigned_int(expected7, actual7))
    return 1;

  // check positions
  if (assert_equals_unsigned_int(expected_pos1, input_pos1))
    return 1;
  if (assert_equals_unsigned_int(expected_pos2, input_pos2))
    return 1;
  if (assert_equals_unsigned_int(expected_pos3, input_pos3))
    return 1;
  if (assert_equals_unsigned_int(expected_pos4, input_pos4))
    return 1;
  if (assert_equals_unsigned_int(expected_pos5, input_pos5))
    return 1;
  if (assert_equals_unsigned_int(expected_pos6, input_pos6))
    return 1;

  // check linenumbers
  if (assert_equals_unsigned_int(expected_ln4, input_ln4))
    return 1;
  if (assert_equals_unsigned_int(expected_ln5, input_ln5))
    return 1;

  return 0;
}

int skip_comment_test() {
  print_test_info("skip_comment_test()");
  char *input1 = "";
  char *input2 = "asdf";
  char *input3 = "1234";
  char *input4 = "1234\n";
  char *input5 = "12\n34";
  char *input6 = "12\n 34";

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

  if (assert_equals_unsigned_int(expected1, actual1))
    return 1;
  if (assert_equals_unsigned_int(expected2, actual2))
    return 1;
  if (assert_equals_unsigned_int(expected3, actual3))
    return 1;
  if (assert_equals_unsigned_int(expected4, actual4))
    return 1;
  if (assert_equals_unsigned_int(expected5, actual5))
    return 1;
  if (assert_equals_unsigned_int(expected6, actual6))
    return 1;

  if (assert_equals_unsigned_int(expected_pos1, input_pos1))
    return 1;
  if (assert_equals_unsigned_int(expected_pos2, input_pos2))
    return 1;
  if (assert_equals_unsigned_int(expected_pos3, input_pos3))
    return 1;
  if (assert_equals_unsigned_int(expected_pos4, input_pos4))
    return 1;
  if (assert_equals_unsigned_int(expected_pos5, input_pos5))
    return 1;

  return 0;
}

int contains_utf8_test() {
  print_test_info("contains_utf_8_test()");
  char *input1 = "";
  char *input2 = "asdf";
  char *input3 = "ä";
  char *input4 = "asäfa";

  int expected1 = 0;
  int expected2 = 0;
  int expected3 = 1;
  int expected4 = 1;

  int actual1 = contains_utf8(input1);
  int actual2 = contains_utf8(input2);
  int actual3 = contains_utf8(input3);
  int actual4 = contains_utf8(input4);

  if (assert_equals_int(expected1, actual1))
    return 1;
  if (assert_equals_int(expected2, actual2))
    return 1;
  if (assert_equals_int(expected3, actual3))
    return 1;
  if (assert_equals_int(expected4, actual4))
    return 1;

  return 0;
}

int contains_escape_characters_test() {
  print_test_info("contains_escape_characters_test()");
  char comment = '#';
  char equals = '=';

  char *input1 = "";
  char *input2 = "asdf";
  char *input3 = "asdf\n";
  char *input4 = "asdf#dd";
  char *input5 = "asdf dd";

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

  if (assert_equals_int(expected1, actual1))
    return 1;
  if (assert_equals_int(expected2, actual2))
    return 1;
  if (assert_equals_int(expected3, actual3))
    return 1;
  if (assert_equals_int(expected4, actual4))
    return 1;
  if (assert_equals_int(expected5, actual5))
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

  int actual1 = read_name(input_name1, input1, start1, end1, comment, equals,
                          allow_utf8_1);
  int actual2 = read_name(input_name2, input2, start2, end2, comment, equals,
                          allow_utf8_1);
  int actual3 = read_name(input_name3, input3, start3, end3, comment, equals,
                          allow_utf8_1);
  int actual4 = read_name(input_name4, input4, start4, end4, comment, equals,
                          allow_utf8_1);
  int actual5 = read_name(input_name5, input5, start5, end5, comment, equals,
                          allow_utf8_5);
  int actual6 = read_name(input_name6, input6, start6, end6, comment, equals,
                          allow_utf8_1);
  int actual7 = read_name(input_name7, input7, start7, end7, comment, equals,
                          allow_utf8_1);

  int expected1 = 1;
  int expected2 = 1;
  int expected3 = 1;
  int expected4 = 0;
  int expected5 = 0;
  int expected6 = 1;
  int expected7 = 1;

  char *expected_name1 = "";
  char *expected_name2 = "asdf";
  char *expected_name3 = "asdf";
  // expected_name4 and expected_name5 not needed because the output is
  // undefined
  char *expected_name6 = "asdf";
  char *expected_name7 = "asdf";

  if (assert_equals_int(expected1, actual1))
    return 1;
  if (assert_equals_int(expected2, actual2))
    return 1;
  if (assert_equals_int(expected3, actual3))
    return 1;
  if (assert_equals_int(expected4, actual4))
    return 1;
  if (assert_equals_int(expected5, actual5))
    return 1;
  if (assert_equals_int(expected6, actual6))
    return 1;
  if (assert_equals_int(expected7, actual7))
    return 1;

  if (assert_equals_str(expected_name1, input_name1))
    return 1;
  if (assert_equals_str(expected_name2, input_name2))
    return 1;
  if (assert_equals_str(expected_name3, input_name3))
    return 1;
  if (assert_equals_str(expected_name6, input_name6))
    return 1;
  if (assert_equals_str(expected_name7, input_name7))
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

  char *expected_inp1 = "";
  char *expected_inp2 = "asdf";
  char *expected_inp3 = " asdf ";
  char *expected_inp4 = "a";
  char *expected_inp7 = " asdf ";

  int actual1 = remove_quotes(input1);
  int actual2 = remove_quotes(input2);
  int actual3 = remove_quotes(input3);
  int actual4 = remove_quotes(input4);
  int actual5 = remove_quotes(input5);
  int actual6 = remove_quotes(input6);
  int actual7 = remove_quotes(input7);
  int actual8 = remove_quotes(input8);

  if (assert_equals_int(expected1, actual1))
    return 1;
  if (assert_equals_int(expected2, actual2))
    return 1;
  if (assert_equals_int(expected3, actual3))
    return 1;
  if (assert_equals_int(expected4, actual4))
    return 1;
  if (assert_equals_int(expected5, actual5))
    return 1;
  if (assert_equals_int(expected6, actual6))
    return 1;
  if (assert_equals_int(expected7, actual7))
    return 1;
  if (assert_equals_int(expected8, actual8))
    return 1;

  if (assert_equals_str(expected_inp1, input1))
    return 1;
  if (assert_equals_str(expected_inp2, input2))
    return 1;
  if (assert_equals_str(expected_inp3, input3))
    return 1;
  if (assert_equals_str(expected_inp4, input4))
    return 1;
  if (assert_equals_str(expected_inp7, input7))
    return 1;

  return 0;
}

int read_value_test() {
  print_test_info("read_value_test()");
  char comment = '#';
  char equals = '=';

  char *str1 = "";
  char *str2 = "asdf";
  char *str3 = "asdf \\\nasdf";
  char *str4 = "asdf \\\nasdf\\\nasdf";
  char *str5 = " asdf ";
  char *str6 = "\" asdf \"";
  char *str7 = "\' asdf \"";
  char *str8 = "\' \\x0030Df\'";
  char *str9 = " \\n";
  char *str10 = " asdf\\nasdf";
  char *str11 = "\\x0030df";
  char *str12 = "as]df";
  char *str13 = "as\\\\ndf";

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

  char *expected_value1 = "";
  char *expected_value2 = "asdf";
  char *expected_value3 = "asdf \nasdf";
  char *expected_value4 = "asdf \nasdf\nasdf";
  char *expected_value5 = "asdf";
  char *expected_value6 = " asdf ";
  char *expected_value8 = " ミ";
  char *expected_value9 = "\n";
  char *expected_value10 = "sdf\nas";
  char *expected_value13 = "as\\ndf";

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

  int actual1 = read_value(value1, str1, start1, end1, comment, equals, &ln1,
                           allow_utf8_1);
  int actual2 = read_value(value2, str2, start2, end2, comment, equals, &ln2,
                           allow_utf8_1);
  int actual3 = read_value(value3, str3, start3, end3, comment, equals, NULL,
                           allow_utf8_1);
  int actual4 = read_value(value4, str4, start4, end4, comment, equals, &ln4,
                           allow_utf8_1);
  int actual5 = read_value(value5, str5, start5, end5, comment, equals, &ln5,
                           allow_utf8_1);
  int actual6 = read_value(value6, str6, start6, end6, comment, equals, &ln6,
                           allow_utf8_1);
  int actual7 = read_value(value7, str7, start7, end7, comment, equals, &ln7,
                           allow_utf8_1);
  int actual8 = read_value(value8, str8, start8, end8, comment, equals, &ln8,
                           allow_utf8_1);
  int actual9 = read_value(value9, str9, start9, end9, comment, equals, &ln9,
                           allow_utf8_1);
  int actual10 = read_value(value10, str10, start10, end10, comment, equals,
                            &ln10, allow_utf8_1);
  int actual11 = read_value(value11, str11, start11, end11, comment, equals,
                            &ln11, allow_utf8_11);
  int actual12 = read_value(value12, str12, start12, end12, comment, equals,
                            &ln12, allow_utf8_11);
  int actual13 = read_value(value13, str13, start13, end13, comment, equals,
                            &ln13, allow_utf8_11);

  if (assert_equals_int(expected1, actual1))
    return 1;
  if (assert_equals_int(expected2, actual2))
    return 1;
  if (assert_equals_int(expected3, actual3))
    return 1;
  if (assert_equals_int(expected4, actual4))
    return 1;
  if (assert_equals_int(expected5, actual5))
    return 1;
  if (assert_equals_int(expected6, actual6))
    return 1;
  if (assert_equals_int(expected7, actual7))
    return 1;
  if (assert_equals_int(expected8, actual8))
    return 1;
  if (assert_equals_int(expected9, actual9))
    return 1;
  if (assert_equals_int(expected10, actual10))
    return 1;
  if (assert_equals_int(expected11, actual11))
    return 1;
  if (assert_equals_int(expected12, actual12))
    return 1;
  if (assert_equals_int(expected13, actual13))
    return 1;

  if (assert_equals_str(expected_value1, value1))
    return 1;
  if (assert_equals_str(expected_value2, value2))
    return 1;
  if (assert_equals_str(expected_value3, value3))
    return 1;
  if (assert_equals_str(expected_value4, value4))
    return 1;
  if (assert_equals_str(expected_value5, value5))
    return 1;
  if (assert_equals_str(expected_value6, value6))
    return 1;
  if (assert_equals_str(expected_value8, value8))
    return 1;
  if (assert_equals_str(expected_value9, value9))
    return 1;
  if (assert_equals_str(expected_value10, value10))
    return 1;
  if (assert_equals_str(expected_value13, value13))
    return 1;

  if (assert_equals_unsigned_int(expected_ln1, ln1))
    return 1;
  if (assert_equals_unsigned_int(expected_ln2, ln2))
    return 1;
  if (assert_equals_unsigned_int(expected_ln4, ln4))
    return 1;
  if (assert_equals_unsigned_int(expected_ln5, ln5))
    return 1;
  if (assert_equals_unsigned_int(expected_ln6, ln6))
    return 1;
  if (assert_equals_unsigned_int(expected_ln7, ln7))
    return 1;
  if (assert_equals_unsigned_int(expected_ln8, ln8))
    return 1;
  if (assert_equals_unsigned_int(expected_ln9, ln9))
    return 1;
  if (assert_equals_unsigned_int(expected_ln10, ln10))
    return 1;
  if (assert_equals_unsigned_int(expected_ln11, ln11))
    return 1;
  if (assert_equals_unsigned_int(expected_ln12, ln12))
    return 1;
  if (assert_equals_unsigned_int(expected_ln13, ln13))
    return 1;

  return 0;
}

int check_for_BOM_test() {
  print_test_info("check_for_BOM_test()");
  FILE *input1 = fopen("check_for_BOM_test/input1.txt", "r");
  FILE *input2 = fopen("check_for_BOM_test/input2.txt", "r");
  FILE *input3 = fopen("check_for_BOM_test/input3.txt", "r");

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

  if (assert_equals_int(expected1, actual1))
    return 1;
  if (assert_equals_int(expected2, actual2))
    return 1;
  if (assert_equals_int(expected3, actual3))
    return 1;

  if (assert_equals_int(expected_c1, actual_c1))
    return 1;
  if (assert_equals_int(expected_c2, actual_c2))
    return 1;
  if (assert_equals_int(expected_c3, actual_c3))
    return 1;

  return 0;
}

int get_utf8_len_test() {
  print_test_info("get_utf8_len_test()");

  char input1 = (char)0x3b;
  char input2 = (char)0xce;
  char input3 = (char)0xe0;
  char input4 = (char)0xf3;
  char input5 = (char)0xff;

  size_t actual1 = get_utf8_len(input1);
  size_t actual2 = get_utf8_len(input2);
  size_t actual3 = get_utf8_len(input3);
  size_t actual4 = get_utf8_len(input4);
  size_t actual5 = get_utf8_len(input5);

  size_t expected1 = 1;
  size_t expected2 = 2;
  size_t expected3 = 3;
  size_t expected4 = 4;
  size_t expected5 = 0;

  if (assert_equals_unsigned_int(expected1, actual1))
    return 1;
  if (assert_equals_unsigned_int(expected2, actual2))
    return 1;
  if (assert_equals_unsigned_int(expected3, actual3))
    return 1;
  if (assert_equals_unsigned_int(expected4, actual4))
    return 1;
  if (assert_equals_unsigned_int(expected5, actual5))
    return 1;

  return 0;
}

int binary_to_hex_digits_test() {
  print_test_info("binary_to_hex_digits_test()");

  char input1 = (char)0x00;
  char input2 = (char)0x03;
  char input3 = (char)0xF0;
  char input4 = (char)0x3F;
  char input5 = (char)0x23;
  char input6 = (char)0xFE;

  struct two_bytes expected1 = {{0x30, 0x30}};
  struct two_bytes expected2 = {{0x33, 0x30}};
  struct two_bytes expected3 = {{0x30, 0x46}};
  struct two_bytes expected4 = {{0x46, 0x33}};
  struct two_bytes expected5 = {{0x33, 0x32}};
  struct two_bytes expected6 = {{0x45, 0x46}};

  struct two_bytes result1 = binary_to_hex_digits(input1);
  struct two_bytes result2 = binary_to_hex_digits(input2);
  struct two_bytes result3 = binary_to_hex_digits(input3);
  struct two_bytes result4 = binary_to_hex_digits(input4);
  struct two_bytes result5 = binary_to_hex_digits(input5);
  struct two_bytes result6 = binary_to_hex_digits(input6);

  if (assert_equals_mem(expected1.bytes, result1.bytes, 2))
    return 1;
  if (assert_equals_mem(expected2.bytes, result2.bytes, 2))
    return 1;
  if (assert_equals_mem(expected3.bytes, result3.bytes, 2))
    return 1;
  if (assert_equals_mem(expected4.bytes, result4.bytes, 2))
    return 1;
  if (assert_equals_mem(expected5.bytes, result5.bytes, 2))
    return 1;
  if (assert_equals_mem(expected6.bytes, result6.bytes, 2))
    return 1;
  
  return 0;
}

int utf8touc_test() {
  print_test_info("utf8touc_test()");
  struct four_bytes input1 = {{0x3b, 0, 0, 0}};
  struct four_bytes input2 = {{0x8f, 0xce, 0, 0}};
  struct four_bytes input3 = {{0x9e, 0xa6, 0xe0, 0}};
  struct four_bytes input4 = {{0xa0, 0x80, 0xa0, 0xf3}};
  struct four_bytes input5 = {{0xff, 0xff, 0xff, 0xff}};
  struct four_bytes input6 = {{0, 0, 0, 1}};

  struct four_bytes actual1 = utf8touc(input1);
  struct four_bytes actual2 = utf8touc(input2);
  struct four_bytes actual3 = utf8touc(input3);
  struct four_bytes actual4 = utf8touc(input4);
  struct four_bytes actual5 = utf8touc(input5);
  struct four_bytes actual6 = utf8touc(input6);

  struct four_bytes expected1 = {{0x3b, 0, 0, 0}};
  struct four_bytes expected2 = {{0x8f, 3, 0, 0}};
  struct four_bytes expected3 = {{0x9e, 9, 0, 0}};
  struct four_bytes expected4 = {{0x20, 0, 0xe, 0}};
  struct four_bytes expected5 = {{0xff, 0xff, 0xff, 0xff}};
  struct four_bytes expected6 = {{0xff, 0xff, 0xff, 0xff}};

  if (assert_equals_mem(expected1.bytes, actual1.bytes, 4))
    return 1;
  if (assert_equals_mem(expected2.bytes, actual2.bytes, 4))
    return 1;
  if (assert_equals_mem(expected3.bytes, actual3.bytes, 4))
    return 1;
  if (assert_equals_mem(expected4.bytes, actual4.bytes, 4))
    return 1;
  if (assert_equals_mem(expected5.bytes, actual5.bytes, 4))
    return 1;
  if (assert_equals_mem(expected6.bytes, actual6.bytes, 4))
    return 1;

  return 0;
}

int write_value_test() {
	print_test_info("write_value_test()");

	struct ini_file f1;
	struct ini_file f2;
	struct ini_file f3;
	struct ini_file f4;
	struct ini_file f5;
	struct ini_file f6;

	ini_open_file(&f1, "write_value_test/file1.test.txt", '=', '#', INI_UTF8_MODE_ALLOW);
	ini_open_file(&f2, "write_value_test/file2.test.txt", '=', '#', INI_UTF8_MODE_ALLOW);
	ini_open_file(&f3, "write_value_test/file3.test.txt", '=', '#', INI_UTF8_MODE_ESCAPE);
	ini_open_file(&f4, "write_value_test/file4.test.txt", '=', '#', INI_UTF8_MODE_FORBID);
	ini_open_file(&f5, "write_value_test/file5.test.txt", '=', '#', INI_UTF8_MODE_ALLOW_WITH_BOM);
	ini_open_file(&f6, "write_value_test/file6.test.txt", '=', '#', INI_UTF8_MODE_ESCAPE);

	const char* input0 = "test";
	const char* input1 = NULL;
	const char* input2 = "\1test# \n \b ]";
    char input3[100] = "\1test \n \b ]";
	input3[3] = (char)0xFF;
	const char* input4 = "\1test# \n \b ]";
	const char* input5 = "\1test#äüß \n \b ] 壊れないで下さい。";
	const char* input6 = "\1test#äüß \n \b ] 壊れないで下さい。";
	
	int result0 = write_value(NULL, input0);
	int result1 = write_value(&f1, input1);
	int result2 = write_value(&f2, input2);
	int result3 = write_value(&f3, input3);
	int result4 = write_value(&f4, input4);
	int result5 = write_value(&f5, input5);
	int result6 = write_value(&f6, input6);

	ini_close_file(&f1);
	ini_close_file(&f2);
	ini_close_file(&f3);
	ini_close_file(&f4);
	ini_close_file(&f5);
	ini_close_file(&f6);

	int expected0 = 0;
	int expected1 = 0;
	int expected2 = 1;
	int expected3 = 0;
	int expected4 = 0;
	int expected5 = 1;
	int expected6 = 1;

	if(assert_equals_int(expected0, result0))
		return 1;
	if(assert_equals_int(expected1, result1))
		return 1;
	if(assert_equals_int(expected2, result2))
		return 1;
	if(assert_equals_int(expected3, result3))
		return 1;
	if(assert_equals_int(expected4, result4))
		return 1;
	if(assert_equals_int(expected5, result5))
		return 1;
	if(assert_equals_int(expected6, result6))
		return 1;

	if(assert_equals_file("write_value_test/file2.exp.txt", "write_value_test/file2.test.txt", 128))
		return 1;
	if(assert_equals_file("write_value_test/file5.exp.txt", "write_value_test/file5.test.txt", 128))
		return 1;
	if(assert_equals_file("write_value_test/file6.exp.txt", "write_value_test/file6.test.txt", 128))
		return 1;
	
	return 0;
}
