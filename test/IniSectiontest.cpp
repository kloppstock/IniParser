#include "test.hpp"

#include "../include/IniSection.hpp"

int IniSection_at_test() {
  print_test_info("IniSection_at_test");

  std::vector<IniEntry> expected1;
  expected1.push_back(IniEntry("name1", "value1"));
  expected1.push_back(IniEntry("name3", "value3"));

  std::vector<IniEntry> expected2;
  expected2.push_back(IniEntry("name1", "nothing"));
  expected2.push_back(IniEntry("name3", "value3"));

  IniEntry e1 = IniEntry("name1", "value1");

  IniSection section("section");

  section.insert(e1);
  section.at("name3") = "value3";

  std::vector<IniEntry> entries1 = section.getAllEntries();

  section.at("name1") = "nothing";

  std::vector<IniEntry> entries2 = section.getAllEntries();

  if (entries1 != expected1) {
    printf("Vectors aren't equal!\n");
    return 1;
  }

  if (entries2 != expected2) {
    printf("Vectors aren't equal!\n");
    return 1;
  }

  return 0;
}

int IniSection_clean_test() {
  print_test_info("IniSection_clean_test");

  std::vector<IniEntry> expected1;
  expected1.push_back(IniEntry("name1", "value1"));
  expected1.push_back(IniEntry("name3", ""));

  std::vector<IniEntry> expected2;
  expected2.push_back(IniEntry("name1", "value1"));

  IniEntry e1 = IniEntry("name1", "value1");

  IniSection section("section");

  section.insert(e1);
  section.at("name3") = "";

  std::vector<IniEntry> entries1 = section.getAllEntries();

  section.clean();

  std::vector<IniEntry> entries2 = section.getAllEntries();

  if (entries1 != expected1) {
    printf("Vectors aren't equal!\n");
    return 1;
  }

  if (entries2 != expected2) {
    printf("Vectors aren't equal!\n");
    return 1;
  }

  return 0;
}

int IniSection_erase_test() {
  print_test_info("IniSection_erase_test");

  std::vector<IniEntry> expected1;
  expected1.push_back(IniEntry("name1", "value1"));
  expected1.push_back(IniEntry("name3", "value3"));

  std::vector<IniEntry> expected2;
  expected2.push_back(IniEntry("name3", "value3"));

  IniEntry e1 = IniEntry("name1", "value1");

  IniSection section("section");

  section.insert(e1);
  section.at("name3") = "value3";

  std::vector<IniEntry> entries1 = section.getAllEntries();

  section.erase("name1");

  std::vector<IniEntry> entries2 = section.getAllEntries();

  if (entries1 != expected1) {
    printf("Vectors aren't equal!\n");
    return 1;
  }

  if (entries2 != expected2) {
    printf("Vectors aren't equal!\n");
    return 1;
  }

  return 0;
}
