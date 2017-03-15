#ifndef INI_PARSER_H
#define INI_PARSER_H

//prepare C header for C++
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
	
#define INI_FORBID_UTF8       0
#define INI_ALLOW_UTF8        1
#define INI_ALLOW_UTF8_ON_BOM 2

	typedef int (*ini_event)(const char* section, const char* name, const char* value, void* data_structure);

	int ini_read_file(const char* path, ini_event handler, void* data_structure, char comment_char, char equals_char, int allow_utf8);
	int ini_read(const char* file_content, size_t file_size, ini_event handler, void* data_structure, char comment_char, char equals_char, int has_bom);

	int ini_write_file(/*fix this*/);

	char hex_digit_to_binary(char c);
	uint32_t uctoutf8(uint32_t code);
	int is_spacing(char c);
	int is_newline(char c);
	int is_whitespace(char c);
	int is_special_character(char c, char comment_char, char equals_char);
	int is_utf8_part(char c);
	int is_printable_char(char c);
	size_t lskipws(const char* str);
	size_t rskipws(const char* str);
	/*TODO remove this function completely:*/size_t max_size_t(size_t f, size_t s);
	/*TODO: remove this completely: int read_hex_from_string(char* str, size_t* pos);*/
	uint32_t read_unicode_from_string(const char* str, size_t* pos);
	int has_non_whitespace_before_newline(const char* str, size_t* pos);
	char unescape_char(char c, char comment, char equals);
	char* stripws(char* str);
	int unescape_string(char* str, size_t len,  char comment, char equals);
	size_t fsize(FILE* stream, int* error);
	void free3(void* data0, void* data1, void* data2);
	size_t count_newlines(const char* str, size_t* pos);
	size_t skip_comment(const char* data, size_t* pos);
	size_t skip_whitespaces(const char* data, size_t* pos, unsigned int* line_number);
	int contains_utf8(const char* str);
	int contains_escape_characters(const char* str, char comment_char, char equals_char);
	int read_name(char* name, const char* str, size_t start, size_t end, char comment_char, char equals_char, int allow_utf8);
	int remove_quotes(char* str);
	int read_value(char* value, const char* str, size_t start, size_t end, char comment_char, char equals_char, unsigned int* line_number, int allow_utf8);
    /*TODO remove this function completely:*/int validate(const char* data, int allow_spacing, int allow_newline, char comment, char equals);
	int check_for_BOM(FILE* stream);
	
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // INI_PARSER_H
