#include "StringLoader.h"

void StringLoader::addString( std::string _string, std::string _path){

    std::ofstream ofile( _path);

    if ( ofile){

        ofile << _string;
        ofile.close();
        std::cout<< "Line succesfully appended to end of text file" <<std::endl;
    }else{

         std::cout<< "Error adding to textfile at " << _path <<"." <<std::endl;
    }
}
//help from http://forums.codeguru.com/showthread.php?523390-C-adding-new-string-into-existing-file

//this function maps the path to a string to the string contained in path, if the string has been loaded 
//before then the strings path is just the key to the string at the path else
//the path is used to return the string
std::string StringLoader::doString( std::string _path){

	for( auto s: strings){

		if(s.first == _path){

            std::cout<< "string at " << _path << " found."<< std::endl;
            return s.second;
        }
    }

    //temporary string container
    std::string temp, input;

    std::ifstream textFile( _path.c_str());

    if( textFile){

        while(getline( textFile, input)){ 

            temp += input + '\n';
        }

        //setting key value pair
        strings[ _path] = temp;

        //recursivley call function again, this way string will have been initialised
        return doString( _path);
    }else{

        std::cout <<"File at " << _path << " not found."<< std::endl;
        return "File does not exist";
    }
}

//help from http://www.dreamincode.net/forums/topic/145699-eof-and-reading-text-files-c/