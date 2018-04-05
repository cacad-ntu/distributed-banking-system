#include "Account.h"

Account::Account(){}

Account::Account(int accountNumber_, string name_, string passw_, int currency_, float balance_){
	accountNumber = accountNumber_;
	name = name_;
	passw = passw_;
	currency = currency_;
	balance = balance_;
}

/**
   Returns account number
 */
int Account::getAccountNumber(){
	return accountNumber;
}

/**
   Returns currency of account balance
 */
int Account::getCurrency(){
    return currency;
}

/**
   Returns account balance
 */
float Account::getBalance(){
	return balance;
}

/**
   Returns account name
 */
string Account::getName(){
	return name;
}

/**
   Returns account password
 */
string Account::getPassword(){
	return passw;
}

/**
   Adds balance with given amount
   Returns pair of currency and balance
 */
pair<int,float> Account::addAmount(float amount){
    balance += amount;
    return pair<int,float>(currency,balance);
}

/**
   Subtracts balance with given amount
   Returns pair of currency and balance
 */
pair<int,float> Account::subtractAmount(float amount){
    balance -= amount;
    return pair<int,float>(currency,balance);
}

/**
   Changes account password
 */
void Account::changePassword(string passw_){
    passw = passw_;
}
