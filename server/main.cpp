#include "udp_server.h"
#include "utils.h"
#include "Handler.h"
#include <iostream>
using namespace std;
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
    cout << "WAITING\n";
    server.receive(header,HEADER_SIZE);
    message_length = utils::unmarshalInt(header);
    cout << "Msg length: " << message_length << '\n';
    
    buffer = new char[message_length];
    server.receive(buffer,message_length);
    
    char *cur = buffer;
    int type = utils::unmarshalInt(cur);
    
    cur+=4;

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
    udp_server server(portno);
    handler = Handler();
    while(true){
        receive(server);
    }
    return 0;
}
