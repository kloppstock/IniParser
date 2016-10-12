#include "IniParser.h"

IniParser::IniParser(std::string path)
{
    this->path = path;
    this->error = false;
    if(this->path != "")
        this->load();
}

IniParser::IniParser(const IniParser& other)
{
    this->path = other.path;
    this->sections = other.sections;
}

IniParser& IniParser::operator=(const IniParser& other)
{
    this->path = other.path;
    this->sections.clear();
    this->sections = other.sections;
    return *this;
}

bool IniParser::load(std::string path)
{
    if(path != "")
        this->path = path;

    if(this->path == "")
        return false;

    std::ifstream is(this->path);

    if(!is.is_open())
        return false;

    //begin parsing
    std::string line;
    std::string current_section = "";
    enum ParsingState state = PARSING_STATE_SEEK_FRONT;
    while(!is.eof())
    {
//        is.getline(line);

        for(char c : line)
        {
//            switch(c)
//            {
//                //handle whitespaces
//                case ' ':
//                case '\t':
//                case '\n':
//                    break;
//                case ''
//            };
        }
    }

    //parsing is done, now clean up
    is.close();
    return true;
}

bool IniParser::store(std::string path)
{
    if(path != "")
        this->path = path;

    if(this->path == "")
        return false;

    std::string content = "";
    for(IniSection s : this->sections)
    {
        content += s.toString() + "\n";
    }

    std::ofstream os(this->path);
    if(!os.is_open())
        return false;

    os << content;
    os.flush();
    os.close();

    return true;
}

size_t IniParser::size() const
{
    return this->sections.size();
}

std::string IniParser::getPath() const
{
    return this->path;
}

bool IniParser::getError() const
{
    return this->error;
}

std::vector<std::string> IniParser::getAllSections() const
{
    std::vector<std::string> sec;
    for(IniSection s : this->sections)
    {
        sec.push_back(s.getName());
    }
    return sec;
}

IniSection& IniParser::at(const std::string& name)
{
    for(unsigned int i = 0; i < this->sections.size(); ++i)
    {
        if(this->sections.at(i).getName() == name)
        {
            return this->sections.at(i);
        }
    }
    this->sections.push_back(IniSection(name));
    return this->sections.at(this->sections.size() - 1);
}

IniSection& IniParser::operator[](const std::string& name)
{
    return this->at(name);
}

void IniParser::clean()
{
    std::vector<unsigned int> indices;
    for(unsigned int i = 0; i < this->sections.size(); ++i)
    {
        if(this->sections.at(i).size() == 0)
        {
            indices.push_back(i);
        }
    }
    for(unsigned int i = indices.size() - 1; i >= 0; --i)
    {
        this->sections.erase(this->sections.begin() + indices.at(i));
    }
}

void IniParser::clear()
{
    this->sections.clear();
}

void IniParser::insert(std::string sectionName)
{
    this->sections.push_back(IniSection(sectionName));
}

void IniParser::erase(const std::string& sectionName)
{
    int index = -1;
    for(unsigned int i = 0; i < this->sections.size(); ++i)
    {
        if(this->sections.at(i).getName() == sectionName)
        {
            index = i;
            break;
        }
    }
    if(index != -1)
    {
        this->sections.erase(this->sections.begin() + index);
    }
}
