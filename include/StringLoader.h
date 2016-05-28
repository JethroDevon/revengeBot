#ifndef STRING_LOADER_H
#define STRING_LOADER_H

#include <iostream>
#include <string>
#include <fstream>
#include <map>

class StringLoader{

    public:

        //no constructor for string loader, will be static

        //returns the string that is mapped to the one in args or loads and adds
        //a new one
        std::string doString( std::string);

        //appends a new string (args 1) to the end of the file at path args2
        void addString( std::string, std::string);

    private:

        std::map<std::string, std::string> strings;
};

#endif // STRING_LOADER_H
