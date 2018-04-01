#include "Handler.h"

Handler::Handler(){
    acManager = AccountManager();
    response_id = 0;
}

int Handler::getResponseID(){
    return response_id++;
}

void Handler::notify(udp_server &server, string s, int status){
    if((int)admins.size() == 0) return;
    
    char header[HEADER_SIZE];
    utils::marshalInt(4 + 1 + 4 + 4 + 4 + (int)s.size(), header);

    char *response = new char[4+1+4+4+4+(int)s.size()];

    while(admins.size() && !admins[0].isAvailable())
        admins.pop_front();

    char ackheader[HEADER_SIZE];
    char ack[1];
    
    for(auto admin:admins){
        char *cur = response;
        
        int responseID = getResponseID();
        
        utils::marshalInt(responseID,cur);
        cur += 4;

        utils::marshalString(ACK_SUCCESS,cur);
        cur += 1;

        utils::marshalInt(4,cur);
        cur += 4;

        char *rem = cur;
        utils::marshalInt(admin.getRemaining(),cur);
        cur += 4;
        
        utils::marshalInt((int)s.size(),cur);
        cur += 4;
    
        utils::marshalString(s,cur);
        cur += (int)s.size();
        
        server.send(header,HEADER_SIZE,admin.getAddress(),admin.getLength());
        server.send(response,4+1+4+4+4+(int)s.size(),admin.getAddress(),admin.getLength());

        if(status >= 1){
            while(admin.isAvailable()){
                cout << "WAITING FOR ACK HEADER\n";
                int n = server.receive_time(header,HEADER_SIZE,1);
                if(n <= 0){
                    if(admin.isAvailable()){
                        utils::marshalInt(admin.getRemaining(),rem);
                        server.send(header,HEADER_SIZE);
                        server.send(response,4+1+4+4+4+(int)s.size());
                        continue;
                    }
                    else break;
                }
                char* ack = new char[5];

                cout << "WAITING FOR ACK\n";
                n = server.receive_time(ack,5,1);
                if(n <= 0){
                    if(admin.isAvailable()){
                        utils::marshalInt(admin.getRemaining(),rem);
                        server.send(header,HEADER_SIZE);
                        server.send(response,4+1+4+4+4+(int)s.size());
                        continue;
                    }
                    else break;
                }
                char *x = ack;
                x += 1;
                int ack_id = utils::unmarshalInt(x);
                if(ack_id == responseID) break;
                else cout << "ID mismatch!\nACK ID: " << ack_id << "\nResponse ID: " << responseID << "\n";
            }
        }
    }
}

void Handler::ackHandler(udp_server &server, char *header, char *response, int responseSize, int responseID){
    while(1){
        cout << "WAITING FOR ACK HEADER\n";
        int n = server.receive_time(header,HEADER_SIZE,1);
        if(n <= 0){
            cout << "Resending header:" << utils::unmarshalInt(header) << "..\n";
            server.send(header,HEADER_SIZE);
            server.send(response,responseSize);
            continue;
        }
        char* ack = new char[1+4];

        cout << "WAITING FOR ACK\n";
        n = server.receive_time(ack,1+4,1);
        
        if(n <= 0){
            cout << "Resending response..\n";
            server.send(header,HEADER_SIZE);
            server.send(response,responseSize);
            continue;
        }
        
        char *x = ack;
        x += 1;
        int ack_id = utils::unmarshalInt(x);
        if(ack_id == responseID) break;
        else{
            cout << "ID mismatch!\nACK ID: " << ack_id << "\nResponse ID: " << responseID << "\n";
        }
    }
}

void Handler::service1(udp_server &server, char *p, int req_id, int status){
    unsigned long cAddress = server.getClientAddress().sin_addr.s_addr;

    if(status == 2 && memo.count({cAddress,req_id})){
        char header[HEADER_SIZE];
        char *response = memo[{cAddress,req_id}];
        
        utils::marshalInt(sizeof(response),header);
        server.send(header,HEADER_SIZE);
        server.send(response,sizeof(response));
        return; 
    }
    
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
    cout << "Created account with account no. " << accountNum << '\n';
    
    char header[HEADER_SIZE];
    char response[4+1+4+4];

    utils::marshalInt(4+1+4+4,header);
    char *cur = response;

    int responseID = getResponseID();
    utils::marshalInt(responseID,cur);
    cur += 4;

    utils::marshalString(ACK,cur);
    cur += 1;

    utils::marshalInt(4,cur);
    cur += 4;

    utils::marshalInt(accountNum,cur);

    if(status == 2) memo[{cAddress,req_id}] = response;
    
    server.send(header,HEADER_SIZE);
    server.send(response,4+1+4+4);

    if(status >= 1) ackHandler(server, header, response, 4+1+4+4, responseID);
    
    notify(server,"Opened a new account with name " + name + ", currency " + to_string(currency) + ", balance " + to_string(balance) + ".", status);
}

void Handler::service2(udp_server &server, char *p, int req_id, int status){
    unsigned long cAddress = server.getClientAddress().sin_addr.s_addr;

    if(status == 2 && memo.count({cAddress,req_id})){
        char header[HEADER_SIZE];
        char *response = memo[{cAddress,req_id}];
        
        utils::marshalInt(sizeof(response),header);
        server.send(header,HEADER_SIZE);
        server.send(response,sizeof(response));
        return; 
    }
    
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
        char *response = new char[4+1];
        char *cur = response;

        cout << "Success deleting account\n";

        utils::marshalInt(4+1,header);

        int responseID = getResponseID();
        utils::marshalInt(responseID,cur);
        cur += 4;

        utils::marshalString(ACK_SUCCESS,cur);

        if(status == 2) memo[{cAddress,req_id}] = response;

        server.send(header,HEADER_SIZE);
        server.send(response,4+1);

        if(status >= 1) ackHandler(server, header, response, 4+1, responseID);
        
        notify(server,"Deleted account no. " + to_string(accountNum) + " (" + name + ").", status);
    }
    else{
        
        cout << "Unsuccessful deletion\n";
        string err = "Wrong account number, name, or password!";

        char *response = new char[4+1+4+(int)err.size()];
        char *cur = response;

        utils::marshalInt(4+1+4+(int)err.size(),header);

        int responseID = getResponseID();
        utils::marshalInt(responseID,cur);
        cur += 4;
        
        utils::marshalString(ACK_FAIL,cur);
        cur += 1;

        utils::marshalInt((int)err.size(),cur);
        cur += 4;

        utils::marshalString(err,cur);

        if(status == 2) memo[{cAddress,req_id}] = response;

        server.send(header,HEADER_SIZE);        
        server.send(response,4+1+4+(int)err.size());

        if(status >= 1) ackHandler(server, header, response, 4+1+4+(int)err.size(), responseID);
    }
}

void Handler::service3(udp_server &server, char *p, int req_id, int status){
    unsigned long cAddress = server.getClientAddress().sin_addr.s_addr;

    if(status == 2 && memo.count({cAddress,req_id})){
        char header[HEADER_SIZE];
        char *response = memo[{cAddress,req_id}];
        
        utils::marshalInt(sizeof(response),header);
        server.send(header,HEADER_SIZE);
        server.send(response,sizeof(response));
        return; 
    }
    
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
        
        utils::marshalInt(4+1+4+(int)err.size(),header);

        char *response = new char[4+1+4+(int)err.size()];
        char *cur = response;

        int responseID = getResponseID();
        utils::marshalInt(responseID,cur);
        cur += 4;

        utils::marshalString(ACK_FAIL,cur);
        cur += 1;

        utils::marshalInt((int)err.size(),cur);
        cur += 4;
            
        utils::marshalString(err,cur);
        cur += (int)err.size();

        if(status == 2) memo[{cAddress,req_id}] = response;

        server.send(header,HEADER_SIZE);
        server.send(response,4+1+4+(int)err.size());

        if(status >= 1) ackHandler(server, header, response, 4+1+4+(int)err.size(), responseID);
    }
    else{
        char response[4+1+4+4+4+4];
        char *cur = response;

        utils::marshalInt(4+1+4+4+4+4,header);

        int responseID = getResponseID();
        utils::marshalInt(responseID,cur);
        cur += 4;

        utils::marshalString(ACK_SUCCESS,cur);
        cur += 1;

        utils::marshalInt(4,cur);
        cur += 4;

        utils::marshalInt(balance.first,cur);
        cur += 4;

        utils::marshalInt(4,cur);
        cur += 4;
        
        utils::marshalFloat(balance.second,cur);
        
        if(status == 2) memo[{cAddress,req_id}] = response;
        
        server.send(header,HEADER_SIZE);
        server.send(response,4+1+4+4+4+4);

        if(status >= 1) ackHandler(server, header, response, 4+1+4+4+4+4, responseID);
        
        notify(server,"Deposited " + to_string(amount) + " of currency " + to_string(currency) + " to account no. " + to_string(accountNum) + " (" + name + ") .", status);
    }
}

void Handler::service4(udp_server &server, char *p, int req_id, int status){
    unsigned long cAddress = server.getClientAddress().sin_addr.s_addr;

    if(status == 2 && memo.count({cAddress,req_id})){
        char header[HEADER_SIZE];
        char *response = memo[{cAddress,req_id}];
        
        utils::marshalInt(sizeof(response),header);
        server.send(header,HEADER_SIZE);
        server.send(response,sizeof(response));
        return; 
    }
    
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
        
        utils::marshalInt(4+1+4+(int)err.size(),header);

        char *response = new char[4+1+4+(int)err.size()];
        char *cur = response;

        int responseID = getResponseID();
        utils::marshalInt(responseID,cur);
        cur += 4;
        
        utils::marshalString(ACK_FAIL,cur);
        cur += 1;

        utils::marshalInt((int)err.size(),cur);
        cur += 4;
            
        utils::marshalString(err,cur);
        cur += (int)err.size();

        if(status == 2) memo[{cAddress,req_id}] = response;

        server.send(header,HEADER_SIZE);
        server.send(response,4+1+4+(int)err.size());

        if(status >= 1) ackHandler(server, header, response, 4+1+4+(int)err.size(), responseID);

    }
    else{
        char response[4+1+4+4+4+4];
        char *cur = response;

        utils::marshalInt(4+1+4+4+4+4,header);

        int responseID = getResponseID();
        utils::marshalInt(responseID,cur);
        cur += 4;
        
        utils::marshalString(ACK_SUCCESS,cur);
        cur += 1;

        utils::marshalInt(4,cur);
        cur += 4;

        utils::marshalInt(balance.first,cur);
        cur += 4;

        utils::marshalInt(4,cur);
        cur += 4;
        
        utils::marshalFloat(balance.second,cur);

        if(status == 2) memo[{cAddress,req_id}] = response;
        
        server.send(header,HEADER_SIZE);
        server.send(response,4+1+4+4+4+4);

        if(status >= 1) ackHandler(server, header, response, 4+1+4+4+4+4, responseID);
        
        notify(server,"Withdrawn " + to_string(amount) + " of currency " + to_string(currency) + " from account no. " + to_string(accountNum) + " (" + name + ") .", status);
    }
}

void Handler::service5(udp_server &server, char *p, int req_id, int status){
    cout << "Beginning service 5\n";
    unsigned long cAddress = server.getClientAddress().sin_addr.s_addr;

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

    cout << "Obtaining client info..\n";
    
    auto clientAddress = server.getClientAddress();
    auto clientLength  = server.getClientLength();

    cout << "Starting clock..\n";
    auto start = std::chrono::high_resolution_clock::now();

    cout << "Creating new admin..\n";
    
    Admin newAdmin(clientAddress, clientLength, start, interval);
    admins.push_back(newAdmin);

    cout << "Marshalling\n";

    char header[HEADER_SIZE];
    char response[4+1+4+4];
    char *cur = response;
    utils::marshalInt(4+1+4+4,header);

    int responseID = getResponseID();
    utils::marshalInt(responseID,cur);
    cur += 4;

    utils::marshalString(ACK_SUCCESS,cur);
    cur += 1;

    utils::marshalInt(4,cur);
    cur += 4;
    
    utils::marshalInt(newAdmin.getRemaining(),cur);

    if(status == 2) memo[{cAddress,req_id}] = response;

    cout << "Sending response\n";
    
    server.send(header,HEADER_SIZE);
    server.send(response,4+1+4+4);

    cout << "Handling ack\n";
    
    if(status >= 1) ackHandler(server, header, response, 4+1+4+4, responseID);
}

void Handler::service6(udp_server &server, char *p, int req_id, int status){
    unsigned long cAddress = server.getClientAddress().sin_addr.s_addr;

    if(status == 2 && memo.count({cAddress,req_id})){
        char header[HEADER_SIZE];
        char *response = memo[{cAddress,req_id}];
        
        utils::marshalInt(sizeof(response),header);
        server.send(header,HEADER_SIZE);
        server.send(response,sizeof(response));
        return; 
    }
    
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
        
        utils::marshalInt(4+1+4+(int)err.size(),header);

        char *response = new char[4+1+4+(int)err.size()];
        char *cur = response;

        int responseID = getResponseID();
        utils::marshalInt(responseID,cur);
        cur += 4;
        
        utils::marshalString(ACK_FAIL,cur);
        cur += 1;

        utils::marshalInt((int)err.size(),cur);
        cur += 4;
            
        utils::marshalString(err,cur);
        cur += (int)err.size();

        if(status == 2) memo[{cAddress,req_id}] = response;

        server.send(header,HEADER_SIZE);
        server.send(response,4+1+4+(int)err.size());

        if(status >= 1) ackHandler(server, header, response, 4+1+4+(int)err.size(), responseID);
    }
    else{
        char response[4+1+4+4+4+4];
        char *cur = response;

        int responseID = getResponseID();
        utils::marshalInt(responseID,cur);
        cur += 4;

        utils::marshalString(ACK_SUCCESS,cur);
        cur += 1;

        utils::marshalInt(4,cur);
        cur += 4;

        utils::marshalInt(balance.first,cur);
        cur += 4;

        utils::marshalInt(4,cur);
        cur += 4;
        
        utils::marshalFloat(balance.second,cur);

        utils::marshalInt(4+1+4+4+4+4,header);

        if(status == 2) memo[{cAddress,req_id}] = response;
    
        server.send(header,HEADER_SIZE);
        server.send(response,4+1+4+4+4+4);
        if(status >= 1) ackHandler(server, header, response, 4+1+4+4+4+4, responseID);
        notify(server,"Transferred " + to_string(amount) + " of currency " + to_string(currency) + " from account no. " + to_string(accountNum1) + " (" + name1 + ") to account no. " + to_string(accountNum2) + " (" + name2 + ") .", status);
    }
}

void Handler::service7(udp_server &server, char *p, int req_id, int status){
    unsigned long cAddress = server.getClientAddress().sin_addr.s_addr;

    if(status == 2 && memo.count({cAddress,req_id})){
        char header[HEADER_SIZE];
        char *response = memo[{cAddress,req_id}];
        
        utils::marshalInt(sizeof(response),header);
        server.send(header,HEADER_SIZE);
        server.send(response,sizeof(response));
        return; 
    }
    
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
    char response[4+1];

    utils::marshalInt(4+1,header);
    char *cur = response;

    int responseID = getResponseID();
    utils::marshalInt(responseID,cur);
    cur += 4;
    
    if(success) utils::marshalString(ACK_SUCCESS,cur);
    else utils::marshalString(ACK_FAIL,cur);

    if(status == 2) memo[{cAddress,req_id}] = response;
    
    server.send(header,HEADER_SIZE);
    server.send(response,4+1);

    if(status >= 1) ackHandler(server, header, response, 4+1, responseID);
    
    notify(server,"Changed password of account no. " + to_string(accountNum) + " (" + name + ").", status);
}
