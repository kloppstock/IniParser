#ifndef __ASSERT_H__
#define __ASSERT_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

int assert_equals_int(const int32_t expected, const int32_t actual);
int assert_equals_unsigned_int(const uint32_t expected, const uint32_t actual);
int assert_equals_str(const char* expected, const char* actual);
int assert_equals_mem(const void* expected, const void* actual, size_t len);
int assert_equals_file(const char* expected_path, const char* actual_path, size_t max_size);

#endif //__ASSERT_H__
