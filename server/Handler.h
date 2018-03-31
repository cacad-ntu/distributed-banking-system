#ifndef HANDLERS
#define HANDLERS

#include <iostream>
#include "udp_server.h"
#include "utils.h"
#include "AccountManager.h"
#include "constants.h"
#include <chrono>
#include <deque>
#include <map>
#include "Admin.h"
using namespace std;

class Handler{
 private:
    deque<Admin> admins;
    AccountManager acManager;
    map<pair<unsigned long,int>,char*> memo;
    int response_id;
 public:
    int getResponseID();
    void notify(udp_server &server, string s, int status);
    void service1(udp_server &server, char *p, int req_id, int status);
    void service2(udp_server &server, char *p, int req_id, int status);
    void service3(udp_server &server, char *p, int req_id, int status);
    void service4(udp_server &server, char *p, int req_id, int status);
    void service5(udp_server &server, char *p, int req_id, int status);
    void service6(udp_server &server, char *p, int req_id, int status);
    void service7(udp_server &server, char *p, int req_id, int status);

    Handler();
};
#endif
