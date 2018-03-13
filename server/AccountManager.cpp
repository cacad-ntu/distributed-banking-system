#include "AccountManager.h"

AccountManager::AccountManager(){
    newID = 0;
    accounts.clear();
}

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

float AccountManager::deposit(int accountNumber, string name, string passw, int currency, float amount){
	if(!accounts.count(accountNumber))
        return -1;
    auto ptr = accounts[accountNumber];

    if(name.compare(ptr->getName()) != 0)
        return -2;

    if(passw.compare(ptr->getPassword()) != 0)
        return -3;

    if(currency != ptr->getCurrency())
        return -4;

    return ptr->addAmount(amount);
}

float AccountManager::withdraw(int accountNumber, string name, string passw, int currency, float amount){
	if(!accounts.count(accountNumber))
        return -11;
    auto ptr = accounts[accountNumber];

    if(name.compare(ptr->getName()) != 0)
        return -12;

    if(passw.compare(ptr->getPassword()) != 0)
        return -13;

    if(currency != ptr->getCurrency())
        return -14;

    if(amount > ptr->getBalance())
        return -15;

    return ptr->subtractAmount(amount);
}

float AccountManager::transfer(int accountNum1, int accountNum2, string name1, string name2, string passw, int currency, float amount){
    if(!accounts.count(accountNum1))
        return -21;

    if(!accounts.count(accountNum2))
        return -22;

    auto ptr1 = accounts[accountNum1];
    auto ptr2 = accounts[accountNum2];

    if(name1.compare(ptr1->getName()) != 0)
        return -23;

    if(name2.compare(ptr2->getName()) != 0)
        return -24;

    if(passw.compare(ptr1->getPassword()) != 0)
        return -25;

    if(currency != ptr1->getCurrency())
        return -26;

    if(currency != ptr2->getCurrency())
        return -27;

    float balance = withdraw(accountNum1,name1,passw,currency,amount);

    if(balance <= -1e-9) return balance;
    return deposit(accountNum2,name2,passw,currency,amount);
}

bool AccountManager::changePassword(int accountNum, string name, string passw, string newPassw){
    if(!accounts.count(accountNum))
        return false;

    auto ptr = accounts[accountNum];

    if(name.compare(ptr->getName()) != 0)
        return false;

    if(passw.compare(ptr->getPassword()) != 0)
        return false;

    ptr->changePassword(newPassw);
    return true;
}
