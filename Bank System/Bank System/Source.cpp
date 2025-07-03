#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>


using namespace std;
using ll = long long;


class Bank {
public :
	static int totalAcc;

};
int Bank::totalAcc = 0;

class Account {
private:
	int accnum;
	string ownerName;
	int balance;

public:
	Account(int an, string ow, int b) : accnum(an), ownerName(ow), balance(b) {
		Bank::totalAcc++;
	}


};





int main() {

}