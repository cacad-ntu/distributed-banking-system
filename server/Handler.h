#ifndef HANDLERS
#define HANDLERS

#include <iostream>
#include "udp_client_server.h"
#include "utils.h"
#include "AccountManager.h"
#include "constants.h"
using namespace std;
using namespace udp_client_server;

class Handler{
 private:
    AccountManager acManager;
 public:
    void service1(udp_server &server, char *p);
    void service2(udp_server &server, char *p);
    void service3(udp_server &server, char *p);
    void service4(udp_server &server, char *p);
    void service5(udp_server &server, char *p);
    void service6(udp_server &server, char *p);
    void service7(udp_server &server, char *p);

    Handler();
};
#endif
