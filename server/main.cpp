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

int status;
/*
void send(udp_server &server){
    server.send(header,HEADER_SIZE);
    server.send(buffer,message_length);
}
*/

void receive(udp_server &server){
    cout << "WAITING\n";
    int n = server.receive_time(header,HEADER_SIZE,10000);

    if(n <= 0){
        cout << "RETURNING\n";
        return;
    }
    
    message_length = utils::unmarshalInt(header);
    cout << "Msg length: " << message_length << '\n';
    
    buffer = new char[message_length];
    n = server.receive_time(buffer,message_length,1);

    if(n <= 0){
        cout << "RETURNING\n";
        return;
    }
    char *cur = buffer;

    int req_id = utils::unmarshalInt(cur);
    cur+=4;
    
    int type = utils::unmarshalInt(cur);
    cur+=4;

    switch(type){
    case 1:
        handler.service1(server,cur,req_id,status);
        break;
    case 2:
        handler.service2(server,cur,req_id,status);
        break;
    case 3:
        handler.service3(server,cur,req_id,status);
        break;
    case 4:
        handler.service4(server,cur,req_id,status);
        break;
    case 5:
        handler.service5(server,cur,req_id,status);
        break;
    case 6:
        handler.service6(server,cur,req_id,status);
        break;
    case 7:
        handler.service7(server,cur,req_id,status);
        break;
    }
}

int main(int argc, char **argv){
    /*
      <program> <port> <status>
     */
    if (argc < 2) portno = 8080;
    else portno = atoi(argv[1]);

    status = 2;
        
    if(argc >= 3) status = atoi(argv[2]);

    udp_server server(portno);
    handler = Handler();
    while(true){
        receive(server);
    }
    return 0;
}

