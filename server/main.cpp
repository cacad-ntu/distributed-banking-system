#include "udp_client_server.h"
#include "utils.h"
#include "Handler.h"
#include <iostream>
using namespace std;
using namespace udp_client_server;
#define HEADER_SIZE 4

int portno;
char header[4];
char *buffer;
int message_length;
Handler handler;

/*
void send(udp_server &server){
    server.send(header,HEADER_SIZE);
    server.send(buffer,message_length);
}
*/

void receive(udp_server &server){
    server.recv(header,HEADER_SIZE);
    message_length = utils::unmarshalInt(header);

    buffer = new char[message_length];
    server.recv(buffer,message_length);
    
    char *cur = buffer;
    int type = *cur;

    cur++;

    switch(type){
    case 1:
        handler.service1(server,cur);
        break;
    case 2:
        handler.service2(server,cur);
        break;
    case 3:
        handler.service3(server,cur);
        break;
    case 4:
        handler.service4(server,cur);
        break;
    case 5:
        handler.service5(server,cur);
        break;
    case 6:
        handler.service6(server,cur);
        break;
    case 7:
        handler.service7(server,cur);
        break;
    }
}

int main(int argc, char **argv){
    if (argc != 2) portno = 8080;
    else portno = atoi(argv[1]);
    udp_server server("127.0.0.1",8080);
    handler = Handler();
    while(true){
        receive(server);
    }
    return 0;
}
