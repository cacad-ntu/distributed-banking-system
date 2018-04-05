#include "udp_server.h"
#include "utils.h"
#include "Handler.h"
#include <iostream>
#include "constants.h"
using namespace std;

int portno;
char header[4];
char *buffer;
int message_length;
double failureRate;
int status;
int limit;

/**
   Handles incoming request from client
 */
void handleRequest(udp_server &server, Handler &handler){
    cout << "WAITING\n";
    int n = server.receive_time(header,HEADER_SIZE,INF);

    if(n <= 0){
        cout << "RETURNING\n";
        return;
    }
    
    message_length = utils::unmarshalInt(header);
    cout << "Msg length: " << message_length << '\n';
    
    buffer = new char[message_length];

    cout << "Receiving message..\n";
    n = server.receive_time(buffer,message_length,RECEIVE_TIMEOUT);
    cout << "Received.\n";
    
    if(n <= 0){
        cout << "RETURNING\n";
        return;
    }
    char *cur = buffer;

    int req_id = utils::unmarshalInt(cur);
    cur+=INT_SIZE;
    
    int type = utils::unmarshalInt(cur);
    cur+=INT_SIZE;

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
      <program> <port> <status> <failure> <limit>
     */

    if (argc < 2) portno = 8080;
    else portno = atoi(argv[1]);

    status = 2;
        
    if(argc >= 3) status = atoi(argv[2]);

    failureRate = 0;
    if(argc >= 4) failureRate = (double)(atoi(argv[3])) / (double)100.0;

    limit = -1;
    if(argc >= 5) limit = atoi(argv[4]);

    udp_server server(portno);
    Handler handler(limit, failureRate);
    while(true){
        handleRequest(server,handler);
    }
    return 0;
}

