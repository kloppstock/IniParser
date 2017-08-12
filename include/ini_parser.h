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

#include "ini_config.h"

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
	int ini_write_comment(const struct ini_file *file, const char *comment);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // INI_PARSER_H
