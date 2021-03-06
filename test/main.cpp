/* Copyright (c) 2017 Jonas Schenke
 *
 * This file is part of IniParser.
 *
 * IniParser is licensed under the MIT license. Form more information see
 * LICENSE.
 */

#include "test.hpp"

int main(void) {
  int res = 0;
  int num_test = 0;

  // helper tests
  res += hex_digit_to_binary_test();
  ++num_test;
  res += uctoutf8_test();
  ++num_test;
  res += is_spacing_test();
  ++num_test;
  res += is_newline_test();
  ++num_test;
  res += is_whitespace_test();
  ++num_test;
  res += is_special_character_test();
  ++num_test;
  res += is_utf8_part_test();
  ++num_test;
  res += is_printable_char_test();
  ++num_test;
  res += lskipws_test();
  ++num_test;
  res += rskipws_test();
  ++num_test;
  res += read_unicode_from_string_test();
  ++num_test;
  res += has_non_whitespace_before_newline_string_test();
  ++num_test;
  res += unescape_char_test();
  ++num_test;
  res += stripws_test();
  ++num_test;
  res += unescape_string_test();
  ++num_test;
  res += fsize_test();
  ++num_test;
  res += count_newlines_test();
  ++num_test;
  res += skip_comment_test();
  ++num_test;
  res += skip_whitespaces_test();
  ++num_test;
  res += contains_utf8_test();
  ++num_test;
  res += contains_escape_characters_test();
  ++num_test;
  res += read_name_test();
  ++num_test;
  res += remove_quotes_test();
  ++num_test;
  res += read_value_test();
  ++num_test;
  res += check_for_BOM_test();
  ++num_test;
  res += get_utf8_len_test();
  ++num_test;
  res += binary_to_hex_digits_test();
  ++num_test;
  res += utf8touc_test();
  ++num_test;
  res += write_value_test();
  ++num_test;

  // read tests
  res += ini_read_file_test();
  ++num_test;
  res += ini_read_test();
  ++num_test;

  // write tests
  res += ini_open_and_close_file_test();
  ++num_test;
  res += ini_write_section_test();
  ++num_test;
  res += ini_write_name_value_test();
  ++num_test;
  res += ini_verify_utf8_test();
  ++num_test;
  res += ini_write_comment_test();
  ++num_test;

  // IniSection tests
  res += IniSection_at_test();
  ++num_test;
  res += IniSection_clean_test();
  ++num_test;
  res += IniSection_erase_test();
  ++num_test;

  // IniFile tests
  res += IniFile_at_test();
  ++num_test;
  res += IniFile_clean_test();
  ++num_test;
  res += IniFile_erase_test();
  ++num_test;

  // IniParser tests
  res += IniParser_load_test();
  ++num_test;
  res += IniParser_store_test();
  ++num_test;
  res += IniParser_getErrorMessage_test();
  ++num_test;

  printf("Number of tests: %d\nNumber of failed tests: %d\n", num_test, res);
  return res;
}
