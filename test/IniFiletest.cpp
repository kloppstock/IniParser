#include "test.hpp"

#include "../include/IniFile.hpp"

#include <iostream>
void printStuff(std::vector<IniSection> stuff) {
  std::cout << stuff.size() << " sections\n";
  for (IniSection section : stuff) {
    std::cout << section.getAllEntries().size() << " entries\n";
    for (IniEntry entry : section.getAllEntries()) {
      std::cout << section.getName() << "." << entry.getKey() << " = "
                << entry.getString() << "\n";
    }
    std::cout << ";\n";
  }
  std::cout << "\n";
}

int IniFile_at_test() {
  print_test_info("IniFile_at_test");

  IniEntry entry1("name1", "value1");
  IniEntry entry2("name3", "");

  IniSection section1("section1");

  section1.insert(entry1);
  section1.insert(entry2);

  IniSection section2("section2");

  section2.insert(entry2);

  IniFile file1;
  file1.insert(section1);
  file1.insert(section2);

  IniFile file2;
  file2.insert(section2);

  IniFile file3;
  file3.insert(IniSection("section"));

  std::vector<IniSection> result1 = file1.getAllSections();

  std::vector<IniSection> result2 = file2.getAllSections();

  std::vector<IniSection> result3 = file3.getAllSections();

  std::vector<IniSection> expected1;
  expected1.push_back(section1);
  expected1.push_back(section2);

  std::vector<IniSection> expected2;
  expected2.push_back(section2);

  std::vector<IniSection> expected3;
  expected3.push_back(IniSection("section"));

  if (result1 != expected1) {
    printf("Vectors aren't equal!\n");
    return 1;
  }

  if (result2 != expected2) {
    printf("Vectors aren't equal!\n");
    return 1;
  }

  if (result3 != expected3) {
    printf("Vectors aren't equal!\n");
    return 1;
  }

  return 0;
}

int IniFile_clean_test() {
  print_test_info("IniSection_clean_test");

  IniEntry entry1("name1", "value1");
  IniEntry entry2("name3", "");

  IniSection section1("section1");

  section1.insert(entry1);
  section1.insert(entry2);

  IniSection section2("section2");

  section2.insert(entry2);

  IniFile file1;
  file1.insert(section1);
  file1.insert(section2);

  IniFile file2;
  file2.insert(section2);

  IniFile file3;
  file3.insert(IniSection("section"));

  file1.clean();

  std::vector<IniSection> result1 = file1.getAllSections();

  file2.clean();

  std::vector<IniSection> result2 = file2.getAllSections();

  file3.clean();

  std::vector<IniSection> result3 = file3.getAllSections();

  section1.clean();
  section2.clean();

  std::vector<IniSection> expected1;
  expected1.push_back(section1);

  std::vector<IniSection> expected2;

  std::vector<IniSection> expected3;

  if (result1 != expected1) {
    printf("Vectors aren't equal!\n");
    return 1;
  }

  if (result2 != expected2) {
    printf("Vectors aren't equal!\n");
    return 1;
  }

  if (result3 != expected3) {
    printf("Vectors aren't equal!\n");
    return 1;
  }

  return 0;
}

int IniFile_erase_test() {
  print_test_info("IniSection_erase_test");

  IniEntry entry1("name1", "value1");
  IniEntry entry2("name3", "");

  IniSection section1("section1");

  section1.insert(entry1);
  section1.insert(entry2);

  IniSection section2("section2");

  section2.insert(entry2);

  IniFile file1;
  file1.insert(section1);
  file1.insert(section2);

  file1.erase("section2");

  std::vector<IniSection> result1 = file1.getAllSections();

  std::vector<IniSection> expected1;
  expected1.push_back(section1);

  if (result1 != expected1) {
    printf("Vectors aren't equal!\n");
    return 1;
  }

  return 0;
}
