#include "Connection.h"
///TO-DO            Comment all of this better


Connection::Connection(float _timeOut): listenThread( &Connection::addServer, this), timeOut(_timeOut) {

    listening = true;
    initREGEX();
    initBOTEX();
}

Connection::~Connection(){

  for(auto &socks : socketConnections){

      //calls destructor in all sockets in socket vector
      //closing thread loops and
      delete socks;
  }
}

//this function uses the first string args to find the connection to send the message in  string args2
void Connection::sendTo(std::string _name, std::string _message){

    for(auto &socks: socketConnections){

        if(socks->getName() == _name){

            //set to send is true, the socket now knows it can post a message as message has been initialised
            socks->setToSend(true);

            //post message has been updated, connection now has a message to send
            socks->setPostMessage(_message);
        }
    }
}

//returns the most recently received message in the top of the stack for the connection named as the same as in args
//REGEX and BOTEX functions scan each string for keywords
std::string Connection::receiveFrom(std::string _name){

     for(auto &socks: socketConnections){

        if(socks->getName() == _name){

            if(socks->unreadMessages() > 0){

                //initialise temp string with data from stack
                std::string temp = socks->getMessage();

                //checks string for commands
                BOTEX( _name, temp);

                //strips string of exluded expressions
                return REGEX( temp);
            }
        }
    }

    return "no data from " + _name;
}

//kills connection with same name as args
void Connection::killConnection(std::string _name){

    if(_name.compare("server")){

        //stops loop in thread and halts block mode of socket
        //this way the connection is closed
        listening = false;
        listener.setBlocking( false);
    }else{

        //esle loops until finds name of socket wrapper
        for(auto &socks: socketConnections){

            if(socks->getName() == _name){

                socks->setAlive( false);
                socks->closeSocket();
                std::cout <<"\n"<<socks->getName()<<" has been disconnected!"<< std::endl;
            }
        }
    }
}

//used to poll connections to check if there is data on the stack
//if connection with matching name to string in args has data will return true
bool Connection::dataAvailable(std::string _name){

     for(auto &socks: socketConnections){

        if(socks->getName() == _name){

            if(!socks->messageStack.empty()){

                return true;
            }else{

                return false;
            }
        }
    }
}

//pings a connection and sets pinged timer/flag to true
void Connection::pingConnection(){

}

//if ping does not return ping within time at args kill connection
void Connection::pingTimeOut(float fl){

}
//getter method for ticks, this is to return passed time from last call
std::clock_t Connection::getTicks(){

    return ticks;
}

//sets teh port number to listen for connections on then launches the server in a thread
void Connection::createServer(unsigned short _port){

    port = _port;

    if (listener.listen( port) != sf::Socket::Done){

        std::cout<<" Couldn't bind to the port: " << port << std::endl;
    }else{

        std::cout<<"Launching server."<< std::endl;
        listenThread.launch();
    }
}

//must be called in a thread
void Connection::addServer(){

    //constantly listening for a new connection
    while(listening){

        //new socket wrapper object to add to array of connections
        socketConnections.push_back(new sockWrapper( "server", "127.0.0.1",  port));

        //blocks untilll connection is made
        if (listener.accept( socketConnections.back()->socket) == sf::Socket::Done){

            std::cout <<" connection made to server."<< std::endl;
            std::cout<< "Total connections: " << socketConnections.size() << std::endl;
            socketConnections.back()->send("Connected.\n");
            socketConnections.back()->setAlive(true);
            socketConnections.back()->runThread();
        }
    }
}


//adds client socket in args to vector
bool Connection::addSocket( std::string _name, std::string _ip, unsigned short _port){

    //adds a new socket to the socket array list with arguments
    socketConnections.push_back( new sockWrapper( _name, _ip,  _port));

    //connect the socket to the server
    return socketConnections.back()->connect();
}

//takes in each string and returns altered string if a matching expression is found
std::string Connection::REGEX( std::string _input){

    //loops for each vector expressions
    for(auto r: regex){

        //gets position of substring
        std::string::size_type i = _input.find(r);

        //if not at end of line, delete regex expression from string
        if (i != std::string::npos)
            _input.erase(i, r.length());
    }

    //return possibly altered input
    return _input;
}

//takes name of connection to send to in first arg, takes string to check for commands in second
void Connection::BOTEX( std::string _name, std::string _input){

    //for each item in botex
    for(auto b: botex){

        //if first expression is found
        if( _input.find( b.first)){

            //send second expression to connection with name in first arg
            sendTo( _name, b.second);
        }
    }
}

//adds a new expression to REGEX.txt to exclude
void Connection::addREGEX( std::string _reg){

    //adds to vector of strings to exclude
    regex.push_back( _reg);

    //also adds new expression to exclude in a seperate text file
    stringload.addString( _reg, "connection/REGEX.txt");
}

//adds a new expression to BOTEX.txt to listen for in args 1 and to respond with in arg 2
void Connection::addBOTEX( std::string _hear, std::string _say){

    //adds expressions to botax map of pairs
    botex[ _hear] = _say;

    //also adds expression to BOTEX.txt file
    stringload.addString( (_hear + "-" + _say), "connection/BOTEX.txt");
}

//initialises botex with each line from BOTEX.txt
void Connection::initBOTEX(){

    //load up BOTEX.txt data from path that string describes
    std::string bot = stringload.doString("connection/BOTEX.txt");

    //create input string stream to manage line by line feed from frequency table
    std::istringstream stable( bot);

    std::string temp;

    // Process for each line, seperates string and float value and pushes that onto pair stream
    while ( std::getline( stable, temp)) {

        //position of comma to size_t
        size_t pos = temp.find("-");

        //first half of string in line is key
        std::string _first = temp.substr( 0, pos);

        //get substring from pos to end of line, this contains string of a float
        temp = temp.substr( pos + 1);

        //adds expressions to botax map of pairs
        botex[ _first] = temp;
    }
}

//initialises regex with eachline from REGEX.txt
void Connection::initREGEX(){

    //load up REGEX.txt data from path that string describes
    std::string reg = stringload.doString("connection/REGEX.txt");

    //create input string stream to manage line by line feed from frequency table
    std::istringstream stable( reg);

    std::string temp;

    //adds each line to vector 'regex'
    while ( std::getline( stable, temp)) {

        regex.push_back( temp);
    }
}
