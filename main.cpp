#include <iostream>
#include <string>

#include "include/IniParser.hpp"
//#include "include/ini_parser.h"

using namespace std;

/*int handler(const char* section, const char* name, const char* value, void*
   data_structure) {
        printf("[%s][%s] = %s\n", section, name, value);
    return 0;
        }*/

int main() {
	/*
  try {
    IniParser p("./test.ini");
    //    p[""]["var0"] = "Noice";
    //    p["Test"]["var1"] = "3";
    //    p["Test"]["var0"] = 2;
    //    p["GIVE_ME_THE_D"]["d"] = 0.1234f;
    //    p.store("./test.txt");
    //
    //    cout << "d = " << (string)p["GIVE_ME_THE_D"]["d"] << std::endl;
    //    cout << "not the d = " << (string)p["GIVE_ME_THE_D"]["not the d"] <<
    //    std::endl;
    //    cout << "var1 + 3 = " << (int)p["Test"]["var1"] + 3 << std::endl;
    //    return 0;
    //    void* p = NULL;
    //    cout << "reader returned: " << ini_read_file("test.ini", handler, p,
    //    ';', '=') << endl;

    for (IniSection section : p.getAllSections()) {
      for (IniEntry entry : section.getAllEntries()) {
        cout << section.getName() << "." << entry.getKey() << " = "
             << entry.getString() << "\n";
      }
    }
  } catch (exception e) {
    cout << "Error: " << e.what() << "\n";
	}*/

	
	IniParser p;
	if(!p.load("./test.ini")) {
		cout << "som ting wong\n";
		cout << "Error: " << p.getErrorString(p.getError()) << "\n";
	} else {
		cout << "really doing stuff now\n";
		for (IniSection section : p.getAllSections()) {
			for (IniEntry entry : section.getAllEntries()) {
				cout << section.getName() << "." << entry.getKey() << " = "
					 << entry.getString() << "\n";
			}
		}
	}

  return 0;
}
