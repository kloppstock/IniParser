/* Copyright (c) 2017 Jonas Schenke
 *
 * This file is part of IniParser.
 *
 * IniParser is licensed under the MIT license. Form more information see
 * LICENSE.
 */

#ifndef ASSERT_H
#define ASSERT_H

// prepare C header for C++
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int assert_equals_int(const int32_t expected, const int32_t actual);
int assert_equals_unsigned_int(const uint32_t expected, const uint32_t actual);
int assert_equals_str(const char *expected, const char *actual);
int assert_equals_mem(const void *expected, const void *actual, size_t len);
int assert_equals_file(const char *expected_path, const char *actual_path,
                       size_t max_size);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // ASSERT_H
