#ifndef INI_CONFIG_H
#define INI_CONFIG_H

// prepare C header for C++
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// uncomment this to use static memory instead of dynamic
//#define INI_STATIC_MEMORY_SIZE 10000

// uncomment this to forbid multiline values
//#define INI_FORBID_MULTILINE

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

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // INI_CONFIG_H
