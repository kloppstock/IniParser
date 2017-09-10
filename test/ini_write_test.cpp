/* Copyright (c) 2017 Jonas Schenke
 *
 * This file is part of IniParser.
 *
 * IniParser is licensed under the MIT license. Form more information see
 * LICENSE.
 */

#include <fcntl.h>
#include <unistd.h>

#include "../include/ini_helper.h"
#include "../include/ini_parser.h"
#include "test.hpp"

int ini_open_and_close_file_test() {
  print_test_info("ini_open_and_close_file_test()");

  struct ini_file f1;
  struct ini_file f2;
  struct ini_file f3;
  struct ini_file f4;
  struct ini_file f5;
  struct ini_file f6;

  int result_o1 =
      ini_open_file(&f1, "ini_open_and_close_file_test/bom.test.txt", '=', '#',
                    INI_UTF8_MODE_ALLOW_WITH_BOM);
  int result_o2 =
      ini_open_file(&f2, "ini_open_and_close_file_test/no_bom.test.txt", '=',
                    '#', INI_UTF8_MODE_ALLOW);
  int result_o3 = ini_open_file(&f3, "", '=', '#', INI_UTF8_MODE_ALLOW);
  int result_o4 = ini_open_file(&f4, NULL, '=', '#', INI_UTF8_MODE_ALLOW);
  int result_o5 = ini_open_file(NULL, "test", '=', '#', INI_UTF8_MODE_ALLOW);
  int result_o6 = ini_open_file(&f5, "test", '\0', '#', INI_UTF8_MODE_ALLOW);
  int result_o7 = ini_open_file(&f6, "test", '=', '\0', INI_UTF8_MODE_ALLOW);

  int result_c1 = ini_close_file(&f1);
  int result_c2 = ini_close_file(&f2);
  int result_c3 = ini_close_file(NULL);

  int expected_o1 = 1;
  int expected_o2 = 1;
  int expected_o3 = 0;
  int expected_o4 = 0;
  int expected_o5 = 0;
  int expected_o6 = 0;
  int expected_o7 = 0;

  int expected_c1 = 1;
  int expected_c2 = 1;
  int expected_c3 = 0;

  if (assert_equals_file("ini_open_and_close_file_test/bom.test.txt",
                         "ini_open_and_close_file_test/bom.exp.txt", 128))
    return 1;
  if (assert_equals_file("ini_open_and_close_file_test/no_bom.test.txt",
                         "ini_open_and_close_file_test/no_bom.exp.txt", 128))
    return 1;

  if (assert_equals_int(expected_o1, result_o1))
    return 1;
  if (assert_equals_int(expected_o2, result_o2))
    return 1;
  if (assert_equals_int(expected_o3, result_o3))
    return 1;
  if (assert_equals_int(expected_o4, result_o4))
    return 1;
  if (assert_equals_int(expected_o5, result_o5))
    return 1;
  if (assert_equals_int(expected_o6, result_o6))
    return 1;
  if (assert_equals_int(expected_o7, result_o7))
    return 1;

  if (assert_equals_int(expected_c1, result_c1))
    return 1;
  if (assert_equals_int(expected_c2, result_c2))
    return 1;
  if (assert_equals_int(expected_c3, result_c3))
    return 1;

  return 0;
}

int ini_write_section_test() {
  print_test_info("ini_write_section_test()");

  struct ini_file f1;
  struct ini_file f2;
  struct ini_file f3;
  struct ini_file f4;
  struct ini_file f5;
  struct ini_file f6;
  struct ini_file f7;
  struct ini_file f8;

  ini_open_file(&f1, "ini_write_section_test/file1.test.txt", '=', '#',
                INI_UTF8_MODE_FORBID);
  ini_open_file(&f2, "ini_write_section_test/file2.test.txt", '=', '#',
                INI_UTF8_MODE_FORBID);
  ini_open_file(&f3, "ini_write_section_test/file3.test.txt", '=', '#',
                INI_UTF8_MODE_FORBID);
  ini_open_file(&f4, "ini_write_section_test/file4.test.txt", '=', '#',
                INI_UTF8_MODE_FORBID);
  ini_open_file(&f5, "ini_write_section_test/file5.test.txt", '=', '#',
                INI_UTF8_MODE_FORBID);
  ini_open_file(&f6, "ini_write_section_test/file6.test.txt", '=', '#',
                INI_UTF8_MODE_FORBID);
  ini_open_file(&f7, "ini_write_section_test/file7.test.txt", '=', '#',
                INI_UTF8_MODE_FORBID);
  ini_open_file(&f8, "ini_write_section_test/file8.test.txt", '=', '#',
                INI_UTF8_MODE_FORBID);

  int result1 = ini_write_section(NULL, "");
  int result2 = ini_write_section(&f1, "");
  int result3 = ini_write_section(&f2, NULL);
  int result4 = ini_write_section(&f3, "section");
  int result5 = ini_write_section(&f4, "section [");
  int result6 = ini_write_section(&f5, " section");
  int result7 = ini_write_section(&f6, "section ");
  int result8 = ini_write_section(&f7, "secätion");
  int result9 = ini_write_section(&f8, "sect#ion");

  ini_close_file(&f1);
  ini_close_file(&f2);
  ini_close_file(&f3);
  ini_close_file(&f4);
  ini_close_file(&f5);
  ini_close_file(&f6);
  ini_close_file(&f7);
  ini_close_file(&f8);

  int expected1 = 0;
  int expected2 = 0;
  int expected3 = 0;
  int expected4 = 1;
  int expected5 = 0;
  int expected6 = 0;
  int expected7 = 0;
  int expected8 = 0;
  int expected9 = 0;

  if (assert_equals_int(expected1, result1))
    return 1;
  if (assert_equals_int(expected2, result2))
    return 1;
  if (assert_equals_int(expected3, result3))
    return 1;
  if (assert_equals_int(expected4, result4))
    return 1;
  if (assert_equals_int(expected5, result5))
    return 1;
  if (assert_equals_int(expected6, result6))
    return 1;
  if (assert_equals_int(expected7, result7))
    return 1;
  if (assert_equals_int(expected8, result8))
    return 1;
  if (assert_equals_int(expected9, result9))
    return 1;

  if (assert_equals_file("ini_write_section_test/file1.exp.txt",
                         "ini_write_section_test/file1.test.txt", 128))
    return 1;
  if (assert_equals_file("ini_write_section_test/file2.exp.txt",
                         "ini_write_section_test/file2.test.txt", 128))
    return 1;
  if (assert_equals_file("ini_write_section_test/file3.exp.txt",
                         "ini_write_section_test/file3.test.txt", 128))
    return 1;
  if (assert_equals_file("ini_write_section_test/file4.exp.txt",
                         "ini_write_section_test/file4.test.txt", 128))
    return 1;
  if (assert_equals_file("ini_write_section_test/file5.exp.txt",
                         "ini_write_section_test/file5.test.txt", 128))
    return 1;
  if (assert_equals_file("ini_write_section_test/file6.exp.txt",
                         "ini_write_section_test/file6.test.txt", 128))
    return 1;
  if (assert_equals_file("ini_write_section_test/file7.exp.txt",
                         "ini_write_section_test/file7.test.txt", 128))
    return 1;
  if (assert_equals_file("ini_write_section_test/file8.exp.txt",
                         "ini_write_section_test/file8.test.txt", 128))
    return 1;

  return 0;
}

int ini_write_name_value_test() {
  print_test_info("ini_write_name_value_test()");

  struct ini_file f2;
  struct ini_file f3;
  struct ini_file f4;
  struct ini_file f5;
  struct ini_file f6;
  struct ini_file f7;
  struct ini_file f8;

  ini_open_file(&f2, "ini_write_name_value_test/file2.test.txt", '=', '#',
                INI_UTF8_MODE_ALLOW);
  ini_open_file(&f3, "ini_write_name_value_test/file3.test.txt", '=', '#',
                INI_UTF8_MODE_ALLOW);
  ini_open_file(&f4, "ini_write_name_value_test/file4.test.txt", '=', '#',
                INI_UTF8_MODE_ALLOW);
  ini_open_file(&f5, "ini_write_name_value_test/file5.test.txt", '=', '#',
                INI_UTF8_MODE_ALLOW);
  ini_open_file(&f6, "ini_write_name_value_test/file6.test.txt", '=', '#',
                INI_UTF8_MODE_ALLOW);
  ini_open_file(&f7, "ini_write_name_value_test/file7.test.txt", '=', '#',
                INI_UTF8_MODE_ALLOW_WITH_BOM);
  ini_open_file(&f8, "ini_write_name_value_test/file8.test.txt", '=', '#',
                INI_UTF8_MODE_ESCAPE);

  const char *name1 = "test";
  const char *name2 = NULL;
  const char *name3 = "test";
  const char *name4 = " test";
  const char *name5 = "täst";
  const char *name6 = "";
  const char *name7 = "name";
  const char *name8 = "name";

  const char *value1 = "test";
  const char *value2 = "test";
  const char *value3 = NULL;
  const char *value4 = "test";
  const char *value5 = "test";
  const char *value6 = "test";
  const char *value7 = "\"壊れないで下さい。　\n #";
  const char *value8 = "\"壊れないで下さい。　\n #";

  int result1 = ini_write_name_value(NULL, name1, value1);
  int result2 = ini_write_name_value(&f2, name2, value2);
  int result3 = ini_write_name_value(&f3, name3, value3);
  int result4 = ini_write_name_value(&f4, name4, value4);
  int result5 = ini_write_name_value(&f5, name5, value5);
  int result6 = ini_write_name_value(&f6, name6, value6);
  int result7 = ini_write_name_value(&f7, name7, value7);
  int result8 = ini_write_name_value(&f8, name8, value8);

  ini_close_file(&f2);
  ini_close_file(&f3);
  ini_close_file(&f4);
  ini_close_file(&f5);
  ini_close_file(&f6);
  ini_close_file(&f7);
  ini_close_file(&f8);

  int expected1 = 0;
  int expected2 = 0;
  int expected3 = 1;
  int expected4 = 0;
  int expected5 = 0;
  int expected6 = 0;
  int expected7 = 1;
  int expected8 = 1;

  if (assert_equals_int(expected1, result1))
    return 1;
  if (assert_equals_int(expected2, result2))
    return 1;
  if (assert_equals_int(expected3, result3))
    return 1;
  if (assert_equals_int(expected4, result4))
    return 1;
  if (assert_equals_int(expected5, result5))
    return 1;
  if (assert_equals_int(expected6, result6))
    return 1;
  if (assert_equals_int(expected7, result7))
    return 1;
  if (assert_equals_int(expected8, result8))
    return 1;

  if (assert_equals_file("ini_write_name_value_test/file3.exp.txt",
                         "ini_write_name_value_test/file3.test.txt", 128))
    return 1;
  if (assert_equals_file("ini_write_name_value_test/file7.exp.txt",
                         "ini_write_name_value_test/file7.test.txt", 128))
    return 1;
  if (assert_equals_file("ini_write_name_value_test/file8.exp.txt",
                         "ini_write_name_value_test/file8.test.txt", 128))
    return 1;

  return 0;
}

int ini_verify_utf8_test() {
  print_test_info("verify_utf8_test()");

  const char *input1 = NULL;
  const char *input2 = "";
  const char *input3 = " test äß\ntestö";
  const char *input4 = " test äß\ntest";
  char input5[100] = "123456780123467890124372820347810239784\0\0\0\0\0\0";
  input5[6] = (char)0xBB;
  const char *input6 = " \ntest";
  const char *input7 = "四是四。";

  int result1 = ini_verify_utf8(input1, 1);
  int result2 = ini_verify_utf8(input2, 1);
  int result3 = ini_verify_utf8(input3, 1);
  int result4 = ini_verify_utf8(input4, 0);
  int result5 = ini_verify_utf8(input5, 1);
  int result6 = ini_verify_utf8(input6, 0);
  int result7 = ini_verify_utf8(input7, 1);

  int expected1 = 0;
  int expected2 = 1;
  int expected3 = 1;
  int expected4 = 0;
  int expected5 = 0;
  int expected6 = 1;
  int expected7 = 1;

  if (assert_equals_int(expected1, result1))
    return 1;
  if (assert_equals_int(expected2, result2))
    return 1;
  if (assert_equals_int(expected3, result3))
    return 1;
  if (assert_equals_int(expected4, result4))
    return 1;
  if (assert_equals_int(expected5, result5))
    return 1;
  if (assert_equals_int(expected6, result6))
    return 1;
  if (assert_equals_int(expected7, result7))
    return 1;

  return 0;
}

int ini_write_comment_test() {
  print_test_info("verify_utf8_test()");

  struct ini_file f2;
  struct ini_file f3;
  struct ini_file f4;
  struct ini_file f5;
  struct ini_file f6;
  struct ini_file f7;
  struct ini_file f8;
  struct ini_file f9;

  ini_open_file(&f2, "ini_write_comment_test/file2.test.txt", '=', '#',
                INI_UTF8_MODE_ALLOW);
  ini_open_file(&f3, "ini_write_comment_test/file3.test.txt", '=', '#',
                INI_UTF8_MODE_ALLOW);
  ini_open_file(&f4, "ini_write_comment_test/file4.test.txt", '=', '#',
                INI_UTF8_MODE_ALLOW);
  ini_open_file(&f5, "ini_write_comment_test/file5.test.txt", '=', '#',
                INI_UTF8_MODE_ALLOW);
  ini_open_file(&f6, "ini_write_comment_test/file6.test.txt", '=', '#',
                INI_UTF8_MODE_ALLOW);
  ini_open_file(&f7, "ini_write_comment_test/file7.test.txt", '=', '#',
                INI_UTF8_MODE_ALLOW_WITH_BOM);
  ini_open_file(&f8, "ini_write_comment_test/file8.test.txt", '=', '#',
                INI_UTF8_MODE_FORBID);
  ini_open_file(&f9, "ini_write_comment_test/file9.test.txt", '=', '#',
                INI_UTF8_MODE_ESCAPE);

  const char *input1 = "";
  const char *input2 = NULL;
  const char *input3 = "";
  const char *input4 = "this is a comment";
  const char *input5 = "Ütf-8";
  const char *input6 = "Ütf-\n8";
  const char *input7 = "Ütf-8\n";
  const char *input8 = "Ütf-8";
  const char *input9 = "Ütf-8";

  int expected1 = 0;
  int expected2 = 0;
  int expected3 = 1;
  int expected4 = 1;
  int expected5 = 1;
  int expected6 = 1;
  int expected7 = 1;
  int expected8 = 0;
  int expected9 = 0;

  int actual1 = ini_write_comment(NULL, input1);
  int actual2 = ini_write_comment(&f2, input2);
  int actual3 = ini_write_comment(&f3, input3);
  int actual4 = ini_write_comment(&f4, input4);
  int actual5 = ini_write_comment(&f5, input5);
  int actual6 = ini_write_comment(&f6, input6);
  int actual7 = ini_write_comment(&f7, input7);
  int actual8 = ini_write_comment(&f8, input8);
  int actual9 = ini_write_comment(&f9, input9);

  ini_close_file(&f2);
  ini_close_file(&f3);
  ini_close_file(&f4);
  ini_close_file(&f5);
  ini_close_file(&f6);
  ini_close_file(&f7);
  ini_close_file(&f8);
  ini_close_file(&f9);

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

  if (assert_equals_file("ini_write_comment_test/file3.exp.txt",
                         "ini_write_comment_test/file3.test.txt", 128))
    return 1;
  if (assert_equals_file("ini_write_comment_test/file4.exp.txt",
                         "ini_write_comment_test/file4.test.txt", 128))
    return 1;
  if (assert_equals_file("ini_write_comment_test/file5.exp.txt",
                         "ini_write_comment_test/file5.test.txt", 128))
    return 1;
  if (assert_equals_file("ini_write_comment_test/file6.exp.txt",
                         "ini_write_comment_test/file6.test.txt", 128))
    return 1;
  if (assert_equals_file("ini_write_comment_test/file7.exp.txt",
                         "ini_write_comment_test/file7.test.txt", 128))
    return 1;

  return 0;
}
