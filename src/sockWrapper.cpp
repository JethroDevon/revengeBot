#include "sockWrapper.h"
#include <iostream>

//TCP client constructor, first initialiser list arg is directly initialising thread as it cannot have a default constructor
sockWrapper::sockWrapper(std::string _clientName, std::string _ipAddress, unsigned short _port): receiveThread( &sockWrapper::receive, this), run( &sockWrapper::runConnection, this), connectionName( _clientName), IP( _ipAddress), port ( _port){

    isAlive = false;
    toSend = false;
    socket.setBlocking(false);
}

sockWrapper::~sockWrapper(){

      setAlive( false);
      messageStack.clear();
}

std::string sockWrapper::getName(){

    return connectionName;
}

void sockWrapper::send( std::string _message){

    _message += '\0';
    const char* toSend = _message.c_str();
    socket.send( toSend, strlen(toSend));
}

void sockWrapper::receive(){

    std::vector<char> buffer(2000);
    std::size_t received = 0;

    // TCP socket:
    if(socket.receive(buffer.data(), buffer.size(), received) == sf::Socket::Done) {

        std::string str(buffer.data(), 0, received);
        messageStack.push_back(str);
    }
}

//returns size of message stack
int sockWrapper::unreadMessages(){

    return messageStack.size();
}

bool sockWrapper::connect(){

    //connect to server
    socket.connect(getIP(), getPort());

    //returns 0 if there is no connection, this is grounds to set alive to false
    //and close this socket
    if(socket.getRemotePort() != 0){

        setAlive(true);
        run.launch();
        std::cout << "connection to " << connectionName <<" completed."<< std::endl;
        return true;
    }else{

        setAlive(false);
        std::cout << "connection to " << connectionName << " failed." << std::endl;
        return false;
    }
}

std::string sockWrapper::getIP(){

    return IP;
}

unsigned short sockWrapper::getPort(){

    return port;
}

bool sockWrapper::getAlive(){

    return isAlive;
}

void sockWrapper::setAlive( bool _a){

    isAlive = _a;
}

//returns last message in stack to application while guarding process with mutex locks
std::string sockWrapper::getMessage(){

    std::string temp;

    //mutex avoids change in size of stack while runConnection i
    mutex.lock();

    if(!messageStack.empty()){

        temp = messageStack.back();
        messageStack.pop_back();
        mutex.unlock();
        //std::cout<< temp <<std::endl;
        return temp;
    }

    mutex.unlock();

    return "bad message stack protocol, out of range error likley!";
}

void sockWrapper::setPostMessage(std::string _m){

    message = _m;
}

bool sockWrapper::getToSend(){

    return toSend;
}

void sockWrapper::setToSend(bool _b){

    toSend = _b;
}

std::string sockWrapper::postMessage(){

    return message;
}

void sockWrapper::closeSocket(){

    socket.disconnect();
}

void sockWrapper::runThread(){

    run.launch();
}

void sockWrapper::runConnection(){

    while(getAlive()){

        if(getToSend()){

            //protect thread from using stack at wrong time
            mutex.lock();
            send(postMessage());

            messageStack.push_back( message + "\n");

            //free mutex
            mutex.unlock();

            //sets message back to ""
            message = "";

            //sets flag to post something back to false
            setToSend(false);
        }

        sf::sleep(sf::milliseconds(10));

        //protect receive functions
        mutex.lock();

        //launches thread of thread for one run, this is normally for if socket is blocking
        receiveThread.launch();

        //free up lock again
        mutex.unlock();
    }
}
