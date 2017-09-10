# Programming interface

## Reading file

### C

For reading .INI files a callback function and a data structure is needed. The callback function has to return an integer (0 = error, anything else = success) to report the status.
The data structure is needed to store parsed information. An example (taken from the [C load example](/examples/load_c/main.c)) would be:

* data structure:
```
struct data_structure {
  char version[16];
  int res_x, res_y;
  char config_path[100];
};
```

* callback function:
```
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
```

If these prequisites are present and the other needed parameters (comment character, equals character and UTF-8 mode), the ```ini_read_file()``` function can be called:
```
int error = ini_read_file("load_example.ini", handler, &data, ';', '=', INI_UTF8_MODE_ALLOW);
```

If the file was already loaded into a buffer it can be parsed using the ```ini_open_file()``` function which takes a buffer pointer and a size instead of the path.

### C++

The C++ wrapper already has the callback function and the data structure integrated. The ```IniParser::load()``` function returns an ```IniFile``` objects, which holds all the information.

If the parsing fails it is possible that the ```IniParser::load()``` function throws an IniParser::ParserException which needs to be caught.

It is also optionally possible to provide comment character, equals character and UTF-8 mode but the default values should be fine most of the time.

All in all, loading a file would look something like this (taken from the [C++ load example](/examples/load_c++/main.cpp)):
```
IniFile file;

// try loading file
try {
  file = IniParser::load("load_example.ini");
} catch (IniParser::ParserException e) {
  std::cerr << "Error while loading file!\n"
}
```

If an error occurs (an exception is thrown), the error code can be obtained using ```IniParser::getError()``` and the accompanying error string can be obtained using ```IniParser::getErrorString(IniParser::getError())```.

## Writing files

### C

Writing files requires some steps more than reading.

First a file needs to be opened and initialized with the parameters (taken from the [C store example](/examples/store_c/main.c)):
```
if (!ini_open_file(&file, "store_example.ini", '=', ';',
                     INI_UTF8_MODE_FORBID)) {
  puts("Something went wrong while opening the file!");
  return -1;
}
```

After this is done it is possible to write:
* comments:
```
if (!ini_write_comment(
          &file, "This .ini file was created by the C store example.")) {
  puts("Something went wrong while writing a comment!");
  return -1;
}
```

* sections:
```
if (!ini_write_section(&file, "Section")) {
  puts("Something went wrong while writing a section!");
  return -1;
}
```

* key value pairs:
```
if (!ini_write_name_value(&file, "Name", "Value")) {
  puts("Something went wrong while writing a key value pair!");
  return -1;
}
```

The information is written in the same order as it is executed. Moving a ```ini_write_name_value()``` after an ```ini_write_section()``` moves the key value pair to this section.

At the end the file needs to be closed:
```
if (!ini_close_file(&file)) {
  puts("Something went wrong while closing the file!");
  return -1;
}
```

### C++

Storing an .INI file in C++ is a bit easier then in C. 

An ```IniFile``` needs to be created which can be used like a map.

This could look something like this (taken from the [C++ store example](/examples/store_c++/main.cpp)):
```
IniFile file;

file["Screen"].setComment("Screen settings");
file["Screen"]["Resolution_X"] = 640;
file["Screen"]["Resolution_Y"] = 480;
```

This data also can be easily modified:
```
if (file["Screen"]["Resolution_X"].getInt() < 1920 &&
    file["Screen"]["Resolution_Y"].getInt() < 1080) {
  // changing values
  file["Screen"]["Resolution_X"] = 1920;
  file["Screen"]["Resolution_Y"] = 1080;
}
```

After the ```IniFile``` is filled, it can be stored:
```
try {
  IniParser::store(file, "store_example.ini");
} catch (IniParser::ParserException e) {
  std::cerr << "Error while storing file!\n";
}
```

Please note, that the ```IniParser::store()``` function might throw an ```IniParser::ParserException``` on failure.

The error handling can be done using ```IniParser::getErrorString(IniParser::getError())```.

## (Optionally) verifying UTF-8

If it is desired to verify the read or to be written values, it can be done using the ```ini_verify_utf8()``` function like this:
```
int error = !ini_verify_utf8(data.config_path, INI_UTF8_MODE_ALLOW);
if (error) {
  printf("Invalid encoding detected!\n");
  return -1;
}
```
