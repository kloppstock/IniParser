cmake_minimum_required (VERSION 3.1)

project(IniParser)
set(CMAKE_BUILD_Type Release)
set(CMAKE_CXX_STANDARD 11)
set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)

add_library(${PROJECT_NAME} SHARED src/ini_helper.c src/ini_reader.c src/ini_writer.c src/IniParser.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(FILES include/ini_config.h include/ini_parser.h include/IniParser.hpp include/IniFile.hpp include/IniEntry.hpp include/IniSection.hpp DESTINATION include/${PROJECT_NAME})
