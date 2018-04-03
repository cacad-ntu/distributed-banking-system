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
    if(distribution(generator) > failureRate)
        sendto(sockfd, buf, bufsize, 0, (struct sockaddr *) &clientaddr, clientlen);
    else cout << "Failure simulated.\n";
}

void udp_server::send(const char *buf, size_t bufsize, struct sockaddr_in addr, unsigned len){
    if(distribution(generator) > failureRate)
        sendto(sockfd, buf, bufsize, 0, (struct sockaddr *) &addr, len);
    else cout << "Failure simulated.\n";
}

udp_server::udp_server(int port, double _failureRate){
    portno = port;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0){
        perror("ERROR opening socket");
        return;
    }
    
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons((unsigned short)portno);

    if (bind(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
        perror("ERROR on binding");

    clientlen = sizeof(clientaddr);
    seed = chrono::system_clock::now().time_since_epoch().count();
    generator = mt19937(seed);
    distribution = uniform_real_distribution<double> (0.0,1.0);

    failureRate = _failureRate;
}

struct sockaddr_in udp_server::getClientAddress(){
    return clientaddr;
}

unsigned udp_server::getClientLength(){
    return clientlen;
}
