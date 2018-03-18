#ifndef ACCOUNTMANAGER
#define ACCOUNTMANAGER

#include<unordered_map>
#include<utility>
#include "Account.h"
#include "SmartPtr.h"
using namespace std;

class AccountManager{
private:
	int newID;
	unordered_map<int,SmartPtr<Account>> accounts;
public:
	int createAccount(string name, string passw, int currency, float balance);
	bool deleteAccount(int accountNumber, string name, string passw);
	pair<int,float> deposit(int accountNumber, string name, string passw, int currency, float amount);
    pair<int,float> withdraw(int accountNumber, string name, string passw, int currency, float amount);
    pair<int,float> transfer(int accountNum1, int accountNum2, string name1, string name2, string passw, int currency, float amount);

    bool changePassword(int accountNum, string name, string passw, string newPassw);
    AccountManager();
};

#endif
