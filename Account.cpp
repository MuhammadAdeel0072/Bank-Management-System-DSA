#include "Account.h"

// ---------------- Transaction ----------------
Transaction::Transaction(const string& t, double amt) {
    type = t;
    amount = amt;
    timestamp = time(NULL);
    next = nullptr;
}

string Transaction::getType() const { return type; }
double Transaction::getAmount() const { return amount; }
time_t Transaction::getTimestamp() const { return timestamp; }
Transaction* Transaction::getNext() const { return next; }
void Transaction::setNext(Transaction* n) { next = n; }

// ---------------- Account ----------------
Account::Account(long accNo, const string& nm, const string& mail, double bal, const string& p) {
    accountNumber = accNo;
    name = nm;
    email = mail;
    balance = bal;
    pin = p;
    transHead = nullptr;
}

Account::~Account() {
    Transaction* temp;
    while (transHead != nullptr) {
        temp = transHead;
        transHead = transHead->getNext();
        delete temp;
    }
}

// ---------------- Getters ----------------
long Account::getAccountNumber() const { return accountNumber; }
string Account::getName() const { return name; }
string Account::getEmail() const { return email; }
double Account::getBalance() const { return balance; }
Transaction* Account::getTransactions() const { return transHead; }
string Account::getPIN() const { return pin; }

// ---------------- Setters ----------------
void Account::setBalance(double b) { balance = b; }
void Account::setTransactions(Transaction* t) { transHead = t; }
void Account::setPIN(const string& p) { pin = p; }

// ---------------- Account operations ----------------
void Account::deposit(double amt) {
    if (amt <= 0) {
        cout << "Invalid deposit amount.\n";
        return;
    }
    balance += amt;
    addTransaction("Deposit", amt);
}

bool Account::withdraw(double amt) {
    if (amt <= 0) {
        cout << "Invalid withdraw amount.\n";
        return false;
    }
    if (amt > balance) {
        cout << "Insufficient balance.\n";
        return false;
    }
    balance -= amt;
    addTransaction("Withdraw", amt);
    return true;
}

void Account::addTransaction(const string& t, double amt) {
    Transaction* tr = new Transaction(t, amt);
    tr->setNext(transHead);
    transHead = tr;
}

void Account::showTransactions() const {
    Transaction* temp = transHead;
    if (!temp) {
        cout << "No transactions.\n";
        return;
    }
    while (temp != nullptr) {
        time_t ts = temp->getTimestamp();
        cout << temp->getType() << " : " << temp->getAmount()
             << " | " << ctime(&ts);
        temp = temp->getNext();
    }
}

void Account::showAccount() const {
    cout << "Account#: " << accountNumber << "  |  Name: " << name << "  |  Email: " << email << "  |  Balance: " << balance << "\n";
}
