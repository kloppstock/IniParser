#include <iostream>

//#include "include/IniParser.h"
#include "include/ini_parser.h"

#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>
#include <locale.h>

using namespace std;

int handler(char* section, char* name, char* value, void* data_structure)
{
    string s = "";
    if(section != NULL)
        s = string(section);
    string n = "";
    if(name != NULL)
        n = string(name);
    string v = "";
    if(value != NULL)
        v = string(value);
    cout << "[" << s << "][" << n << "] = " << v << endl;
    return 0;
}

int main()
{
//    IniParser p;
//    p[""]["var0"] = "Noice";
//    p["Test"]["var1"] = "3";
//    p["Test"]["var0"] = 2;
//    p["GIVE_ME_THE_D"]["d"] = 0.1234f;
//    p.store("./test.txt");
//
//    cout << "d = " << (string)p["GIVE_ME_THE_D"]["d"] << std::endl;
//    cout << "not the d = " << (string)p["GIVE_ME_THE_D"]["not the d"] << std::endl;
//    cout << "var1 + 3 = " << (int)p["Test"]["var1"] + 3 << std::endl;
//    return 0;
    void* p = NULL;
    cout << "reader returned: " << ini_read_file("test.ini", handler, p, ';', '=') << endl;
    return 0;
}
