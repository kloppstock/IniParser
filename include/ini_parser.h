#ifndef INI_PARSER_H
#define INI_PARSER_H

// prepare C header for C++
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// uncomment this to use static memory instead of dynamic
//#define STATIC_MEMORY_SIZE 10000

#define INI_FORBID_UTF8 0
#define INI_ALLOW_UTF8 1
#define INI_ALLOW_UTF8_ON_BOM 2

#define INI_UTF8_MODE_FORBID 0
#define INI_UTF8_MODE_ALLOW 1
#define INI_UTF8_MODE_ALLOW_WITH_BOM 2
#define INI_UTF8_MODE_ESCAPE 3

struct ini_file {
  int utf8_mode;
  FILE *file;
  char equals;
  char comment;
};

typedef int (*ini_event)(const char *section, const char *name,
                         const char *value, void *data_structure);

// read methods
int ini_read_file(const char *path, ini_event handler, void *data_structure,
                  char comment, char equals, int allow_utf8);
int ini_read(const char *str, size_t file_size, ini_event handler,
             void *data_structure, char comment, char equals, int has_bom);

// write methods
int ini_open_file(struct ini_file *file, const char *path, char equals,
                  char comment, int utf8_mode);
int ini_close_file(struct ini_file *file);
int ini_write_section(const struct ini_file *file, const char *section);
int ini_write_name_value(const struct ini_file *file, const char *name,
                         const char *value);
int ini_verify_utf8(const char *str, int allow_utf8);

// helper functions
int write_value(const struct ini_file *file, const char *value);
size_t get_utf8_len(char c);
uint16_t binary_to_hex_digits(char c);
uint32_t utf8touc(uint32_t code);
char hex_digit_to_binary(char c);
uint32_t uctoutf8(uint32_t code);
int is_spacing(char c);
int is_newline(char c);
int is_whitespace(char c);
int is_special_character(char c, char comment, char equals);
int is_utf8_part(char c);
int is_printable_char(char c);
size_t lskipws(const char *str);
size_t rskipws(const char *str);
uint32_t read_unicode_from_string(const char *str, size_t *pos);
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

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // INI_PARSER_H
