#pragma once

#include <iostream>
#include <string>
#include <ctime>
using namespace std;

class Transaction {
private:
    string type;
    double amount;
    time_t timestamp;
    Transaction* next;

public:
    Transaction(const string& t, double amt);

    string getType() const;
    double getAmount() const;
    time_t getTimestamp() const;
    Transaction* getNext() const;
    void setNext(Transaction* n);
};

class Account {
private:
    long accountNumber;
    string name;
    string email;
    double balance;
    string pin;            // PIN
    Transaction* transHead;

public:
    Account(long accNo = 0, const string& nm = "", const string& mail = "", double bal = 0, const string& p = "");
    ~Account();

    // Getters
    long getAccountNumber() const;
    string getName() const;
    string getEmail() const;
    double getBalance() const;
    Transaction* getTransactions() const;
    string getPIN() const;

    // Setters
    void setBalance(double b);
    void setTransactions(Transaction* t);
    void setPIN(const string& p);

    // Account operations
    void deposit(double amt);
    bool withdraw(double amt);
    void addTransaction(const string& t, double amt);
    void showTransactions() const;
    void showAccount() const;
};
