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

struct data_structure {
  size_t index;
  char str[1000];
};

int callback(const char *section, const char *name, const char *value,
             void *data) {
  struct data_structure *ds = (struct data_structure *)data;

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

int callback_empty(const char *section, const char *name, const char *value,
                   void *data) {
  (void)data;
  (void)section;
  (void)name;
  (void)value;
  return 1;
}

int callback_fail(const char *section, const char *name, const char *value,
                  void *data) {
  (void)data;
  (void)section;
  (void)name;
  (void)value;
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

  size_t expected_len1 =
      load_file(expected_buffer1, "ini_read_test/expected1.txt");
  size_t expected_len2 =
      load_file(expected_buffer2, "ini_read_test/expected2.txt");
  size_t expected_len3 =
      load_file(expected_buffer3, "ini_read_test/expected3.txt");
  size_t expected_len13 =
      load_file(expected_buffer13, "ini_read_test/expected13.txt");

  int actual1 =
      ini_read(input1, len1, callback, &ds1, comment, equals, allow_utf8_1);
  int actual2 =
      ini_read(input2, len2, callback, &ds2, comment, equals, allow_utf8_2);
  int actual3 =
      ini_read(input3, len3, callback, &ds3, comment, equals, allow_utf8_3);
  int actual4 = ini_read(input4, len4, callback_empty, NULL, comment, equals,
                         allow_utf8_4);
  int actual5 = ini_read(input5, len5, callback_empty, NULL, comment, equals,
                         allow_utf8_5);
  int actual6 = ini_read(input6, len6, callback_empty, NULL, comment, equals,
                         allow_utf8_6);
  int actual7 = ini_read(input7, len7, callback_empty, NULL, comment, equals,
                         allow_utf8_7);
  int actual8 = ini_read(input8, len8, callback_empty, NULL, comment, equals,
                         allow_utf8_8);
  int actual9 = ini_read(input9, len9, callback_empty, NULL, comment, equals,
                         allow_utf8_9);
  int actual10 = ini_read(input10, len10, callback_empty, NULL, comment, equals,
                          allow_utf8_10);
  int actual11 = ini_read(input11, len11, callback_empty, NULL, comment, equals,
                          allow_utf8_11);
  int actual12 = ini_read(input12, len12, callback_empty, NULL, comment, equals,
                          allow_utf8_12);
  int actual13 =
      ini_read(input13, len13, callback, &ds13, comment, equals, allow_utf8_13);
  int actual14 = ini_read(input14, len14, callback_empty, NULL, comment, equals,
                          allow_utf8_14);
  int actual15 = ini_read(input15, len15, callback_empty, NULL, comment, equals,
                          allow_utf8_15);
  int actual16 = ini_read(input16, len16, callback_fail, NULL, comment, equals,
                          allow_utf8_16);
  int actual17 = ini_read(input17, len17, callback_empty, NULL, comment, ']',
                          allow_utf8_17);

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

  if (assert_equals_unsigned_int(expected_len1, ds1.index))
    return 1;
  if (assert_equals_unsigned_int(expected_len2, ds2.index))
    return 1;
  if (assert_equals_unsigned_int(expected_len3, ds3.index))
    return 1;
  if (assert_equals_unsigned_int(expected_len13, ds13.index))
    return 1;

  if (assert_equals_str(expected_buffer1, ds1.str))
    return 1;
  if (assert_equals_str(expected_buffer2, ds2.str))
    return 1;
  if (assert_equals_str(expected_buffer3, ds3.str))
    return 1;
  if (assert_equals_str(expected_buffer13, ds13.str))
    return 1;

  return 0;
}

int ini_read_file_test() {
  print_test_info("ini_read_file_test()");
  char comment = ';';
  char equals = '=';

  const char *path1 = "";
  const char *path2 = "ini_read_file_test/input2.ini";
  const char *path3 = "ini_read_file_test/input3.ini";
  const char *path4 = "ini_read_file_test/input4.ini";
  const char *path5 = "ini_read_file_test/input5.ini";
  const char *path6 = "ini_read_file_test/input6.ini";

  struct data_structure ds3;
  struct data_structure ds4;

  ds3.index = 0;
  ds3.str[0] = '\0';

  ds4.index = 0;
  ds4.str[0] = '\0';

  enum ini_utf8_mode allow_utf8_1 = INI_UTF8_MODE_ALLOW;
  enum ini_utf8_mode allow_utf8_2 = INI_UTF8_MODE_ALLOW;
  enum ini_utf8_mode allow_utf8_3 = INI_UTF8_MODE_ALLOW;
  enum ini_utf8_mode allow_utf8_4 = INI_UTF8_MODE_ALLOW_WITH_BOM;
  enum ini_utf8_mode allow_utf8_5 = INI_UTF8_MODE_FORBID;
  enum ini_utf8_mode allow_utf8_6 = INI_UTF8_MODE_FORBID;

  int expected1 = -1;
  int expected2 = -1;
  int expected3 = 0;
  int expected4 = 0;
  int expected5 = 7;
  int expected6 = 7;

  char expected_buffer3[1000];
  char expected_buffer4[1000];

  int expected_len3 =
      load_file(expected_buffer3, "ini_read_file_test/expected3.txt");
  int expected_len4 =
      load_file(expected_buffer4, "ini_read_file_test/expected4.txt");

  int actual1 =
      ini_read_file(path1, callback_empty, NULL, comment, equals, allow_utf8_1);
  int actual2 =
      ini_read_file(path2, callback_empty, NULL, comment, equals, allow_utf8_2);
  int actual3 =
      ini_read_file(path3, callback, &ds3, comment, equals, allow_utf8_3);
  int actual4 =
      ini_read_file(path4, callback, &ds4, comment, equals, allow_utf8_4);
  int actual5 =
      ini_read_file(path5, callback_empty, NULL, comment, equals, allow_utf8_5);
  int actual6 =
      ini_read_file(path6, callback_empty, NULL, comment, equals, allow_utf8_6);

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

  if (assert_equals_unsigned_int(expected_len3, ds3.index))
    return 1;
  if (assert_equals_unsigned_int(expected_len4, ds4.index))
    return 1;

  if (assert_equals_str(expected_buffer3, ds3.str))
    return 1;
  if (assert_equals_str(expected_buffer4, ds4.str))
    return 1;

  return 0;
}
