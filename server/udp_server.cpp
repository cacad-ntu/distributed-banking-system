#include "udp_server.h"

int udp_server::receive_time(char *buf, size_t bufsize, int timeout_in_seconds){
    //bzero(buf, bufsize);
    tv.tv_sec = timeout_in_seconds;
    tv.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
    
    n = recvfrom(sockfd, buf, bufsize, 0, (struct sockaddr *) &clientaddr, &clientlen);
    //if (n < 0) perror("ERROR in recvfrom");
    if(n > 0) printf("server received %d bytes\n", n);
    return n;
}

void udp_server::send(const char *buf, size_t bufsize){
    sendto(sockfd, buf, bufsize, 0, (struct sockaddr *) &clientaddr, clientlen);
}

void udp_server::send(const char *buf, size_t bufsize, struct sockaddr_in addr, unsigned len){
    sendto(sockfd, buf, bufsize, 0, (struct sockaddr *) &addr, len);
}

udp_server::udp_server(int port){
    portno = port;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0){
        perror("Unable to open socket.");
        return;
    }
    
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons((unsigned short)portno);

    if (bind(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
        perror("ERROR on binding");

    clientlen = sizeof(clientaddr);
}

struct sockaddr_in udp_server::getClientAddress(){
    return clientaddr;
}

unsigned udp_server::getClientLength(){
    return clientlen;
}
