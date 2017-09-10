/* Copyright (c) 2017 Jonas Schenke
 *
 * This file is part of IniParser.
 *
 * IniParser is licensed under the MIT license. Form more information see
 * LICENSE.
 */

#ifndef TEST_HPP
#define TEST_HPP

#include <math.h>

#include "assert.h"

// test helper
size_t load_file(char buffer[1000], const char *path);
void print_test_info(const char *test);

// helper tests
int hex_digit_to_binary_test();
int uctoutf8_test();
int is_spacing_test();
int is_newline_test();
int is_whitespace_test();
int is_special_character_test();
int is_utf8_part_test();
int is_printable_char_test();
int lskipws_test();
int rskipws_test();
int read_unicode_from_string_test();
int has_non_whitespace_before_newline_string_test();
int unescape_char_test();
int stripws_test();
int unescape_string_test();
int fsize_test();
int count_newlines_test();
int skip_comment_test();
int skip_whitespaces_test();
int contains_utf8_test();
int contains_escape_characters_test();
int read_name_test();
int remove_quotes_test();
int read_value_test();
int check_for_BOM_test();
int get_utf8_len_test();
int binary_to_hex_digits_test();
int utf8touc_test();
int write_value_test();

// reader tests
int ini_read_test();
int ini_read_file_test();

// writer tests
int ini_open_and_close_file_test();
int ini_write_section_test();
int ini_write_name_value_test();
int ini_verify_utf8_test();
int ini_write_comment_test();

// IniSection tests
int IniSection_at_test();
int IniSection_clean_test();
int IniSection_erase_test();

// IniFile tests
int IniFile_at_test();
int IniFile_clean_test();
int IniFile_erase_test();

// IniParser tests
int IniParser_load_test();
int IniParser_store_test();
int IniParser_getErrorMessage_test();

#endif // TEST_HPP
