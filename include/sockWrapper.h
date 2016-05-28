#ifndef SOCKWRAPPER_H
#define SOCKWRAPPER_H

#include <SFML/Network.hpp>
#include <string>
#include <cstring>
#include <vector>
#include <queue>

/*

    This class Creates a connection on creation and if it does
    not succeed, isAlive is set to false, it also manages a receive
    function that is ran in its own thread
    receive stacks incoming messages in a string vector called message stack.

*/

class sockWrapper{

    public:

        //constructor for TCP client
        sockWrapper( std::string, std::string, unsigned short);

        virtual ~sockWrapper();

        //get for top message from message stack, once popped is destroyed,
        std::string getMessage();

        //setPostMessage sets message to be posted in send function
        void setPostMessage(std::string);

        //post message returns the message to be posted and updated by setPostMessage in the function function send
        std::string postMessage();

        //send and receive are handled within a separate thread, this function launches it each time
        //it is called
        void runConnection();

        //connect send and receive functions
        void send(std::string);
        void receive();
        bool connect();

        //set and get port and IP
        unsigned short getPort();
        std::string getIP();

        //set ad get isAlive flag
        bool getAlive();
        void setAlive(bool);

        //set and get to post bool
        void setToSend(bool);
        bool getToSend();

        //closes socket
        void closeSocket();

        //gets connection name
        std::string getName();

        int unreadMessages();

        void runThread();

        sf::Thread receiveThread, run;

        //to avoid leaving send and receive functions spinning in their threads
        sf::Mutex mutex;

        //for the socket connection from a server, will be returned by a get function
        sf::TcpSocket socket;

        //will store a stack of strings if messages have been received but not processed
        std::vector<std::string> messageStack;

    private:

        //ip address, connections name
        std::string IP, connectionName, message;

        //if isAlive is ever false once the socket has been connected the socket will close gracefully, toSend is set to true when there is a message to send
        bool isAlive, toSend;

        sf::Packet packet;

        //will contain port number
        unsigned short port;
};

#endif // SOCKWRAPPER_H
