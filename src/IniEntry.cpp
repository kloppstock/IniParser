#include "IniEntry.h"

IniEntry::IniEntry(std::string key)
{
    this->key = key;
    this->value = "";
}

IniEntry::IniEntry(std::string key, std::string value)
{
    this->key = key;
    this->value = value;
}

IniEntry::IniEntry(std::string key, double value)
{
    this->key = key;
    this->value = std::to_string(value);
}

IniEntry::IniEntry(std::string key, int value)
{
    this->key = key;
    this->value = std::to_string(value);
}

IniEntry::IniEntry(const IniEntry& other)
{
    this->key = other.key;
    this->value = other.value;
}

IniEntry IniEntry::operator=(const IniEntry& other)
{
    this->key = other.key;
    this->value = other.value;
    return *this;
}

IniEntry IniEntry::operator=(const std::string& other)
{
    this->value = other;
    return *this;
}

IniEntry IniEntry::operator=(const double& other)
{
    this->value = std::to_string(other);
    return *this;
}

IniEntry IniEntry::operator=(const int& other)
{
    this->value = std::to_string(other);
    return *this;
}

std::string IniEntry::getKey() const
{
    return this->key;
}

std::string IniEntry::toString() const
{
    return this->value;
}

double IniEntry::toDouble() const
{
    return atof(this->value.c_str());
}

int IniEntry::toInt() const
{
    return atoi(this->value.c_str());
}

IniEntry::operator std::string() const
{
    return this->toString();
}

IniEntry::operator double() const
{
    return this->toDouble();
}

IniEntry::operator int() const
{
    return this->toInt();
}

std::string IniEntry::getCompleteString() const
{
    return this->key + " = " + this->value;
}
