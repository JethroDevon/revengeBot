#ifndef MESSAGEHANDLER
#define MESSAGEHANDLER

#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <queue>


class MessageHandler{

    public:

      void CLI();

    private:

      
      void Perform();

      //THIS VECTOR STORES A STACK OF MESSAGES BETWEEN THE USER AND THE PROGRAM
      std::vector<std::map< std::string, std::string>>;

};

#endif // STRING_LOADER_H
