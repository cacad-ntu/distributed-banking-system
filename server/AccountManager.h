#ifndef ACCOUNTMANAGER
#define ACCOUNTMANAGER

#include<unordered_map>
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
	void deposit(int accountNumber, string name, string passw, int currency, float amount);
	void withdraw(int accountNumber, string name, string passw, int currency, float amount);
};

#endif
