#include "IniSection.h"

IniSection::IniSection(std::string name)
{
    this->name = name;
}

IniSection::IniSection(const IniSection& other)
{
    this->name = other.name;
    this->elements = other.elements;
}

IniSection& IniSection::operator=(const IniSection& other)
{
    this->name = other.name;
    this->elements.clear();
    this->elements = other.elements;
    return *this;
}

size_t IniSection::size() const
{
    return this->elements.size();
}

std::vector<std::string> IniSection::getAllEntries() const
{
    std::vector<std::string> entries;
    for(IniEntry e : this->elements)
    {
        entries.push_back(e.getKey());
    }
    return entries;
}

std::string IniSection::getName() const
{
    return this->name;
}

IniEntry& IniSection::at(const std::string& key)
{
    for(unsigned int i = 0; i < this->elements.size(); ++i)
    {
        if(this->elements.at(i).getKey() == key)
        {
            return this->elements.at(i);
        }
    }
    this->elements.push_back(IniEntry(key));
    return this->elements.at(this->elements.size() - 1);
}

IniEntry& IniSection::operator[](const std::string& key)
{
    return this->at(key);
}

void IniSection::clean()
{
    std::vector<unsigned int> indices;
    for(unsigned int i = 0; i < this->elements.size(); ++i)
    {
        if(this->elements.at(i).toString() == "")
        {
            indices.push_back(i);
        }
    }
    for(unsigned int i = indices.size() - 1; i >= 0; --i)
    {
        this->elements.erase(this->elements.begin() + indices.at(i));
    }
}

void IniSection::clear()
{
    this->elements.clear();
}

void IniSection::insert(std::string key, std::string value)
{
    this->elements.push_back(IniEntry(key, value));
}

void IniSection::erase(const std::string& key)
{
    int index = -1;
    for(unsigned int i = 0; i < this->elements.size(); ++i)
    {
        if(this->elements.at(i).getKey() == key)
        {
            index = i;
            break;
        }
    }
    if(index != -1)
    {
        this->elements.erase(this->elements.begin() + index);
    }
}

std::string IniSection::toString() const
{
    std::string str = "";
    if(name != "")
        str += "[" + this->name + "]\n";
    for(IniEntry e : this->elements)
    {
        str += e.getCompleteString() + "\n";
    }
    return str;
}
