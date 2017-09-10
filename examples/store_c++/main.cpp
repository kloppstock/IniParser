/* Copyright (c) 2017 Jonas Schenke
 *
 * This file is part of IniParser.
 *
 * IniParser is licensed under the MIT license. Form more information see
 * LICENSE.
 */

#include <iostream>

#include "../../include/IniParser.hpp"

int main() {

  IniFile file;
  // inserting a comment fore the file
  file.setComment("This .ini file was created by the C++ store example.");

  // insert and modify values
  file[""]["Version"] = 4.4;

  // insert Screen section with a comment
  file["Screen"].setComment("Screen settings");
  file["Screen"]["Resolution_X"] = 640;
  file["Screen"]["Resolution_Y"] = 480;

  // insert Controls section with a comment for the ConfigPath
  file["Controls"]["ConfigPath"].setComment("Path for control config file");
  file["Controls"]["ConfigPath"] = "/somewhere/on/the/harddrive";

  if (file["Screen"]["Resolution_X"].getInt() < 1920 &&
      file["Screen"]["Resolution_Y"].getInt() < 1080) {
    // changing values
    file["Screen"]["Resolution_X"] = 1920;
    file["Screen"]["Resolution_Y"] = 1080;
  }

  // storing the .ini file

  try {
    IniParser::store(file, "store_example.ini");
  } catch (IniParser::ParserException e) {
    std::cerr << "Error while storing file!\n";
  }

  // printing errors
  std::cout << IniParser::getErrorString(IniParser::getError()) << "\n";

  return 0;
}
