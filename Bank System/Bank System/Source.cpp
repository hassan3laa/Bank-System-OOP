#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>


using namespace std;
using ll = long long;


class Bank {
public:
    static int totalAcc;

};
int Bank::totalAcc = 0;

class Account {
protected:
    int accnum;
    string ownerName;
    double balance;

public:
    Account(int an, string ow, double b) : accnum(an), ownerName(ow), balance(b) {
        Bank::totalAcc++;
    }
    virtual void deposit(double am) {
        if (am > 0) {
            balance += am;
        }
        else {
            cout << "Invalid Amount";
        }
    }
    virtual void withdraw(double am) = 0;
    virtual void display_info() const = 0;

    int get_accnum() {
        return accnum;
    }
    double get_balance() {
        return balance;
    }
    friend void monthly_fee(Account& test);


};
class current_acc : public Account {
protected:
    int overlimit; 

public:
    void withdraw(double am) {
        if (overlimit <= am && am < balance) {
            balance -= am;
        }
        else {
            cout << "Invalid ammount\n";
        }
    }

    void display_info() {
        cout << "Account number : " << accnum << '\n'
        << "Owner Name : " << ownerName << '\n'
        << "balance : " << balance << '\n'
        << "Your account tyoe is : cuurent account" << '\n';
    }



};
class saving_acc : public Account {
private:
    double benfites = 0.2 * balance;

public:

    void added_ben() {
        balance += benfites;
    }

    void withdraw(double am) {
        if (am > balance) {
            cout << "Invalid ammount\n";
        }
        else {
            balance += am;
        }
    }

    void display_info() {
        cout << "Account number : " << accnum << '\n'
        << "Owner name : " << ownerName << '\n'
        << "balance : " << balance << '\n'
        << "Your  account type is : Saving account" << '\n';
    }

};
void monthly_fee(Account& test) {
    test.balance -= 20;
}


int main() {