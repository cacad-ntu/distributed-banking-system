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
bool at_most_once;
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

    int req_id = utils::unmarshalInt(cur);
    cur+=4;
    
    int type = utils::unmarshalInt(cur);
    cur+=4;

    switch(type){
    case 1:
        handler.service1(server,cur,req_id,at_most_once);
        break;
    case 2:
        handler.service2(server,cur,req_id,at_most_once);
        break;
    case 3:
        handler.service3(server,cur,req_id,at_most_once);
        break;
    case 4:
        handler.service4(server,cur,req_id,at_most_once);
        break;
    case 5:
        handler.service5(server,cur,req_id,at_most_once);
        break;
    case 6:
        handler.service6(server,cur,req_id,at_most_once);
        break;
    case 7:
        handler.service7(server,cur,req_id,at_most_once);
        break;
    }
}

int main(int argc, char **argv){
    /*
      <program> <port> <at-most-once>
     */
    if (argc < 2) portno = 8080;
    else portno = atoi(argv[1]);

    at_most_once = false;

    if(argc >= 3){
        switch(argv[2][0]){
        case 't':
        case 'T':
        case '1':
            at_most_once = true;
            break;
        }
    }

    udp_server server(portno);
    handler = Handler();
    while(true){
        receive(server);
    }
    return 0;
}

