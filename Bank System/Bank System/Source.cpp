#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <conio.h>

using namespace std;
using ll = long long;


class Bank {
public:
    static int totalAcc;
    static void loadTotalAcc() {
        ifstream in("accounts.txt");
        string line;
        int lastAcc = 1000;
        while (getline(in, line)) {
            stringstream ss(line);
            string tmp;
            for (int i = 0; i < 3; ++i) getline(ss, tmp, ',');
            int accnum;
            ss >> accnum;
            lastAcc = max(lastAcc, accnum);
        }
        totalAcc = lastAcc;
    }

};
int Bank::totalAcc = 1000;

class Account {
protected:
    int accnum;
    string ownerName;
    double balance;
    string password;
    string accounttype;
    string username;


public:
    Account(int an, string ow, double b, string pass, string acctype, string user) : accnum(an), ownerName(ow), balance(b), password(pass), accounttype(acctype), username(user) {}

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
    string getOwnreName() {
        return ownerName;
    }
    string getPassword() const { return password; }
    void setPassword(string pass) { password = pass; }

    void transfer(Account& to, int ammount) {
        if (balance >= ammount) {
            this->withdraw(ammount);
            to.deposit(ammount);

            cout << "Transfer compelete\n";
        }
        else {
            cout << "Invalid ammount\n";
        }
    }

    string get_username() const { return username; }

    string getType() const { return accounttype; }

};
class current_acc : public Account {
protected:
    int overlimit; 

public:
    current_acc(int an, string ow, double b, string pass, string user) : Account(an, ow, b, pass, "current", user), overlimit(500) {}

    void withdraw(double am) override {
        if (balance - am >= -overlimit) {
            balance -= am;
        }
        else {
            cout << "Invalid ammount\n";
        }
    }

    void display_info() const override{
        cout << "Account number : " << accnum << '\n'
        << "Owner Name : " << ownerName << '\n'
        << "balance : " << balance << '\n'
        << "Your account type is : cuurent account" << '\n';
    }



};
class saving_acc : public Account {
public:
    saving_acc(int an, string ow, double b, string pass, string user) : Account(an, ow, b, pass, "saving", user) {}

    void added_ben() {
        double benfites = 0.2 * 100;
        balance += benfites;
    }

    void withdraw(double am) override {
        if (am > balance) {
            cout << "Invalid ammount\n";
        }
        else {
            balance -= am;
        }
    }

    void display_info() const override {
        cout << "Account number : " << accnum << '\n'
        << "Owner name : " << ownerName << '\n'
        << "balance : " << balance << '\n'
        << "Your  account type is : Saving account" << '\n';
    }

};



string getPasswordInput() {
    string password;
    char ch;
    while ((ch = _getch()) != '\r') { // if the user click Enter
        if (ch == '\b') { // Backspace
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b";
            }
        }
        else {
            password += ch;
            cout << '*';
        }
    }
    cout << '\n';
    return password;
}


Account* findAccnum(vector<Account*>& allaccounts, int accnum) {
    for (auto acc : allaccounts) {
        if (acc->get_accnum() == accnum) {
            return acc;
        }
    }
    return nullptr;
}

void saveAccountsToFile(vector<Account*>& allaccounts) {
    ofstream out("accounts.txt");
    for (auto acc : allaccounts) {
        string username = acc->get_username();
        string password = acc->getPassword();
        string type = acc->getType();

        out << username << "," << acc->getPassword() << "," << type << ","
            << acc->get_accnum() << "," << acc->get_balance() << ","
            << acc->getOwnreName() << "\n";
    }
}

void loadAccountsFromFile(vector<Account*>& allAccounts) {
    ifstream in("accounts.txt");
    string line;
    while (getline(in, line)) {
        stringstream ss(line);
        string user, pass, type, owner;
        int accnum; double balance;
        getline(ss, user, ',');
        getline(ss, pass, ',');
        getline(ss, type, ',');
        ss >> accnum; ss.ignore();
        ss >> balance; ss.ignore();
        getline(ss, owner);
        Account* acc = nullptr;
            if (type == "saving") {
                acc = new saving_acc(accnum, owner, balance, pass, user);
            }
            else {
                acc = new current_acc(accnum, owner, balance, pass, user);
            }
        allAccounts.push_back(acc);
    }
}


class Registrationsystem {
public:
    Account* login(vector<Account*>& allaccounts);
    Account* signup(vector<Account*>& allaccounts);
};

Account* Registrationsystem::login(vector<Account*>& allaccounts) {
    string username, pass;

    cin.ignore();

    cout << "Enter Username : "; getline(cin, username);
    cout << "Enter Password : "; pass = getPasswordInput();

    for (auto acc : allaccounts) {
        if (acc->get_username() == username) {
            if (acc->getPassword() == pass) {
                return acc;
            }
        }
    }

    cout << "Invalid Login\n";
    return nullptr;
}

Account* Registrationsystem::signup (vector<Account*>& allaccounts) {
    
    string user, pass, type, ownername;
    double balance;

    cout << "---------- Creat Account ----------\n";
    cin.ignore();
    cout << "Username : "; getline(cin,user);
    cout << "Password : "; pass = getPasswordInput();
    cout << "Owner name : "; getline(cin, ownername);
    cout << "Initial Balacne : "; cin >> balance;

    do {
        cout << "Account Type (saving / current) : "; cin >> type;
    } while (type != "saving" && type != "current");

    int accnum = Bank::totalAcc++;
    
    Account* acc = nullptr;
    if (type == "saving") {
        acc = new saving_acc(accnum, ownername, balance, pass, user);
    }
    else {
        acc = new current_acc(accnum, ownername, balance, pass, user);
    }

    allaccounts.push_back(acc);
    saveAccountsToFile(allaccounts);
    cout << "? Account Created. Number: " << accnum << "\n";
    return acc;
}






int main() {
    Registrationsystem reg;
    vector<Account*> allaccounts;
    Account* currAcc = nullptr;

    int ch;

login_menu:
    allaccounts.clear();
    loadAccountsFromFile(allaccounts);

    if (!allaccounts.empty()) {
        Bank::totalAcc = allaccounts.back()->get_accnum() + 1;
    }
    else {
        Bank::totalAcc = 1000;
    }



    while (true) {

        cout << "====================== Welcome To Our Bank ======================\n";
        cout << "1 - login\n2 - Sign Up\n3 - Exit\n";
        cout << "\nChoice : ";
        cin >> ch;

        if (ch == 1) {
            currAcc = reg.login(allaccounts);
            if (currAcc) { break; }
        }
        else if (ch == 2) {
            currAcc = reg.signup(allaccounts);
            if (currAcc) { break; }
        }
        else if (ch == 3) {

            cout << "====================== Thank you for your business ======================\n";
            return 0;
        }
        else cout << "Invalid Input\n";

    }


    while (true) {
        cout << "====================== Welcome To Your Account Bank ======================\n";
        cout << "1. Display Info\n2. Deposit\n3. Withdraw\n4. Transfer\n5. Logout\nChoice: ";
        
        cin >> ch;

        if (ch == 1) {
            currAcc->display_info();
        }
        else if (ch == 2) {
            double am;
            cout << "Enter Ammount : "; cin >> am;
            currAcc->deposit(am);
            saveAccountsToFile(allaccounts);
        }
        else if (ch == 3) {
            double am; 
            cout << "Enter Ammount : "; cin >> am;
            currAcc->withdraw(am);
            saveAccountsToFile(allaccounts);
        }
        else if (ch == 4) {
            int to; double am;
            cout << "To Ammount Number : "; cin >> to;
            cout << "Enter Ammount : "; cin >> am;
            Account* target = findAccnum(allaccounts, to);

            if (!target) { cout << "Invalid Account Number \n"; }
            else {
                currAcc->transfer(*target, am);

                saveAccountsToFile(allaccounts);
            }
        }
        else if (ch == 5) {
            currAcc = nullptr;
            goto login_menu;
        }

        else {
            cout << '\n' << "Invalid \n";
        }
    }



}