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

#define INI_PARSE_ERROR              1

#define INI_RETURN_SUCCESS           0
#define INI_RETURN_FILE_ERROR       -1
#define INI_RETURN_MEMORY_ERROR     -2
#define INI_RETURN_UNKNOWN_ERROR    -3

//#define INI_IGNORE_INVALID_ESCAPED_CHARACTERS

typedef int (*ini_event)(char* section, char* name, char* value, void* data_structure);

int ini_read_file(char* path, ini_event handler, void* data_structure, char comment_char = ';', char equals_char = '=');
int ini_write_file(/*fix this*/);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // INI_PARSER_H
