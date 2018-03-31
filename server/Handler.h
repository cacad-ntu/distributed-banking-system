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
    void notify(udp_server &server, string s);
    void service1(udp_server &server, char *p, int req_id, bool at_most_one);
    void service2(udp_server &server, char *p, int req_id, bool at_most_one);
    void service3(udp_server &server, char *p, int req_id, bool at_most_one);
    void service4(udp_server &server, char *p, int req_id, bool at_most_one);
    void service5(udp_server &server, char *p, int req_id, bool at_most_one);
    void service6(udp_server &server, char *p, int req_id, bool at_most_one);
    void service7(udp_server &server, char *p, int req_id, bool at_most_one);

    Handler();
};
#endif
