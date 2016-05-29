#include "Connection.h";
#include "sockWrapper.h";

//port number for socket connection
int PORT;

//URL or IP to connect to, USERNAME could also be name for hte connnection
std::string URL, USERNAME, CHANNEL;

//Connection object for managing connection
Connection con( 1000);

MessageHandler mhandler;

//main function requires exact number of strings passed to it in order to work
int main( int argc, char *argv[]){
  if( argc == 4){

    //converts char array to port number, other global variables are initialised
    PORT = std::atoi( argv[1]);
    URL = argv[2];
    USERNAME = argv[3];

    //initialise socket with data from command line
    con.addSocket( USERNAME, URL, PORT);

    //irc protocol requires a short pause before posting connection data
    sf::sleep( sf::milliseconds( 1000));

    //send irc protocol to join chat room at GoldsmithsC++
    con.sendTo( USERNAME, "NICK revengeBOT\r\nUser revengeBOT * * :rvngbt\r\nJOIN :#GoldsmithsC++\r\n" );

    while( mhandler.isRunning()){

      //irc protocol requires a short pause before posting connection data
      sf::sleep(sf::milliseconds( 1000));

      if( con.dataAvailable( USERNAME)){

        std::cout << con.receiveFrom( USERNAME)<<".-";
      }
    }

    return 0;
  }else{

    std::cout<<"revengeBOT closing, bye :D";
    return 0;
  }
}
