#include "AccountManager.h"

int AccountManager::createAccount(string name, string passw, int currency, float balance){
    int accountNumber = newID++;
	SmartPtr<Account> ptr(new Account(accountNumber,name,passw,currency,balance));
	accounts[accountNumber] = ptr;
	return accountNumber;
}

bool AccountManager::deleteAccount(int accountNumber, string name, string passw){
	if(!accounts.count(accountNumber))
		return false;
	auto ptr = accounts[accountNumber];
	if(name.compare(ptr->getName()) != 0)
		return false;
    if(passw.compare(ptr->getPassword()) != 0)
		return false;
    accounts.erase(accountNumber);
	return true;
}

void AccountManager::deposit(int accountNumber, string name, string passw, int currency, float amount){
	
}

void AccountManager::withdraw(int accountNumber, string name, string passw, int currency, float amount){
	
}
