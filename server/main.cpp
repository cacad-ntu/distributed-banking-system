#include "udp_client_server.h"
#include "utils.h"
#include <iostream>
using namespace std;
using namespace udp_client_server;
#define HEADER_SIZE 4

int portno;
char header[4];
char *buffer;
int message_length;

void receive(udp_server &server){
	server.recv(header,HEADER_SIZE);
	message_length = utils::unmarshalInt(header);
	cout << "Message length: " << message_length << '\n';

	buffer = new char[message_length];
	server.recv(buffer,message_length);
	
	char *cur = buffer;
	int type = *cur;
	cur++;
	int length = utils::unmarshalInt(cur);
	cur += 4;

	if(type == 1){
		float x = utils::unmarshalFloat(cur);
		cout << "Received float: " << x << "\n";
	}
	else if(type == 2){
		int x = utils::unmarshalInt(cur);
		cout << "Received int: " << x << "\n";
	}
	else if(type == 3){
		string x = utils::unmarshalString(cur,length);
		cout << "Received string: " << x << "\n";
	}
}

void send(udp_server &server){
	server.send(header,HEADER_SIZE);
	server.send(buffer,message_length);
}

int main(int argc, char **argv){
    if (argc != 2) portno = 8080;
    else portno = atoi(argv[1]);
    udp_server server("127.0.0.1",8080);
    while(true){
		receive(server);
		send(server);
    }
    return 0;
}
