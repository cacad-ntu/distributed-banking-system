
#ifndef ACCOUNT
#define ACCOUNT
#include <string>
using namespace std;
class Account{
private:
	int accountNumber, currency;
	float balance;
	string name, passw;
public:
	int getAccountNumber();
	string getCurrency();
	float getBalance();
	string getName();
	string getPassword();
	Account();
	Account(int accountNumber_, string name_, string passw_, int currency_, float balance_);
};
#endif
