#ifndef HANDLERS
#define HANDLERS

#include <iostream>
#include "udp_client_server.h"
#include "utils.h"
using namespace std;
using namespace udp_client_server;


namespace handlers{
    void service1Handler(udp_server &server, char *p);
    void service2Handler(udp_server &server, char *p);
    void service3Handler(udp_server &server, char *p);
    void service4Handler(udp_server &server, char *p);
    void service5Handler(udp_server &server, char *p);
    void service6Handler(udp_server &server, char *p);
}
#endif
