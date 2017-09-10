# Features

The goal of this project was a feature rich yet light weight and robust .INI file parser which has the ability to read and write .INI files.

One feature is the the way of handling UTF-8 sequences, which can be chosen by the programmer at runtime:
* support it (INI_UTF_8_MODE_ALLOW)
* don't support it (INI_UTF_8_FORBID)
* support it if a BOM(=byte order mark) is present (INI_UTF_8_MODE_ALLOW_ON_BOM)
* escape all UTF-8 sequences (INI_UTF_8_MODE_ESCAPE)

In addition to this the programmer may choose an equals and a comment character to allow the parsing of slightly different formats.

Multi line values can be disabled by defining the INI_FORBID_MULTILINE in the [ini_config.h](/include/ini_config.h) file or through the compiler.

This parser also has the option to only use static memory. To enable this define the number of bytes for the static memory in the [ini_config.h](/include/ini_config.h) file or through the compiler using INI_STATIC_MEMORY_SIZE.

If an parsing error occurs, the parser will provide the line number. On any other error an error type is provided.