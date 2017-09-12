# IniParser
An .INI file parser written in C (with a C++ wrapper).

The goal of this project was a feature rich yet light weight and robust .INI file parser which has the ability to read and write .INI files.

A complete feature list can be found in the [doc/features.md](doc/features.md) document. 

## Install

The IniParser can easily be installed using cmake:
```
mkdir build
cd build
cmake ..
sudo make install
```

(Note: Editing the PATH or the LD_EXPORT_PATH variable might be necessary in certain cases,)

Tests can be executed using:
```
cd test
make test
```

(Note: Depending on the installed compiler the CC and CXX variables would need to be modified in the [Makefile](/test/Makefile).)

## Documentation

The documentation can be found in the [doc](/doc/) folder.

This folder contains the [C](/doc/c_api.md) and the [C++](/doc/c++_api.pdf) API references.

In addition to this the [format](/doc/format.md) and the [features](/doc/features.md) are explaned in detail.

The usage is explained in detail in the [usage](/doc/usage.md) document and examples can be found in the [examples](/examples/) folder.

When using the installed library the include path is `#include <IniParser/IniParser.hpp>` (C++) or `#include <IniParser/ini_parser.h>`.
The project needs to be linked to the `IniParser` library e.g. using `-lIniParser` (when using gcc or clang).

## License

This project is licensed under the MIT license which can be found in the [LICENSE](/LICENSE) file.

