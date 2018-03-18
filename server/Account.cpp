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

int Account::getCurrency(){
    return currency;
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

pair<int,float> Account::addAmount(float amount){
    balance += amount;
    return pair<int,float>(currency,balance);
}

pair<int,float> Account::subtractAmount(float amount){
    balance -= amount;
    return pair<int,float>(currency,balance);
}

void Account::changePassword(string passw_){
    passw = passw_;
}
