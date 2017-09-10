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

  // try loading file
  try {
    file = IniParser::load("load_example.ini");
  } catch (IniParser::ParserException e) {
    std::cerr << "Error while loading file!\n";
  }

  // printing errors
  std::cout << IniParser::getErrorString(IniParser::getError()) << "\n";

  // reading values
  std::string config_path = file["Controls"]["ConfigPath"].getString();
  std::string version = file[""]["Version"].getString();
  int res_x, res_y;

  // catching conversion exceptions
  try {
    res_x = file["Screen"]["Resolution_X"].getInt();
    res_y = file["Screen"]["Resolution_Y"].getInt();
  } catch (std::invalid_argument e) {
    std::cerr << "Error converting values!\n";

    return -1;
  }

  // printing values
  std::cout << "Version: " << version << "\n";
  std::cout << "Resolution: " << res_x << "x" << res_y << "\n";
  std::cout << "Config path: " << config_path << "\n";

  return 0;
}
