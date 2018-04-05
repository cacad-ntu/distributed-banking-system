#include "Handler.h"

Handler::Handler(int _limit, double _failureRate){
    acManager = AccountManager();
    response_id = 0;
    limit = _limit;

    seed = chrono::system_clock::now().time_since_epoch().count();
    generator = mt19937(seed);
    distribution = uniform_real_distribution<double> (0.0,1.0);
    failureRate = _failureRate;
}

/**
   Increments response ID
 */
int Handler::getResponseID(){
    return response_id++;
}

/**
   Sends response under certain failure rate
 */
void Handler::sendReply(udp_server &server, char *header, char *response, int responseSize){   
    if(distribution(generator) > failureRate){
        server.send(header,HEADER_SIZE);
        server.send(response, responseSize);
        cout << ">>>>>>>>>>Response id " << utils::unmarshalInt(response) << " of length " << responseSize << " is sent\n";
    }
    else cout << "################################Failure simulated#####################################\n";
}

/**
   Notifies monitoring users by sending notification string 
 */
void Handler::notify(udp_server &server, string notif, int status){
    if((int)admins.size() == 0) return;
    
    char header[HEADER_SIZE];
    utils::marshalInt(ID_SIZE + STATUS_SIZE + 3*INT_SIZE + (int)notif.size(), header);

    char *response = new char[ID_SIZE + STATUS_SIZE + 3*INT_SIZE + (int)notif.size()];

    while(admins.size() && !admins[0].isAvailable())
        admins.pop_front();

    
    for(auto admin:admins){
        char *cur = response;
        
        int responseID = getResponseID();
        
        utils::marshalInt(responseID,cur);
        cur += INT_SIZE;

        utils::marshalString(ACK_SUCCESS,cur);
        cur += STATUS_SIZE;

        utils::marshalInt(INT_SIZE,cur);
        cur += INT_SIZE;

        char *rem = cur;
        utils::marshalInt(admin.getRemaining(),cur);
        cur += INT_SIZE;
        
        utils::marshalInt((int)notif.size(),cur);
        cur += INT_SIZE;
    
        utils::marshalString(notif,cur);
        cur += (int)notif.size();
        
        server.send(header,HEADER_SIZE,admin.getAddress(),admin.getLength());
        server.send(response,ID_SIZE + STATUS_SIZE + 3*INT_SIZE + (int)notif.size(),admin.getAddress(),admin.getLength());

        if(status == 2){
            while(admin.isAvailable()){
                char ackHeader[HEADER_SIZE];

                cout << "WAITING FOR ACK HEADER\n";
                int n = server.receive_time(ackHeader,HEADER_SIZE,RECEIVE_TIMEOUT);
                
                if(n <= 0){
                    if(admin.isAvailable()){
                        utils::marshalInt(admin.getRemaining(),rem);
                        server.send(header,HEADER_SIZE,admin.getAddress(),admin.getLength());
                        server.send(response,ID_SIZE + STATUS_SIZE + 3*INT_SIZE + (int)notif.size(),admin.getAddress(),admin.getLength());
                        continue;
                    }
                    else break;
                }

                int ackSize = utils::unmarshalInt(ackHeader);
                char* ack = new char[ackSize];

                cout << "WAITING FOR ACK\n";
                n = server.receive_time(ack,ackSize,RECEIVE_TIMEOUT);
                if(n <= 0){
                    if(admin.isAvailable()){
                        utils::marshalInt(admin.getRemaining(),rem);
                        server.send(header,HEADER_SIZE,admin.getAddress(),admin.getLength());
                        server.send(response,ID_SIZE + STATUS_SIZE + 3*INT_SIZE + (int)notif.size(),admin.getAddress(),admin.getLength());
                        continue;
                    }
                    else break;
                }
                char *x = ack;
                int ack_id = utils::unmarshalInt(x);
                x += ID_SIZE;
                
                if(ack_id == responseID) break;
                else if(status == 2 && checkAndSendOldResponse(server,admin.getAddress().sin_addr.s_addr,ack_id)){
                    cout << "Old request ID received instead!\n";
                    cout << "Old response sent..!\n";
                    cout << "Waiting for ack again..\n";
                    continue;
                }
                else{
                    cout << "ID mismatch!\nACK ID: " << ack_id << "\nResponse ID: " << responseID << "\n";
                }
            }
        }
    }
}

/**
   Repeatedly listens for incoming ack and resends response each timeout
 */
void Handler::ackHandler(udp_server &server, char *header, char *response, int responseSize, int responseID, int status, unsigned long cAddress){
    char ackHeader[HEADER_SIZE];

    for(int i=1; i!=limit+1; i++){

        cout << "WAITING FOR ACK HEADER\n";
        int n = server.receive_time(ackHeader,HEADER_SIZE,RECEIVE_TIMEOUT);

        if(n <= 0){
            cout << "Timeout!, resending response ... \n";
            sendReply(server,header,response,responseSize);
            continue;
        }

        int ackSize = utils::unmarshalInt(ackHeader);
        
        char* ack = new char[ackSize];

        cout << "WAITING FOR ACK\n";
        n = server.receive_time(ack,ackSize,RECEIVE_TIMEOUT);
        
        if(n <= 0){
            cout << "Timeout!, resending response ... \n";
            sendReply(server,header,response,responseSize);
            continue;
        }
        
        char *x = ack;
        int ack_id = utils::unmarshalInt(x);
        x += ID_SIZE;
        
        if(ack_id == responseID) break;
        else if(status == 2 && checkAndSendOldResponse(server,cAddress,ack_id)){
            cout << "Old request ID received instead!\n";
            cout << "Old response sent..!\n";
            cout << "Waiting for ack again..\n";
            continue;
        }
        else{
            cout << "ID mismatch!\nACK ID: " << ack_id << "\nResponse ID: " << responseID << "\n";
            sendReply(server,header,response,responseSize);
        }
    }
}

/**
   Checks whether message is a duplicate based on req_id
   and if so, sends the old response
 */
bool Handler::checkAndSendOldResponse(udp_server &server, unsigned long cAddress, int req_id){
    if(!memo.count({cAddress,req_id})) return false;
    char header[HEADER_SIZE];

    string str = memo[{cAddress,req_id}];
    
    char *response = new char[str.size()];
    for(int i=0;i<(int)str.size();i++)
        response[i] = str[i];
    
    int size = (int)str.size();
        
    utils::marshalInt(size,header);
    sendReply(server,header,response,size);
    cout << "Old response of size " << size << " is sent back\n";
    cout << "??????Status byte is: " << (int)*(response+ID_SIZE) << '\n';
    cout << "??????Status byte iz: " << (int)str[ID_SIZE] << '\n';
    //cout << "Account number sent is " << getS1AccNum(response) << "\n";
    return true;
}

int Handler::getS1AccNum(char *response){
    char *c = response;
    c += ID_SIZE + STATUS_SIZE + INT_SIZE;
    return utils::unmarshalInt(c);
}

/**
   Handles service 1 (create account):
   1. unmarshal request
   2. create account
   3. marshals and sends response
   4. handles ack
   5. notifies monitoring users
 */
void Handler::service1(udp_server &server, char *p, int req_id, int status){
    cout << "################################Service 1#####################################\n";
    unsigned long cAddress = server.getClientAddress().sin_addr.s_addr;

    if(status == 2 && checkAndSendOldResponse(server,cAddress,req_id))
        return;
    
    int currency;
    float balance;
    string passw, name;

    int length = utils::unmarshalInt(p);
    p += INT_SIZE;
    name = utils::unmarshalString(p,length);
    p += length;
    
    length = utils::unmarshalInt(p);
    p += INT_SIZE;
    passw = utils::unmarshalString(p,length);
    p += length;

    length = utils::unmarshalInt(p);
    p += INT_SIZE;
    currency = utils::unmarshalInt(p);
    p += length;

    length = utils::unmarshalInt(p);
    p += INT_SIZE;
    balance = utils::unmarshalFloat(p);
    p += length;

    int accountNum = acManager.createAccount(name,passw,currency,balance);
    cout << "Created account with account no. " << accountNum << '\n';
    
    char header[HEADER_SIZE];
    char response[ID_SIZE+STATUS_SIZE+INT_SIZE+FLOAT_SIZE];

    utils::marshalInt(ID_SIZE+STATUS_SIZE+INT_SIZE+FLOAT_SIZE,header);
    char *cur = response;

    int responseID = getResponseID();
    utils::marshalInt(responseID,cur);
    cur += ID_SIZE;

    utils::marshalString(ACK,cur);
    cur += STATUS_SIZE;

    utils::marshalInt(INT_SIZE,cur);
    cur += INT_SIZE;

    utils::marshalInt(accountNum,cur);

    cout << "!!!!!!!Saving response with status byte " << (int)*(response+ID_SIZE) << " into memo!!!!!!!!!\n";

    if(status == 2) memo[{cAddress,req_id}] = string(response,ID_SIZE+STATUS_SIZE+2*INT_SIZE);

    sendReply(server,header,response,ID_SIZE+STATUS_SIZE+2*INT_SIZE);
    
    if(status == 2) ackHandler(server, header, response, ID_SIZE+STATUS_SIZE+2*INT_SIZE, responseID, status, cAddress);
    
    notify(server,"Opened a new account with name " + name + ", currency " + to_string(currency) + ", balance " + to_string(balance) + ".", status);
}

/**
   Handles service 2 (delete account):
   1. unmarshal request
   2. delete account
   3. marshals and sends response
   4. handles ack
   5. notifies monitoring users
 */
void Handler::service2(udp_server &server, char *p, int req_id, int status){
    cout << "################################Service 2#####################################\n";
    unsigned long cAddress = server.getClientAddress().sin_addr.s_addr;

    if(status == 2 && checkAndSendOldResponse(server,cAddress,req_id))
        return;
    
    int accountNum;
    string passw, name;

    int length = utils::unmarshalInt(p);
    p += INT_SIZE;
    name = utils::unmarshalString(p,length);
    p += length;
    
    length = utils::unmarshalInt(p);
    p += INT_SIZE;
    accountNum = utils::unmarshalInt(p);
    p += length;

    length = utils::unmarshalInt(p);
    p += INT_SIZE;
    passw = utils::unmarshalString(p,length);
    p += length;

    bool success = acManager.deleteAccount(accountNum,name,passw);

    char header[HEADER_SIZE];
   
    
    if(success){
        char *response = new char[ID_SIZE+STATUS_SIZE];
        char *cur = response;

        cout << "Success deleting account\n";

        utils::marshalInt(ID_SIZE+STATUS_SIZE,header);

        int responseID = getResponseID();
        utils::marshalInt(responseID,cur);
        cur += ID_SIZE;

        utils::marshalString(ACK_SUCCESS,cur);

        if(status == 2) memo[{cAddress,req_id}] = string(response,ID_SIZE+STATUS_SIZE);

        sendReply(server,header,response,ID_SIZE+STATUS_SIZE);
        
        if(status == 2) ackHandler(server, header, response, ID_SIZE+STATUS_SIZE, responseID, status, cAddress);
        
        notify(server,"Deleted account no. " + to_string(accountNum) + " (" + name + ").", status);
    }
    else{
        
        cout << "Unsuccessful deletion\n";
        string err = "Wrong account number, name, or password!";

        char *response = new char[ID_SIZE+STATUS_SIZE+INT_SIZE+(int)err.size()];
        char *cur = response;

        utils::marshalInt(ID_SIZE+STATUS_SIZE+INT_SIZE+(int)err.size(),header);

        int responseID = getResponseID();
        utils::marshalInt(responseID,cur);
        cur += ID_SIZE;
        
        utils::marshalString(ACK_FAIL,cur);
        cur += STATUS_SIZE;

        utils::marshalInt((int)err.size(),cur);
        cur += INT_SIZE;

        utils::marshalString(err,cur);

        if(status == 2) memo[{cAddress,req_id}] = string(response,ID_SIZE+STATUS_SIZE+INT_SIZE+(int)err.size());

        sendReply(server,header,response,ID_SIZE+STATUS_SIZE+INT_SIZE+(int)err.size());
        
        if(status == 2) ackHandler(server, header, response, ID_SIZE+STATUS_SIZE+INT_SIZE+(int)err.size(), responseID, status, cAddress);
    }
}

/**
   Handles service 3 (deposit):
   1. unmarshal request
   2. deposit balance
   3. marshals and sends response
   4. handles ack
   5. notifies monitoring users
 */
void Handler::service3(udp_server &server, char *p, int req_id, int status){
    cout << "################################Service 3#####################################\n";
    unsigned long cAddress = server.getClientAddress().sin_addr.s_addr;

    if(status == 2 && checkAndSendOldResponse(server,cAddress,req_id))
        return;
    
    int accountNum, currency;
    float amount;
    string passw, name;
    
    int length = utils::unmarshalInt(p);
    p += INT_SIZE;
    name = utils::unmarshalString(p,length);
    p += length;

    length = utils::unmarshalInt(p);
    p += INT_SIZE;
    accountNum = utils::unmarshalInt(p);
    p += length;

    length = utils::unmarshalInt(p);
    p += INT_SIZE;
    passw = utils::unmarshalString(p,length);
    p += length;

    length = utils::unmarshalInt(p);
    p += INT_SIZE;
    currency = utils::unmarshalInt(p);
    p += length;
    
    length = utils::unmarshalInt(p);
    p += INT_SIZE;
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
        
        utils::marshalInt(ID_SIZE+STATUS_SIZE+INT_SIZE+(int)err.size(),header);

        char *response = new char[ID_SIZE+STATUS_SIZE+INT_SIZE+(int)err.size()];
        char *cur = response;

        int responseID = getResponseID();
        utils::marshalInt(responseID,cur);
        cur += ID_SIZE;

        utils::marshalString(ACK_FAIL,cur);
        cur += STATUS_SIZE;

        utils::marshalInt((int)err.size(),cur);
        cur += INT_SIZE;
            
        utils::marshalString(err,cur);
        cur += (int)err.size();

        if(status == 2) memo[{cAddress,req_id}] = string(response,ID_SIZE+STATUS_SIZE+INT_SIZE+(int)err.size());

        sendReply(server,header,response,ID_SIZE+STATUS_SIZE+INT_SIZE+(int)err.size());
        
        if(status == 2) ackHandler(server, header, response, ID_SIZE+STATUS_SIZE+INT_SIZE+(int)err.size(), responseID, status, cAddress);
    }
    else{
        char response[ID_SIZE+STATUS_SIZE+3*INT_SIZE+FLOAT_SIZE];
        char *cur = response;

        utils::marshalInt(ID_SIZE+STATUS_SIZE+3*INT_SIZE+FLOAT_SIZE,header);

        int responseID = getResponseID();
        utils::marshalInt(responseID,cur);
        cur += ID_SIZE;

        utils::marshalString(ACK_SUCCESS,cur);
        cur += STATUS_SIZE;

        utils::marshalInt(INT_SIZE,cur);
        cur += INT_SIZE;

        utils::marshalInt(balance.first,cur);
        cur += INT_SIZE;

        utils::marshalInt(FLOAT_SIZE,cur);
        cur += INT_SIZE;
        
        utils::marshalFloat(balance.second,cur);
        
        if(status == 2) memo[{cAddress,req_id}] = string(response,ID_SIZE+STATUS_SIZE+3*INT_SIZE+FLOAT_SIZE);

        sendReply(server,header,response,ID_SIZE+STATUS_SIZE+3*INT_SIZE+FLOAT_SIZE);
        
        if(status == 2) ackHandler(server, header, response, ID_SIZE+STATUS_SIZE+3*INT_SIZE+FLOAT_SIZE, responseID, status, cAddress);
        
        notify(server,"Deposited " + to_string(amount) + " of currency " + to_string(currency) + " to account no. " + to_string(accountNum) + " (" + name + ") .", status);
    }
}

/**
   Handles service 4 (withdraw):
   1. unmarshal request
   2. withdraws balance
   3. marshals and sends response
   4. handles ack
   5. notifies monitoring users
 */
void Handler::service4(udp_server &server, char *p, int req_id, int status){
    cout << "################################Service 4#####################################\n";
    unsigned long cAddress = server.getClientAddress().sin_addr.s_addr;

    if(status == 2 && checkAndSendOldResponse(server,cAddress,req_id))
        return;
    
    int accountNum, currency;
    float amount;
    string passw, name;
    
    int length = utils::unmarshalInt(p);
    p += INT_SIZE;
    name = utils::unmarshalString(p,length);
    p += length;

    length = utils::unmarshalInt(p);
    p += INT_SIZE;
    accountNum = utils::unmarshalInt(p);
    p += length;

    length = utils::unmarshalInt(p);
    p += INT_SIZE;
    passw = utils::unmarshalString(p,length);
    p += length;

    length = utils::unmarshalInt(p);
    p += INT_SIZE;
    currency = utils::unmarshalInt(p);
    p += length;
    
    length = utils::unmarshalInt(p);
    p += INT_SIZE;
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
        
        utils::marshalInt(ID_SIZE+STATUS_SIZE+INT_SIZE+(int)err.size(),header);

        char *response = new char[ID_SIZE+STATUS_SIZE+INT_SIZE+(int)err.size()];
        char *cur = response;

        int responseID = getResponseID();
        utils::marshalInt(responseID,cur);
        cur += ID_SIZE;
        
        utils::marshalString(ACK_FAIL,cur);
        cur += STATUS_SIZE;

        utils::marshalInt((int)err.size(),cur);
        cur += INT_SIZE;
            
        utils::marshalString(err,cur);
        cur += (int)err.size();

        if(status == 2) memo[{cAddress,req_id}] = string(response,ID_SIZE+STATUS_SIZE+INT_SIZE+(int)err.size());

        sendReply(server,header,response,ID_SIZE+STATUS_SIZE+INT_SIZE+(int)err.size());
        
        if(status == 2) ackHandler(server, header, response, ID_SIZE+STATUS_SIZE+INT_SIZE+(int)err.size(), responseID, status, cAddress);

    }
    else{
        char response[ID_SIZE+STATUS_SIZE+3*INT_SIZE+FLOAT_SIZE];
        char *cur = response;

        utils::marshalInt(ID_SIZE+STATUS_SIZE+3*INT_SIZE+FLOAT_SIZE,header);

        int responseID = getResponseID();
        utils::marshalInt(responseID,cur);
        cur += ID_SIZE;
        
        utils::marshalString(ACK_SUCCESS,cur);
        cur += STATUS_SIZE;

        utils::marshalInt(INT_SIZE,cur);
        cur += INT_SIZE;

        utils::marshalInt(balance.first,cur);
        cur += INT_SIZE;

        utils::marshalInt(FLOAT_SIZE,cur);
        cur += INT_SIZE;
        
        utils::marshalFloat(balance.second,cur);

        if(status == 2) memo[{cAddress,req_id}] = string(response,ID_SIZE+STATUS_SIZE+3*INT_SIZE+FLOAT_SIZE);

        sendReply(server,header,response,ID_SIZE+STATUS_SIZE+3*INT_SIZE+FLOAT_SIZE);
        
        if(status == 2) ackHandler(server, header, response, ID_SIZE+STATUS_SIZE+3*INT_SIZE+FLOAT_SIZE, responseID, status, cAddress);
        
        notify(server,"Withdrawn " + to_string(amount) + " of currency " + to_string(currency) + " from account no. " + to_string(accountNum) + " (" + name + ") .", status);
    }
}

/**
   Handles service 5 (add new monitoring user):
   1. unmarshal request
   2. adds new monitoring user
   3. marshals and sends response
   4. handles ack
 */
void Handler::service5(udp_server &server, char *p, int req_id, int status){
    cout << "################################Service 5#####################################\n";
    cout << "Beginning service 5\n";
    unsigned long cAddress = server.getClientAddress().sin_addr.s_addr;

    if(status == 2 && checkAndSendOldResponse(server,cAddress,req_id))
        return;
    

    int length = utils::unmarshalInt(p);
    p += INT_SIZE;
    int interval = utils::unmarshalInt(p);
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
    char response[ID_SIZE+STATUS_SIZE+2*INT_SIZE];
    char *cur = response;
    utils::marshalInt(ID_SIZE+STATUS_SIZE+2*INT_SIZE,header);

    int responseID = getResponseID();
    utils::marshalInt(responseID,cur);
    cur += INT_SIZE;

    utils::marshalString(ACK_SUCCESS,cur);
    cur += STATUS_SIZE;

    utils::marshalInt(INT_SIZE,cur);
    cur += INT_SIZE;
    
    utils::marshalInt(newAdmin.getRemaining(),cur);

    if(status == 2) memo[{cAddress,req_id}] = string(response,ID_SIZE+STATUS_SIZE+2*INT_SIZE);

    cout << "Sending response\n";

    sendReply(server,header,response,ID_SIZE+STATUS_SIZE+2*INT_SIZE);
    
    cout << "Handling ack\n";
    
    if(status == 2) ackHandler(server, header, response, ID_SIZE+STATUS_SIZE+2*INT_SIZE, responseID, status, cAddress);
}

/**
   Handles service 6 (transfer balance):
   1. unmarshal request
   2. transfer balance
   3. marshals and sends response
   4. handles ack
   5. notifies monitoring users
 */
void Handler::service6(udp_server &server, char *p, int req_id, int status){
    cout << "################################Service 6#####################################\n";
    unsigned long cAddress = server.getClientAddress().sin_addr.s_addr;

    if(status == 2 && checkAndSendOldResponse(server,cAddress,req_id))
        return;
    
    int accountNum1, accountNum2, currency;
    float amount;
    string passw, name1, name2;

    int length = utils::unmarshalInt(p);
    p += INT_SIZE;
    name1 = utils::unmarshalString(p,length);
    p += length;
    
    length = utils::unmarshalInt(p);
    p += INT_SIZE;
    accountNum1 = utils::unmarshalInt(p);
    p += length;

    length = utils::unmarshalInt(p);
    p += INT_SIZE;
    name2 = utils::unmarshalString(p,length);
    p += length;

    length = utils::unmarshalInt(p);
    p += INT_SIZE;
    accountNum2 = utils::unmarshalInt(p);
    p += length;

    length = utils::unmarshalInt(p);
    p += INT_SIZE;
    passw = utils::unmarshalString(p,length);
    p += length;
    
    length = utils::unmarshalInt(p);
    p += INT_SIZE;
    currency = utils::unmarshalInt(p);
    p += length;
    
    length = utils::unmarshalInt(p);
    p += INT_SIZE;
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
        else if(balance.first == -15) err = "Unable to transfer amount higher than balance!";
        else if(balance.first == -21) err = "Account number not found!";
        else if(balance.first == -22) err = "Account number of recipient not found!";
        else if(balance.first == -23) err = "Wrong name!";
        else if(balance.first == -24) err = "Wrong recipient name!";
        else if(balance.first == -25) err = "Wrong password!";
        else if(balance.first == -26) err = "Currency mismatch!";
        else if(balance.first == -27) err = "Recipient currency mismatch!";
        else err = "Unknown error!";
        
        utils::marshalInt(ID_SIZE+STATUS_SIZE+INT_SIZE+(int)err.size(),header);

        char *response = new char[ID_SIZE+STATUS_SIZE+INT_SIZE+(int)err.size()];
        char *cur = response;

        int responseID = getResponseID();
        utils::marshalInt(responseID,cur);
        cur += INT_SIZE;
        
        utils::marshalString(ACK_FAIL,cur);
        cur += STATUS_SIZE;

        utils::marshalInt((int)err.size(),cur);
        cur += INT_SIZE;
            
        utils::marshalString(err,cur);
        cur += (int)err.size();

        if(status == 2) memo[{cAddress,req_id}] = string(response,ID_SIZE+STATUS_SIZE+INT_SIZE+(int)err.size());

        sendReply(server,header,response,ID_SIZE+STATUS_SIZE+INT_SIZE+(int)err.size());
        
        if(status == 2) ackHandler(server, header, response, ID_SIZE+STATUS_SIZE+INT_SIZE+(int)err.size(), responseID, status, cAddress);
    }
    else{
        char response[ID_SIZE+STATUS_SIZE+3*INT_SIZE+FLOAT_SIZE];
        char *cur = response;

        int responseID = getResponseID();
        utils::marshalInt(responseID,cur);
        cur += ID_SIZE;

        utils::marshalString(ACK_SUCCESS,cur);
        cur += STATUS_SIZE;

        utils::marshalInt(INT_SIZE,cur);
        cur += INT_SIZE;

        utils::marshalInt(balance.first,cur);
        cur += INT_SIZE;

        utils::marshalInt(INT_SIZE,cur);
        cur += INT_SIZE;
        
        utils::marshalFloat(balance.second,cur);

        utils::marshalInt(ID_SIZE+STATUS_SIZE+3*INT_SIZE+FLOAT_SIZE,header);

        if(status == 2) memo[{cAddress,req_id}] = string(response,ID_SIZE+STATUS_SIZE+3*INT_SIZE+FLOAT_SIZE);

        sendReply(server,header,response,ID_SIZE+STATUS_SIZE+3*INT_SIZE+FLOAT_SIZE);
        
        if(status == 2) ackHandler(server, header, response, ID_SIZE+STATUS_SIZE+3*INT_SIZE+FLOAT_SIZE, responseID, status, cAddress);
        notify(server,"Transferred " + to_string(amount) + " of currency " + to_string(currency) + " from account no. " + to_string(accountNum1) + " (" + name1 + ") to account no. " + to_string(accountNum2) + " (" + name2 + ") .", status);
    }
}

/**
   Handles service 7 (forget password):
   1. unmarshal request
   2. change password
   3. marshals and sends response
   4. handles ack
   5. notifies monitoring users
 */
void Handler::service7(udp_server &server, char *p, int req_id, int status){
    cout << "################################Service 7#####################################\n";
    unsigned long cAddress = server.getClientAddress().sin_addr.s_addr;

    if(status == 2 && checkAndSendOldResponse(server,cAddress,req_id))
        return;
    
    int accountNum;
    string passw, newPassw, name;

    int length = utils::unmarshalInt(p);
    p += INT_SIZE;
    name = utils::unmarshalString(p,length);
    p += length;
    
    length = utils::unmarshalInt(p);
    p += INT_SIZE;
    accountNum = utils::unmarshalInt(p);
    p += length;

    length = utils::unmarshalInt(p);
    p += INT_SIZE;
    passw = utils::unmarshalString(p,length);
    p += length;

    length = utils::unmarshalInt(p);
    p += INT_SIZE;
    newPassw = utils::unmarshalString(p,length);
    p += length;

    bool success = acManager.changePassword(accountNum,name,passw,newPassw);

    char header[HEADER_SIZE];
    char response[ID_SIZE+STATUS_SIZE];

    utils::marshalInt(ID_SIZE+STATUS_SIZE,header);
    char *cur = response;

    int responseID = getResponseID();
    utils::marshalInt(responseID,cur);
    cur += ID_SIZE;
    
    if(success) utils::marshalString(ACK_SUCCESS,cur);
    else utils::marshalString(ACK_FAIL,cur);

    if(status == 2) memo[{cAddress,req_id}] = string(response,ID_SIZE+STATUS_SIZE);

    sendReply(server,header,response,ID_SIZE+STATUS_SIZE);

    if(status == 2) ackHandler(server, header, response, ID_SIZE+STATUS_SIZE, responseID, status, cAddress);
    
    notify(server,"Changed password of account no. " + to_string(accountNum) + " (" + name + ").", status);
}
