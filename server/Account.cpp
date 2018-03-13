#include "Account.h"

Account::Account(){}

Account::Account(int accountNumber_, string name_, string passw_, int currency_, float balance_){
	accountNumber = accountNumber_;
	name = name_;
	passw = passw_;
	currency = currency_;
	balance = balance_;
}

int Account::getAccountNumber(){
	return accountNumber;
}

string Account::getCurrency(){
	switch(currency){
	case 0: return "USD";
	default: return "SGD";
    }
	return "SGD";
}

float Account::getBalance(){
	return balance;
}

string Account::getName(){
	return name;
}

string Account::getPassword(){
	return passw;
}
