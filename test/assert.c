#include "assert.h"

int assert_equals_int(const int32_t expected, const int32_t actual) {
  if(expected == actual)
    return EXIT_SUCCESS;
  fprintf(stderr, "ERROR: expected %d, but was %d!\n", expected, actual);
  return EXIT_FAILURE;
}

int assert_equals_unsigned_int(const uint32_t expected, const uint32_t actual) {
  if(expected == actual)
    return EXIT_SUCCESS;
  fprintf(stderr, "ERROR: expected %u, but was %u!\n", expected, actual);
  return EXIT_FAILURE;
}

int assert_equals_str(const char* expected, const char* actual) {
  if(expected == actual)
    return EXIT_SUCCESS;
  if(expected == NULL || actual == NULL) {
    fprintf(stderr, "ERROR: one string was empty!\n");
    return EXIT_FAILURE;
  }
  if(strcmp(expected, actual) == 0)
    return EXIT_SUCCESS;
  fprintf(stderr, "ERROR: expected \"%s\", but was \"%s\"!\n", expected, actual);
  return EXIT_FAILURE;
}

int assert_equals_mem(const void* expected, const void* actual, size_t len) {
  if(expected == actual)
    return EXIT_SUCCESS;
  if(expected == NULL || actual == NULL) {
    fprintf(stderr, "ERROR: one string was empty!\n");
    return EXIT_FAILURE;
  }
  if(memcmp(expected, actual, len) == 0)
    return EXIT_SUCCESS;
  fprintf(stderr, "ERROR: The memory content is not equal!\n");
  return EXIT_FAILURE;
}

int assert_equals_file(const char* expected_path, const char* actual_path, size_t max_size) {
  if(expected_path == actual_path)
    return EXIT_SUCCESS;
  if(expected_path == NULL || actual_path == NULL) {
    fprintf(stderr, "ERROR: one path is empty!\n");
    return EXIT_FAILURE;
  }
  char* buffer_expected = (char*)malloc(sizeof(char) * max_size);
  if(buffer_expected == NULL) {
    fprintf(stderr, "FATAL ERROR: couldn't allocate enough memory! Please rerun the tests with more memory!\n");
    return EXIT_FAILURE;
  }
  char* buffer_actual = (char*)malloc(sizeof(char) * max_size);
  if(buffer_actual == NULL) {
    fprintf(stderr, "FATAL ERROR: couldn't allocate enough memory! Please rerun the tests with more memory!\n");
    free(buffer_expected);
    return EXIT_FAILURE;
  }
  FILE* f_expected = fopen(expected_path, "r");
  if(f_expected == NULL) {
    fprintf(stderr, "FATAL ERROR: couldn't open file '%s'!\n", expected_path);
    free(buffer_expected);
    free(buffer_actual);
    return EXIT_FAILURE;
  }
  FILE* f_actual = fopen(actual_path, "r");
  if(f_actual == NULL) {
    fprintf(stderr, "FATAL ERROR: couldn't open file '%s'!\n", actual_path);
    free(buffer_expected);
    free(buffer_actual);
    fclose(f_expected);
    return EXIT_FAILURE;
  }
  size_t size_expected = fread(buffer_expected, sizeof(char), max_size, f_expected);
  size_t size_actual = fread(buffer_actual, sizeof(char), max_size, f_actual);
  if(size_expected != size_actual) {
    fprintf(stderr, "ERROR: the files are unequal ('%s', '%s')!\n", expected_path, actual_path);
    free(buffer_expected);
    free(buffer_actual);
    fclose(f_expected);
    fclose(f_actual);
    return EXIT_FAILURE;
  }
  if(memcmp(buffer_expected, buffer_actual, size_expected) == 0) {
    free(buffer_expected);
    free(buffer_actual);
    fclose(f_expected);
    fclose(f_actual);
    return EXIT_SUCCESS;
  }
  fprintf(stderr, "ERROR: the files are unequal ('%s', '%s')!\n", expected_path, actual_path);
  free(buffer_expected);
  free(buffer_actual);
  fclose(f_expected);
  fclose(f_actual);
  return EXIT_FAILURE;
}
