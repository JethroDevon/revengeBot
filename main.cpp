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

    while( mhandler.isRunning()){

  
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
