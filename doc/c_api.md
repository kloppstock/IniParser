# C API Reference

Table of contents:
* [ini_read](#ini_read)
* [ini_read_file](#ini_read_file)
* [ini_open_file](#ini_open_file)
* [ini_close_file](#ini_close_file)
* [ini_write_section](#ini_write_section)
* [ini_verify_utf8](#ini_verify_utf8)
* [ini_write_name_value](#ini_write_name_value)
* [ini_write_comment](#ini_write_comment)

## ini_read()

**Definition:**
```
int ini_read(const char *str, size_t file_size, ini_event handler, void *data_structure, char comment, char equals, int allow_utf8);
```

**Description:**
Parses a string with a given file size and calls a handler function when a key value pair is read.
The handler should return 0 on failure and a non 0 value on success.

**Parameters:**
* string
* file size
* handler function
* pointer to user data structure
* comment char
* equals char
* allow UTF-8 flag

**Return:**
0 on success; -1 on memory error and the currently parsed line number on a parsing error

## ini_read_file()

**Definition:**
```
int ini_read_file(const char *path, ini_event handler, void *data_structure, char comment, char equals, enum ini_utf8_mode allow_utf8);
```

**Description:**
Reads an .ini file and parses it using the user defined handler function and the data structure.
The handler should return 0 on failure and a non 0 value on success.

**Parameters:**
* string
* file size
* handler function
* pointer to user data structure
* comment char
* equals char
* allow UTF-8 flag

**Return:**
0 on success; -1 on memory error and the currently parsed line number on a parsing error

## ini_open_file()

**Definition:**
```
int ini_open_file(struct ini_file *file, const char *path, char equals, char comment, enum ini_utf8_mode utf8_mode);
```

**Description:**
Opens an .INI file for write and writes the byte order mark if needed.

**Parameters:**
* file pointer
* path
* equals character
* comment character
* UTF-8 mode (INI_UTF8_MODE_FORBID, INI_UTF8_MODE_ALLOW, INI_UTF8_MODE_ALLOW_WITH_BOM, INI_UTF8_MODE_ESCAPE)

**Return:**
1 on success; 0 on failure

## ini_close_file()

**Definition:**
```
int ini_close_file(struct ini_file *file);
```

**Description:**
Closes an .INI file. The file pointer must be valid!

**Parameters:**
* file pointer

**Return:**
1 on success; 0 on failure

## ini_write_section()

**Definition:**
```
int ini_write_section(const struct ini_file *file, const char *section);
```

**Description:**
Writes a section name to a file. The section and file pointers must be valid! 
The section name mustn't include whitespace character, other escape characters!
The section name also mustn't be empty!

**Parameters:**
* file pointer
* section name

**Return:**
1 on success; 0 on failure

## ini_verify_utf8()

**Definition:**
```
int ini_verify_utf8(const char *str, int allow_utf8);
```

**Description:**
Verifies that a given string consists only of valid UTF-8 sequences or doesn't contain any UTF-8 sequence.

**Parameters:**
* string
* flag whether to allow UTF-8 or not

**Return:**
1 if the string is valid; 0 otherwise

## ini_write_name_value()

**Definition:**
```
int ini_write_name_value(const struct ini_file *file, const char *name, const char *value);
```

**Description:**
Writes a name - value pair to a file. The name and file pointers mustn't be NULL!

**Parameters:**
* file pointer
* name
* value

**Return:**
1 on success; 0 on failure

## ini_write_comment()

**Definition:**
```
int ini_write_comment(const struct ini_file *file, const char *comment);
```

**Description:**
Writes a comment into an .INI file. The comment string and file pointers mustn't be NULL!

**Parameters:**
* file
* comment string

**Return:**
1 on success; 0 on failure
