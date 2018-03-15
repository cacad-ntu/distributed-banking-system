#include "udp_server.h"
void udp_server::receive(char *buf, size_t bufsize){
    n = recvfrom(sockfd, buf, bufsize, 0, (struct sockaddr *) &clientaddr, &clientlen);
    if (n < 0) perror("ERROR in recvfrom");

    hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, sizeof(clientaddr.sin_a\
ddr.s_addr), AF_INET);
    if (hostp == NULL) perror("ERROR on gethostbyaddr");
    hostaddrp = inet_ntoa(clientaddr.sin_addr);
    if (hostaddrp == NULL) perror("ERROR on inet_ntoa\n");
    printf("server received datagram from %s (%s)\n", hostp->h_name, hostaddrp);
    printf("server received %d/%d bytes: %d\n", strlen(buf), n, (int)(buf[0]));
}

void udp_server::send(const char *buf, size_t bufsize){
    sendto(sockfd, buf, bufsize, 0, (struct sockaddr *) &clientaddr, clientlen);
}

udp_server::udp_server(int port){
    portno = port;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0){
        perror("ERROR opening socket");
        return;
    }

    optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int));

    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons((unsigned short)portno);

    if (bind(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
        perror("ERROR on binding");

    clientlen = sizeof(clientaddr);
}
