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

bool AccountManager::deposit(int accountNumber, string name, string passw, int currency, float amount){
	if(!accounts.count(accountNumber))
        return false;
    auto ptr = accounts[accountNumber];

    if(name.compare(ptr->getName()) != 0)
        return false;

    if(passw.compare(ptr->getPassword()) != 0)
        return false;

    if(currency != ptr->getCurrency())
        return false;

    ptr->addAmount(amount);
    return true;
}

bool AccountManager::withdraw(int accountNumber, string name, string passw, int currency, float amount){
	if(!accounts.count(accountNumber))
        return false;
    auto ptr = accounts[accountNumber];

    if(name.compare(ptr->getName()) != 0)
        return false;

    if(passw.compare(ptr->getPassword()) != 0)
        return false;

    if(currency != ptr->getCurrency())
        return false;

    if(amount > ptr->getBalance())
        return false;

    ptr->subtractAmount(amount);
    return true;
}

bool AccountManager::transfer(int accountNum1, int accountNum2, string name1, string name2, string passw, int currency, float amount){
    if(!accounts.count(accountNum1))
        return false;

    if(!accounts.count(accountNum2))
        return false;

    auto ptr1 = accounts[accountNum1];
    auto ptr2 = accounts[accountNum2];

    if(name1.compare(ptr1->getName()) != 0)
        return false;

    if(name2.compare(ptr2->getName()) != 0)
        return false;

    if(passw.compare(ptr1->getPassword()) != 0)
        return false;

    if(currency != ptr1->getCurrency())
        return false;

    if(currency != ptr2->getCurrency())
        return false;

    bool withdrawSuccess = withdraw(accountNum1,name1,passw,currency,amount);

    if(!withdrawSuccess) return false;

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
