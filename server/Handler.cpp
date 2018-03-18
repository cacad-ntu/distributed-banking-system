#include "Handler.h"

Handler::Handler(){
    acManager = AccountManager();
}

void Handler::service1(udp_server &server, char *p){
    int currency;
    float balance;
    string passw, name;

    int length = utils::unmarshalInt(p);
    p += 4;
    name = utils::unmarshalString(p,length);
    p += length;
    
    length = utils::unmarshalInt(p);
    p += 4;
    passw = utils::unmarshalString(p,length);
    p += length;

    length = utils::unmarshalInt(p);
    p += 4;
    currency = utils::unmarshalInt(p);
    p += length;

    length = utils::unmarshalInt(p);
    p += 4;
    balance = utils::unmarshalFloat(p);
    p += length;

    int accountNum = acManager.createAccount(name,passw,currency,balance);

    char header[HEADER_SIZE];
    char response[9];

    utils::marshalInt(9,header);
    char *cur = response;
    utils::marshalString(ACK,cur);
    cur += 1;
    utils::marshalInt(4,cur);
    cur += 4;
    utils::marshalInt(accountNum,cur);
    
    server.send(header,HEADER_SIZE);
    server.send(response,9);
}

void Handler::service2(udp_server &server, char *p){
    int accountNum;
    string passw, name;

    int length = utils::unmarshalInt(p);
    p += 4;
    name = utils::unmarshalString(p,length);
    p += length;
    
    length = utils::unmarshalInt(p);
    p += 4;
    accountNum = utils::unmarshalInt(p);
    p += length;

    length = utils::unmarshalInt(p);
    p += 4;
    passw = utils::unmarshalString(p,length);
    p += length;

    bool success = acManager.deleteAccount(accountNum,name,passw);

    char header[HEADER_SIZE];
    char response[1];

    //utils::marshalInt(1,header);
    char *cur = response;
    
    if(success){
        utils::marshalInt(1,header);
        utils::marshalString(ACK_SUCCESS,cur);

        server.send(header,HEADER_SIZE);
        server.send(response,1);
    }
    else{
        string err = "Wrong account number, name, or password!";
        utils::marshalInt(1+4+(int)err.size(),header);

        utils::marshalString(ACK_FAIL,cur);
        cur += 1;

        utils::marshalInt((int)err.size(),cur);
        cur += 4;

        utils::marshalString(err,cur);

        server.send(header,HEADER_SIZE);
        server.send(response,1+4+(int)err.size());
    }
}

void Handler::service3(udp_server &server, char *p){
    int accountNum, currency;
    float amount;
    string passw, name;
    
    int length = utils::unmarshalInt(p);
    p += 4;
    name = utils::unmarshalString(p,length);
    p += length;

    length = utils::unmarshalInt(p);
    p += 4;
    accountNum = utils::unmarshalInt(p);
    p += length;

    length = utils::unmarshalInt(p);
    p += 4;
    passw = utils::unmarshalString(p,length);
    p += length;

    length = utils::unmarshalInt(p);
    p += 4;
    currency = utils::unmarshalInt(p);
    p += length;
    
    length = utils::unmarshalInt(p);
    p += 4;
    amount = utils::unmarshalFloat(p);
    p += length;

    pair<int,float> balance = acManager.deposit(accountNum,name,passw,currency,amount);

    char header[HEADER_SIZE];

    if(balance.first < 0){
        string err;
        if(balance.first == -1) err = "Account number not found!";
        else if(balance.first == -2) err = "Wrong name!";
        else if(balance.first == -3) err = "Wrong password!";
        else if(balance.first == -4) err = "Currency mismatch!";
        else err = "Unknown error!";
        
        utils::marshalInt(1+4+(int)err.size(),header);

        char *response = new char[1+4+(int)err.size()];
        char *cur = response;

        utils::marshalString(ACK_FAIL,cur);
        cur += 1;

        utils::marshalInt((int)err.size(),cur);
        cur += 4;
            
        utils::marshalString(err,cur);
        cur += (int)err.size();

        server.send(header,HEADER_SIZE);
        server.send(response,1+4+(int)err.size());
    }
    else{
        char response[17];
        char *cur = response;

        utils::marshalString(ACK_SUCCESS,cur);
        cur += 1;

        utils::marshalInt(4,cur);
        cur += 4;

        utils::marshalInt(balance.first,cur);
        cur += 4;

        utils::marshalInt(4,cur);
        cur += 4;
        
        utils::marshalFloat(balance.second,cur);

        utils::marshalInt(17,header);
        server.send(header,HEADER_SIZE);
        server.send(response,17);
    }
}

void Handler::service4(udp_server &server, char *p){
    int accountNum, currency;
    float amount;
    string passw, name;
    
    int length = utils::unmarshalInt(p);
    p += 4;
    name = utils::unmarshalString(p,length);
    p += length;

    length = utils::unmarshalInt(p);
    p += 4;
    accountNum = utils::unmarshalInt(p);
    p += length;

    length = utils::unmarshalInt(p);
    p += 4;
    passw = utils::unmarshalString(p,length);
    p += length;

    length = utils::unmarshalInt(p);
    p += 4;
    currency = utils::unmarshalInt(p);
    p += length;
    
    length = utils::unmarshalInt(p);
    p += 4;
    amount = utils::unmarshalFloat(p);
    p += length;

    pair<int,float> balance = acManager.withdraw(accountNum,name,passw,currency,amount);

    char header[HEADER_SIZE];

    if(balance.first < 0){
        string err;
        if(balance.first == -11) err = "Account number not found!";
        else if(balance.first == -12) err = "Wrong name!";
        else if(balance.first == -13) err = "Wrong password!";
        else if(balance.first == -14) err = "Currency mismatch!";
        else if(balance.first == -15) err = "Unable to withdraw amount higher than balance!";
        else err = "Unknown error!";
        
        utils::marshalInt(1+4+(int)err.size(),header);

        char *response = new char[1+4+(int)err.size()];
        char *cur = response;

        utils::marshalString(ACK_FAIL,cur);
        cur += 1;

        utils::marshalInt((int)err.size(),cur);
        cur += 4;
            
        utils::marshalString(err,cur);
        cur += (int)err.size();

        server.send(header,HEADER_SIZE);
        server.send(response,1+4+(int)err.size());
    }
    else{
        char response[17];
        char *cur = response;

        utils::marshalString(ACK_SUCCESS,cur);
        cur += 1;

        utils::marshalInt(4,cur);
        cur += 4;

        utils::marshalInt(balance.first,cur);
        cur += 4;

        utils::marshalInt(4,cur);
        cur += 4;
        
        utils::marshalFloat(balance.second,cur);

        utils::marshalInt(17,header);
        server.send(header,HEADER_SIZE);
        server.send(response,17);
    }
}

void Handler::service5(udp_server &server, char *p){
    int interval;

    int length = utils::unmarshalInt(p);
    p += 4;
    interval = utils::unmarshalInt(p);
    p += length;
}

void Handler::service6(udp_server &server, char *p){
    int accountNum1, accountNum2, currency;
    float amount;
    string passw, name1, name2;

    int length = utils::unmarshalInt(p);
    p += 4;
    name1 = utils::unmarshalString(p,length);
    p += length;
    
    length = utils::unmarshalInt(p);
    p += 4;
    accountNum1 = utils::unmarshalInt(p);
    p += length;

    length = utils::unmarshalInt(p);
    p += 4;
    name2 = utils::unmarshalString(p,length);
    p += length;

    length = utils::unmarshalInt(p);
    p += 4;
    accountNum2 = utils::unmarshalInt(p);
    p += length;

    length = utils::unmarshalInt(p);
    p += 4;
    passw = utils::unmarshalString(p,length);
    p += length;
    
    length = utils::unmarshalInt(p);
    p += 4;
    currency = utils::unmarshalInt(p);
    p += length;
    
    length = utils::unmarshalInt(p);
    p += 4;
    amount = utils::unmarshalFloat(p);
    p += length;

    pair<int,float> balance = acManager.transfer(accountNum1,accountNum2,name1,name2,passw,currency,amount);
    char header[HEADER_SIZE];

    if(balance.first < 0){
        string err;
        if(balance.first == -1) err = "Account number not found!";
        else if(balance.first == -2) err = "Wrong name!";
        else if(balance.first == -3) err = "Wrong password!";
        else if(balance.first == -4) err = "Currency mismatch!";
        else if(balance.first == -11) err = "Account number not found!";
        else if(balance.first == -12) err = "Wrong name!";
        else if(balance.first == -13) err = "Wrong password!";
        else if(balance.first == -14) err = "Currency mismatch!";
        else if(balance.first == -15) err = "Unable to withdraw amount higher than balance!";
        else if(balance.first == -21) err = "Account number not found!";
        else if(balance.first == -22) err = "Account number of recipient not found!";
        else if(balance.first == -23) err = "Wrong name!";
        else if(balance.first == -24) err = "Wrong recipient name!";
        else if(balance.first == -25) err = "Wrong password!";
        else if(balance.first == -26) err = "Currency mismatch!";
        else if(balance.first == -27) err = "Recipient currency mismatch!";
        else err = "Unknown error!";
        
        utils::marshalInt(1+4+(int)err.size(),header);

        char *response = new char[1+4+(int)err.size()];
        char *cur = response;

        utils::marshalString(ACK_FAIL,cur);
        cur += 1;

        utils::marshalInt((int)err.size(),cur);
        cur += 4;
            
        utils::marshalString(err,cur);
        cur += (int)err.size();

        server.send(header,HEADER_SIZE);
        server.send(response,1+4+(int)err.size());
    }
    else{
        char response[17];
        char *cur = response;

        utils::marshalString(ACK_SUCCESS,cur);
        cur += 1;

        utils::marshalInt(4,cur);
        cur += 4;

        utils::marshalInt(balance.first,cur);
        cur += 4;

        utils::marshalInt(4,cur);
        cur += 4;
        
        utils::marshalFloat(balance.second,cur);

        utils::marshalInt(17,header);
    
        server.send(header,HEADER_SIZE);
        server.send(response,17);
    }
}

void Handler::service7(udp_server &server, char *p){
    int accountNum;
    string passw, newPassw, name;

    int length = utils::unmarshalInt(p);
    p += 4;
    name = utils::unmarshalString(p,length);
    p += length;
    
    length = utils::unmarshalInt(p);
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

    bool success = acManager.changePassword(accountNum,name,passw,newPassw);

    char header[HEADER_SIZE];
    char response[1];

    utils::marshalInt(1,header);
    char *cur = response;
    if(success) utils::marshalString(ACK_SUCCESS,cur);
    else utils::marshalString(ACK_FAIL,cur);
    
    server.send(header,HEADER_SIZE);
    server.send(response,1);
}
