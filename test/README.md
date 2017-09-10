# Tests

This is the test directory of the project.

## Running the tests

To run the test simply run: ```make test```.
This will compile the parser and all its tests and runs them afterwars.
Use ```make clean``` to remove all generated files.

## What is tested and waht isn't?

Every C function is fully covered by the C test routines.

The C++ test routines only cover more complex routines. Simple getter, setter and wrapper routines are not tested.

## Contents

### assert.c & assert.h

These two files contain functions for a very simple C test suite. The included functions compare an expected value to a provided value and print the difference if there is any. On failure these functions return 0.
There are functions for comparing integers, unsigned integers, strings, files and memory.

### Makefile

This is used to build the tests and clean up.

### testfile & testfile_empty

These files are needed by fsize_test() inside the [ini_helper_test.c](/test/ini_helper_test.cpp) file.

### main.cpp & test.hpp

The [main.cpp](/test/main.cpp) file executes all tests and prints the result. The test function prototypes are defined in the [test.hpp](./test/test.hpp) file.

### ini_helper_test.cpp & ini_write_test.cpp & ini_read_test.cpp

These files contain tests for the C routines.
* [ini_helper_test.cpp](/test/ini_helper_test.cpp): Tests for helper functions
* [ini_write_test.cpp](/test/ini_helper_test.cpp): Tests for write functions
* [ini_read_test.cpp](/test/ini_helper_test.cpp): Tests for read functions

### IniFiletest.cpp & IniParsertest.cpp & IniSectiontest.cpp

These files contain tests for the C++ wrapper routines.
* [IniFiletest.cpp](/test/IniFiletest.cpp): Tests for the IniFile class
* [IniParsertest.cpp](/test/IniParsertest.cpp): Tests for the IniParser class
* [IniSectiontest.cpp](/test/IniSectiontest.cpp): Tests for the IniSection class

### Directories

The directories are named after the tested functions and contain results for these. When executing tests they also contain the results from the test functions.
