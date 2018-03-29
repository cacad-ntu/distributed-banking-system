#ifndef HANDLERS
#define HANDLERS

#include <iostream>
#include "udp_server.h"
#include "utils.h"
#include "AccountManager.h"
#include "constants.h"
#include <chrono>
#include <deque>
#include "Admin.h"
using namespace std;

class Handler{
 private:
    deque<Admin> admins;
    AccountManager acManager;
 public:
    void notify(udp_server &server, string s);
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
