#ifndef INI_HELPER_H
#define INI_HELPER_H

// prepare C header for C++
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "ini_config.h"

struct two_bytes {
  unsigned char bytes[2];
};

struct four_bytes {
  unsigned char bytes[4];
};
	
// helper functions
size_t get_utf8_len(char c);
struct two_bytes binary_to_hex_digits(char c);
struct four_bytes utf8touc(struct four_bytes code);
char hex_digit_to_binary(char c);
struct four_bytes uctoutf8(struct four_bytes code);
int is_spacing(char c);
int is_newline(char c);
int is_whitespace(char c);
int is_special_character(char c, char comment, char equals);
int is_utf8_part(char c);
int is_printable_char(char c);
size_t lskipws(const char *str);
size_t rskipws(const char *str);
struct four_bytes read_unicode_from_string(const char *str, size_t *pos);
int has_non_whitespace_before_newline(const char *str, size_t *pos);
char unescape_char(char c, char comment, char equals);
char *stripws(char *str);
int unescape_string(char *str, size_t len, char comment, char equals);
size_t fsize(FILE *stream, int *error);
void free3(void *data0, void *data1, void *data2);
unsigned int count_newlines(const char *str, size_t *pos);
size_t skip_comment(const char *data, size_t *pos);
size_t skip_whitespaces(const char *data, size_t *pos,
                        unsigned int *line_number);
int contains_utf8(const char *str);
int contains_escape_characters(const char *str, char comment, char equals);
int read_name(char *name, const char *str, size_t start, size_t end,
              char comment, char equals, int allow_utf8);
int remove_quotes(char *str);
int read_value(char *value, const char *str, size_t start, size_t end,
               char comment, char equals, unsigned int *line_number,
               int allow_utf8);
int check_for_BOM(FILE *stream);
int write_value(const struct ini_file *file, const char *value);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // INI_HELPER_H
