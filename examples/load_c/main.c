#include <stdio.h>
#include <string.h>

#include "../../include/ini_parser.h"

// data structure for storing the parsed values
struct data_structure {
  char version[16];
  int res_x, res_y;
  char config_path[100];
};

// handler for data handling of the parsed data
int handler(const char *section, const char *name, const char *value,
            void *raw_data) {
  struct data_structure *data = (struct data_structure *)raw_data;

  // converting and storing values
  if (!strcmp(section, "") && !strcmp(name, "Version"))
    strcpy(data->version, value);
  else if (!strcmp(section, "Screen") && !strcmp(name, "Resolution_X"))
    data->res_x = atoi(value);
  else if (!strcmp(section, "Screen") && !strcmp(name, "Resolution_Y"))
    data->res_y = atoi(value);
  else if (!strcmp(section, "Controls") && !strcmp(name, "ConfigPath"))
    strcpy(data->config_path, value);
  else
    return 0;

  return 1;
}

int main(void) {
  struct data_structure data;

  // read file
  int error = ini_read_file("load_example.ini", handler, &data, ';', '=',
                            INI_UTF8_MODE_ALLOW);

  // check for errors
  if (error) {
    printf("Something went wrong while parsing the file (error code: %d)!\n",
           error);
    return -1;
  }

  // optionally check encoding
  error = !ini_verify_utf8(data.version, INI_UTF8_MODE_ALLOW);
  if (error) {
    printf("Invalid encoding detected!\n");
    return -1;
  }

  error = !ini_verify_utf8(data.config_path, INI_UTF8_MODE_ALLOW);
  if (error) {
    printf("Invalid encoding detected!\n");
    return -1;
  }

  // printing values
  printf("Version: %s\n", data.version);
  printf("Resolution: %dx%d\n", data.res_x, data.res_y);
  printf("Config path: %s\n", data.config_path);

  return 0;
}
