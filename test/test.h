#ifndef __TEST_H__
#define __TEST_H__

#include <math.h>

#include "assert.h"

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
int max_size_t_test();
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
int ini_read_test();
int check_for_BOM_test();
int ini_read_file_test();

#endif //__TEST_H__
