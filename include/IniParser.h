#ifndef INIPARSER_H
#define INIPARSER_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "IniSection.h"

class IniParser
{
    public:
        IniParser(std::string path = "");
        IniParser(const IniParser& other);
        ~IniParser() = default;

        IniParser& operator=(const IniParser& other);

        bool load(std::string path = "");
        bool store(std::string path = "");

        size_t size() const;
        std::string getPath() const;
        bool getError() const;
        std::vector<std::string> getAllSections() const;
        IniSection& at(const std::string& name = "");
        IniSection& operator[](const std::string& name);
        void clean();
        void clear();
        void insert(std::string sectionName);
        void erase(const std::string& sectionName);
    protected:
    private:
        std::string path;
        bool error;
        std::vector<IniSection> sections;

        enum ParsingState
        {
            PARSING_STATE_SEEK_FRONT,
            PARSING_STATE_SEEK_SECTION,
            PARSING_STATE_READ_SECTION,
            PARSING_STATE_SEEK_MORE_SECTION,
            PARSING_STATE_SEEK_EOL,
            PARSING_STATE_READ_ENTRY_NAME,
            PARSING_STATE_SEEK_EQUALS,
            PARSING_STATE_SEEK_VALUE,
            PARSING_STATE_READ_VALUE,
            PARSING_STATE_SEEK_MORE_VALUE,
            PARSING_STATE_READ_MULTI_LINE_VALUE,
            PARSING_STATE_SEAK_MORE_MULTI_LINE_VALUE
        };
};

#endif // INIPARSER_H
