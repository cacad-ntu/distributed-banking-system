#include "Handler.h"

Handler::Handler(){
    acManager = AccountManager();
}

void Handler::notify(udp_server &server, string s){
    if((int)admins.size() == 0) return;
    
    char header[HEADER_SIZE];
    utils::marshalInt(4 + (int)s.size(), header);

    char *response = new char[4+(int)s.size()];
    char *cur = response;

    utils::marshalInt((int)s.size(),cur);
    cur += 4;
    
    utils::marshalString(s,cur);
    cur += (int)s.size();

    while(admins.size() && !admins[0].isAvailable())
        admins.pop_front();
    
    for(auto admin:admins){
        server.send(header,HEADER_SIZE,admin.getAddress(),admin.getLength());
        server.send(response,4+(int)s.size(),admin.getAddress(),admin.getLength());
    }
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

    notify(server,"Opened a new account with name " + name + ", currency " + to_string(currency) + ", balance " + to_string(balance) + ".");
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
   
    
    if(success){
        char response[1];
        char *cur = response;
        cout << "Success deleting account\n";
        utils::marshalInt(1,header);
        utils::marshalString(ACK_SUCCESS,cur);

        server.send(header,HEADER_SIZE);
        server.send(response,1);

        notify(server,"Deleted account no. " + to_string(accountNum) + " (" + name + ").");
    }
    else{
        
        cout << "Unsuccessful deletion\n";
        string err = "Wrong account number, name, or password!";

        char *response = new char[1+4+(int)err.size()];
        char *cur = response;
        
        utils::marshalInt(1+4+(int)err.size(),header);

        utils::marshalString(ACK_FAIL,cur);
        cur += 1;

        utils::marshalInt((int)err.size(),cur);
        cur += 4;

        utils::marshalString(err,cur);

        cout << "sending\n";

        server.send(header,HEADER_SIZE);

        cout << "header sent\n";
        
        server.send(response,1+4+(int)err.size());

        cout << "response sent\n";
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

        notify(server,"Deposited " + to_string(amount) + " of currency " + to_string(currency) + " to account no. " + to_string(accountNum) + " (" + name + ") .");
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

        notify(server,"Withdrawn " + to_string(amount) + " of currency " + to_string(currency) + " from account no. " + to_string(accountNum) + " (" + name + ") .");
    }
}

void Handler::service5(udp_server &server, char *p){

    /*
      ### Service 5 - Monitor updates to all updates made to all bank account

      ##### Message
      - Interval in milliseconds (Integer)

      ##### Response
      - Account Update
      - Message Type
      - Details
     */
    
    int interval;

    int length = utils::unmarshalInt(p);
    p += 4;
    interval = utils::unmarshalInt(p);
    p += length;

    auto clientAddress = server.getClientAddress();
    auto clientLength  = server.getClientLength();
    auto start = std::chrono::high_resolution_clock::now();

    admins.push_back(Admin(clientAddress, clientLength, start, interval));
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

        notify(server,"Transferred " + to_string(amount) + " of currency " + to_string(currency) + " from account no. " + to_string(accountNum1) + " (" + name1 + ") to account no. " + to_string(accountNum2) + " (" + name2 + ") .");
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

    notify(server,"Changed password of account no. " + to_string(accountNum) + " (" + name + ").");
}
