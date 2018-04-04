#ifndef HANDLERS
#define HANDLERS

#include <iostream>
#include "udp_server.h"
#include "utils.h"
#include "AccountManager.h"
#include "constants.h"
#include <chrono>
#include <deque>
#include <string>
#include <map>
#include "Admin.h"
using namespace std;

class Handler{
 private:
    deque<Admin> admins;
    AccountManager acManager;
    map<pair<unsigned long,int>,string> memo;
    
    int response_id;
    int limit;
    unsigned seed;
    mt19937 generator;
    std::uniform_real_distribution<double> distribution;

    double failureRate;
 public:
    void ackHandler(udp_server &server, char *header, char *response, int responseSize, int responseID, int status, unsigned long cAddress);
    int getResponseID();
    void notify(udp_server &server, string s, int status);
    int getS1AccNum(char *response);
    void service1(udp_server &server, char *p, int req_id, int status);
    void service2(udp_server &server, char *p, int req_id, int status);
    void service3(udp_server &server, char *p, int req_id, int status);
    void service4(udp_server &server, char *p, int req_id, int status);
    void service5(udp_server &server, char *p, int req_id, int status);
    void service6(udp_server &server, char *p, int req_id, int status);
    void service7(udp_server &server, char *p, int req_id, int status);
    bool checkAndSendOldResponse(udp_server &server, unsigned long cAddress, int req_id);
    void sendReply(udp_server &server, char *header, char *response, int respondSize);
    Handler(int _limit, double _failureRate);
};
#endif
