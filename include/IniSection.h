#ifndef INISECTION_H
#define INISECTION_H

#include <iostream>
#include <string>
#include <vector>

#include "IniEntry.h"

class IniSection
{
    public:
        IniSection(std::string name = "");
        IniSection(const IniSection& other);
        ~IniSection() = default;

        IniSection& operator=(const IniSection& other);

        size_t size() const;
        std::vector<std::string> getAllEntries() const;
        std::string getName() const;

        IniEntry& at(const std::string& key);
        IniEntry& operator[](const std::string& key);
        void clean();
        void clear();
        void insert(std::string key, std::string value);
        void erase(const std::string& key);

        std::string toString() const;
    protected:
    private:
        std::string name;
        std::vector<IniEntry> elements;
};

#endif // INISECTION_H
