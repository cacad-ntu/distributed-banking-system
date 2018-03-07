#include "handlers.h"

void service1Handler(udp_server &server, char *p){
    int currency;
    float balance;
    string passw, name;

    int length = utils::unmarshalInt(p);
    p += 4;
    currency = utils::unmarshalInt(p);
    p += length;

    length = utils::unmarshalInt(p);
    p += 4;
    balance = utils::unmarshalFloat(p);
    p += length;

    length = utils::unmarshalInt(p);
    p += 4;
    passw = utils::unmarshalString(p,length);
    p += length;

    length = utils::unmarshalInt(p);
    p += 4;
    name = utils::unmarshalString(p,length);
    p += length;
}

void service2Handler(udp_server &server, char *p){
    int accountNum;
    string passw, name;

    int length = utils::unmarshalInt(p);
    p += 4;
    accountNum = utils::unmarshalInt(p);
    p += length;

    length = utils::unmarshalInt(p);
    p += 4;
    passw = utils::unmarshalString(p,length);
    p += length;

    length = utils::unmarshalInt(p);
    p += 4;
    name = utils::unmarshalString(p,length);
    p += length;
}

void service3Handler(udp_server &server, char *p){
    int isDeposit, accountNum, currency;
    float amount;
    string passw, name;

    int length = utils::unmarshalInt(p);
    p += 4;
    isDeposit = utils::unmarshalInt(p);
    p += length;

    length = utils::unmarshalInt(p);
    p += 4;
    accountNum = utils::unmarshalInt(p);
    p += length;

    length = utils::unmarshalInt(p);
    p += 4;
    currency = utils::unmarshalInt(p);
    p += length;
    
    length = utils::unmarshalInt(p);
    p += 4;
    amount = utils::unmarshalFloat(p);
    p += length;

    length = utils::unmarshalInt(p);
    p += 4;
    passw = utils::unmarshalString(p,length);
    p += length;

    length = utils::unmarshalInt(p);
    p += 4;
    name = utils::unmarshalString(p,length);
    p += length;
}

void service4Handler(udp_server &server, char *p){
    int interval;

    int length = utils::unmarshalInt(p);
    p += 4;
    interval = utils::unmarshalInt(p);
    p += length;
}

void service5Handler(udp_server &server, char *p){
    int accountNum1, accountNum2, currency;
    float amount;
    string passw, name1, name2;

    int length = utils::unmarshalInt(p);
    p += 4;
    accountNum1 = utils::unmarshalInt(p);
    p += length;

    length = utils::unmarshalInt(p);
    p += 4;
    accountNum2 = utils::unmarshalInt(p);
    p += length;
    
    length = utils::unmarshalInt(p);
    p += 4;
    currency = utils::unmarshalInt(p);
    p += length;
    
    length = utils::unmarshalInt(p);
    p += 4;
    amount = utils::unmarshalFloat(p);
    p += length;

    length = utils::unmarshalInt(p);
    p += 4;
    passw = utils::unmarshalString(p,length);
    p += length;

    length = utils::unmarshalInt(p);
    p += 4;
    name1 = utils::unmarshalString(p,length);
    p += length;

    length = utils::unmarshalInt(p);
    p += 4;
    name2 = utils::unmarshalString(p,length);
    p += length;
}

void service6Handler(udp_server &server, char *p){
    int accountNum;
    string passw, newPassw, name;

    int length = utils::unmarshalInt(p);
    p += 4;
    accountNum = utils::unmarshalInt(p);
    p += length;

    length = utils::unmarshalInt(p);
    p += 4;
    passw = utils::unmarshalString(p,length);
    p += length;

    length = utils::unmarshalInt(p);
    p += 4;
    newPassw = utils::unmarshalString(p,length);
    p += length;

    length = utils::unmarshalInt(p);
    p += 4;
    name = utils::unmarshalString(p,length);
    p += length;
}
