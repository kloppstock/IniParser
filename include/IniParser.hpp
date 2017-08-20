#ifndef INIPARSER_H
#define INIPARSER_H

#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "IniSection.hpp"
#include "ini_parser.h"

/**
 * Class for parser exceptions.
 */
class ParserException : public std::exception {
  /**
   * Returns exception string.
   * @return exception string
   */
  virtual const char *what() const throw() { return "Parser exception"; }
} parser_exception;

/*
 * Parser class.
 */
class IniParser {
public:
  /**
   * Constructor. If the path is not empty, the file at the path will be parsed.
   * @param path
   * @param UTF8 mode
   * @param equals character
   * @param comment character
   * @param comment (stored at the beginning of the file when store() is called)
   * @throws a parser_exception and might throw others as well (see std::string
   * constructor)
   */
  IniParser(std::string path = "",
            enum ini_utf8_mode utf8_mode = INI_UTF8_MODE_FORBID,
            char equals = '=', char comment_char = '#',
            std::string comment = "")
      : path(path), comment(comment), error(0),
        file({utf8_mode, NULL, equals, comment_char}) {
    if (path != "")
      if (load(path))
        throw parser_exception;
  }

  /**
   * Loads an INI file at a given path. If this path is empty, the path of the
   * class is used. If the path string isn't empty, it overrides the current
   * working path. If this function fails, the error code can be accessed using
   * the getError() method.
   * @param path
   * @return true on success; false otherwise
   */
  bool load(std::string path = "") {
    if (path == "" && this->path == "")
      return false;

    if (path == "")
      path = this->path;

    this->path = path;

    error = ini_read_file(path.c_str(), IniParser::callback, this, file.comment,
                          file.equals, file.utf8_mode);
    if (error < 0) {
      error = READ_ERROR;

      clear();
    }

    return (error == 0);
  }

  /**
   * Stores an INI file at a given path. If this path is empty, the path of the
   * class is used. If the path string isn't empty, it overrides the current
   * working path. If this function fails, the error code can be accessed using
   * the getError() method.
   * @param path
   * @return true on success; false otherwise
   */
  bool store(std::string path = "") {
    if (path == "" && this->path == "")
      return false;

    if (path == "")
      path = this->path;

	this->path = path;
	
    if (ini_open_file(&file, path.c_str(), file.equals, file.comment,
                      file.utf8_mode)) {
      error = WRITE_ERROR;
      return false;
    }

    if (ini_write_comment(&file, comment.c_str())) {
      error = WRITE_ERROR;
      return false;
    }

    for (IniSection section : sections) {
      if (ini_write_comment(&file, section.getComment().c_str())) {
        error = WRITE_ERROR;
        return false;
      }
      if (ini_write_section(&file, section.getName().c_str())) {
        error = WRITE_ERROR;
        return false;
      }

      for (IniEntry entry : section.getAllEntries()) {
        if (ini_write_comment(&file, entry.getComment().c_str())) {
          error = WRITE_ERROR;
          return false;
        }
        if (ini_write_name_value(&file, entry.getKey().c_str(),
                                 entry.getString().c_str())) {
          error = WRITE_ERROR;
          return false;
        }
      }
    }

    if (ini_close_file(&file)) {
      error = WRITE_ERROR;
      return false;
    }

    return true;
  }

  /**
   * Returns the number of currently loaded sections.
   * @return number of sections
   */
  size_t size() const { return sections.size(); }

  /**
   * Tests if there are currently any sections loaded.
   * @return true, if there are sections present
   */
  bool empty() const { return sections.empty(); }

  /**
   * Getter for the current working path.
   * @return path
   */
  std::string getPath() const { return path; }

  /**
   * Getter for the error. This error can be converted to a string using the
   * getErrorString() method.
   * @return error
   */
  int getError() const { return error; }

  /**
   * Returns an error string for the given error code.
   * @param error code
   * @return error string
   * @throws might throw exceptions (see std::string construtor)
   */
  std::string getErrorString(int error) const {
    if (error == WRITE_ERROR)
      return std::string("Error while writing file: ") + path +
             std::string("!");
    else if (error == READ_ERROR)
      return std::string("Error while reading file: ") + path +
             std::string("!");
    else if (error == NO_ERROR)
      return std::string("No errors detected.");
    else if (error == INVALID_STRING_ERROR)
      return std::string("Invalid string found!");
    else
      return std::string("Error in file ") + path + std::string(" on line ") +
             std::to_string(error) + std::string("!");
  }

  /**
   * Returns an std::vector of all currently loaded sections
   * @return sections
   */
  std::vector<IniSection> getAllSections() const { return sections; }

  /**
   * Returns the section with the given name.
   * @param section name
   * @return section
   * @throws might throw exceptions (see std::string construtor)
   */
  IniSection &at(const std::string &name = "") {
    for (unsigned int i = 0; i < sections.size(); ++i)
      if (sections.at(i).getName() == name)
        return sections.at(i);

    sections.push_back(IniSection(name));
    return sections.at(sections.size() - 1);
  }

  /**
   * Returns the section with the given name.
   * @param section name
   * @return section
   * @throws might throw exceptions (see std::string construtor)
   */
  IniSection &operator[](const std::string &name) { return at(name); }

  /**
   * cleans out all empty sections.
   */
  void clean() {
    for (int i = sections.size() - 1; i >= 0; --i)
      if (sections.at(i).empty())
        sections.erase(sections.begin() + i);
  }

  /**
   * Deletes all sections.
   */
  void clear() { sections.clear(); }

  /**
   * Inserts a section.
   * @param section
   * @throws might throw exceptions (see std::vector::push_back)
   */
  void insert(std::string sectionName) { sections.push_back(sectionName); }

  /**
   * Erases a section with a given name.
   * @param section
   */
  void erase(const std::string &sectionName) {
    for (unsigned int i = 0; i < sections.size(); ++i)
      if (sections.at(i).getName() == sectionName)
        sections.erase(sections.begin() + i);
  }

  /**
   * Setter for the comment.
   * @param comment
   */
  void setComment(std::string comment) { this->comment = comment; }

protected:
private:
  std::string path;
  std::string comment;
  int error;
  struct ini_file file;
  std::vector<IniSection> sections;

  const int NO_ERROR = 0;
  const int READ_ERROR = -1;
  const int WRITE_ERROR = -2;
  const int INVALID_STRING_ERROR = -3;

  /**
   * Callback function for the C interface.
   * @param section name
   * @param key
   * @param value
   * @param data structure
   */
  static int callback(const char *section, const char *name, const char *value,
                      void *data_structure) {
    if (!data_structure)
      return 0;
    IniParser *parser = (IniParser *)data_structure;

    std::string section_str = std::string((section ? section : ""));
    std::string name_str = std::string((name ? name : ""));
    std::string value_str = std::string((value ? value : ""));

    parser->at(section).at(name) = value;

    return 1;
  }
};

#endif // INIPARSER_H
