#ifndef UDPSERVER
#define UDPSERVER
#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
using namespace std;

class udp_server{
private:
    int sockfd; /* socket */
    int portno; /* port to listen on */
    unsigned clientlen; /* byte size of client's address */
    struct sockaddr_in serveraddr; /* server's addr */
    struct sockaddr_in clientaddr; /* client addr */
    struct hostent *hostp; /* client host info */
    char *hostaddrp; /* dotted decimal host addr string */
    int optval; /* flag value for setsockopt */
    int n; /* message byte size */
public:
    
    void receive(char *buf, size_t bufsize);

    void send(const char *buf, size_t bufsize);
    
    udp_server(int port);
};

#endif
