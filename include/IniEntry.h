#ifndef INIENTRY_H
#define INIENTRY_H

#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class IniEntry
{
    public:
        IniEntry(std::string key);
        IniEntry(std::string key, std::string value);
        IniEntry(std::string key, double value);
        IniEntry(std::string key, int value);
        IniEntry(const IniEntry& other);
        ~IniEntry() = default;

        IniEntry operator=(const IniEntry& other);
        IniEntry operator=(const std::string& other);
        IniEntry operator=(const double& other);
        IniEntry operator=(const int& other);

        std::string getKey() const;
        std::string toString() const;
        double toDouble() const;
        int toInt() const;

        operator std::string() const;
        operator double() const;
        operator int() const;

        std::string getCompleteString() const;
    protected:
    private:
        std::string key;
        std::string value;
};

#endif // INIENTRY_H
