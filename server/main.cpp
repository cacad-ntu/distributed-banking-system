#include "udp_client_server.h"
#include <iostream>
using namespace std;
using namespace udp_client_server;
#define HEADER_SIZE 4

int portno;
char header[4];
char *buffer;

int unmarshalHeader(){
  return header[0] << 24 | (header[1] & 0xFF) << 16 | (header[2] & 0xFF) << 8 | (header[3] & 0xFF);
}

int main(int argc, char **argv){
  if (argc != 2) portno = 8080;
  else portno = atoi(argv[1]);
  udp_server server("127.0.0.1",8080);
  while(true){
    server.recv(header,HEADER_SIZE);
    int message_length = unmarshalHeader();
    cout << "Message length: " << message_length << '\n';

    buffer = new char[message_length];
    server.recv(buffer,message_length);
    cout << buffer << '\n';

    server.send(header,HEADER_SIZE);
    server.send(buffer,message_length);
  }
  return 0;
}
