#include "test.hpp"

#include "../include/IniParser.hpp"
#include "assert.h"

int IniParser_load_test() {
  print_test_info("IniParser_load_test");

  IniFile file1;
  IniFile file2;

  try {
    file1 = IniParser::load("IniParser_load_test/input1.ini");
  } catch (IniParser::ParserException e) {
    std::cerr
        << "ERROR: The load function threw an unexpected ParserException!\n";
    std::cerr << IniParser::getErrorString(IniParser::getError()) << "\n";

    return 1;
  }

  try {
    file2 = IniParser::load("IniParser_load_test/input2.ini");
    std::cerr << "ERROR: No ParserException was thrown!\n";
    return 1;

  } catch (IniParser::ParserException e) {
    if (IniParser::getError() != 3) {
      std::cerr << "ERROR: Expected error code " << 3 << " but was error code "
                << IniParser::getError() << "\n";
      return 1;
    }
  }

  IniFile expected;
  expected[""]["key"] = "value without a section";
  expected["section_string"]["key0"] = "value0";
  expected["section_string"]["key1"] = "value10, value11, value12";
  expected["section_string"]["key3"] = "";
  expected["section_string"]["key4"] = "";
  expected["another_section"]["key"] = "value";

  if (file1 != expected) {
    std::cerr << "ERROR: File was parsed incorrectly\n";
    return 1;
  }

  return 0;
}

int IniParser_store_test() {
  print_test_info("IniParser_store_test");

  IniFile expected;
  expected[""]["key"] = "value without a section";
  expected["section_string"]["key0"] = "value0";
  expected["section_string"]["key1"] = "value10, value11, value12";
  expected["section_string"]["key3"] = "";
  expected["section_string"]["key4"] = "";
  expected["another_section"]["key"] = "value";

  expected.setComment("Here is the file comment");
  expected[""].setComment("This is the section without name");
  expected["section_string"]["key4"].setComment(
      "This is a comment for a specific key - value pair");

  IniFile file1;
  IniFile file2;
  IniFile file3;

  try {
    IniParser::store(expected, "IniParser_store_test/result1.ini");
  } catch (IniParser::ParserException e) {
    std::cerr
        << "ERROR: The store function threw an unexpected ParserException!\n";
    std::cerr << IniParser::getErrorString(IniParser::getError()) << "\n";

    return 1;
  }

  try {
    IniParser::store(expected, "/no/where.ini");
    std::cerr << "ERROR: No ParserException was thrown!\n";
    return 1;

  } catch (IniParser::ParserException e) {
    if (IniParser::getError() != IniParser::WRITE_ERROR) {
      std::cerr << "ERROR: Expected error code " << IniParser::WRITE_ERROR
                << " but was error code " << IniParser::getError() << "\n";
      return 1;
    }
  }

  try {
    file3 = IniParser::load("IniParser_store_test/result1.ini");
  } catch (IniParser::ParserException e) {
    std::cerr
        << "ERROR: The load function threw an unexpected ParserException!\n";
    std::cerr << IniParser::getErrorString(IniParser::getError()) << "\n";

    return 1;
  }

  if (assert_equals_file("IniParser_store_test/result1.ini",
                         "IniParser_store_test/expected1.ini", 1000)) {
    std::cerr << "ERROR: File was parsed incorrectly\n";
    return 1;
  }

  // remove comments

  expected.setComment("");
  expected[""].setComment("");
  expected["section_string"]["key4"].setComment("");

  if (file3 != expected) {
    std::cerr << "ERROR: Stored and loaded files are not equal!\n";
    return 1;
  }

  return 0;
}

int IniParser_getErrorMessage_test() {
  print_test_info("IniParser_getErrorMessage_test");

  IniFile file1;

  try {
    file1 = IniParser::load("IniParser_load_test/input1.ini");
  } catch (IniParser::ParserException e) {
    std::cerr
        << "ERROR: The load function threw an unexpected ParserException!\n";
    std::cerr << IniParser::getErrorString(IniParser::getError()) << "\n";

    return 1;
  }

  int input1 = -3;
  int input2 = -2;
  int input3 = -1;
  int input4 = 0;
  int input5 = 15;

  std::string expected1 = "Invalid string found!";
  std::string expected2 =
      "Error while writing file: IniParser_load_test/input1.ini!";
  std::string expected3 =
      "Error while reading file: IniParser_load_test/input1.ini!";
  std::string expected4 = "No errors detected.";
  std::string expected5 =
      "Error in file IniParser_load_test/input1.ini on line 15!";

  std::string actual1 = IniParser::getErrorString(input1);
  std::string actual2 = IniParser::getErrorString(input2);
  std::string actual3 = IniParser::getErrorString(input3);
  std::string actual4 = IniParser::getErrorString(input4);
  std::string actual5 = IniParser::getErrorString(input5);

  if (assert_equals_str(expected1.c_str(), actual1.c_str()))
    return 1;
  if (assert_equals_str(expected2.c_str(), actual2.c_str()))
    return 1;
  if (assert_equals_str(expected3.c_str(), actual3.c_str()))
    return 1;
  if (assert_equals_str(expected4.c_str(), actual4.c_str()))
    return 1;
  if (assert_equals_str(expected5.c_str(), actual5.c_str()))
    return 1;

  return 0;
}
