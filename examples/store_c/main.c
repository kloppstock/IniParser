#include <stdio.h>

#include "../../include/ini_parser.h"

int main() {
  struct ini_file file;

  // opening the file
  if (!ini_open_file(&file, "store_example.ini", '=', ';',
                     INI_UTF8_MODE_FORBID)) {
    puts("Something went wrong while opening the file!");
    return -1;
  }

  // writing a comment at the beginning of the file
  if (!ini_write_comment(
          &file, "This .ini file was created by the C store example.")) {
    puts("Something went wrong while writing a comment!");
    return -1;
  }

  // writing the version information
  if (!ini_write_name_value(&file, "Version", "4.4.1")) {
    puts("Something went wrong while writing a key value pair!");
    return -1;
  }

  // writing a section with a comment and two key value pairs
  if (!ini_write_section(&file, "Screen")) {
    puts("Something went wrong while writing a section!");
    return -1;
  }

  if (!ini_write_comment(&file, "Screen settings")) {
    puts("Something went wrong while writing a comment!");
    return -1;
  }

  if (!ini_write_name_value(&file, "Resolution_X", "640")) {
    puts("Something went wrong while writing a key value pair!");
    return -1;
  }

  if (!ini_write_name_value(&file, "Resolution_Y", "480")) {
    puts("Something went wrong while writing a key value pair!");
    return -1;
  }

  // writing another section with a single key value pair and another comment
  if (!ini_write_section(&file, "Controls")) {
    puts("Something went wrong while writing a section!");
    return -1;
  }

  if (!ini_write_comment(&file, "Path for control config file")) {
    puts("Something went wrong while writing a comment!");
    return -1;
  }

  if (!ini_write_name_value(&file, "ConfigPath",
                            "/somewhere/on/the/harddrive")) {
    puts("Something went wrong while writing a key value pair!");
    return -1;
  }

  // closing the file again
  if (!ini_close_file(&file)) {
    puts("Something went wrong while closing the file!");
    return -1;
  }

  return 0;
}
