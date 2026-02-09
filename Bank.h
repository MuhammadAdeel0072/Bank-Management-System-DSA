#pragma once

#include "AVLTree.h"
#include "Stack.h"
#include "Queue.h"
#include <string>

class Bank {
private:
    AVLTree accounts;
    Stack undoStack;
    Queue transferQueue;
    long nextAccountNumber;

    // Helper functions
    bool validateEmail(const std::string& email) const;
    bool validateName(const std::string& name) const;
    int getIntInput(const std::string& prompt) const;
    double getDoubleInput(const std::string& prompt) const;
    std::string getStringInput(const std::string& prompt) const;
    void clearScreen() const;

    // login helpers
    void adminLogin();
    void customerLogin();

public:
    Bank();

    // System start
    void start(); // persistent login system

    // Login
    void login(); // original (kept for compatibility, called by admin/customer helpers if desired)

    // Admin menu
    void adminMenu();

    // Customer menu (logged-in account)
    void customerMenu(Account* acc);

    // Admin operations
    void createAccount();
    void deleteAccount();
    void searchAccount();
    void displayAllAccounts();

    // Customer operations
    void depositMoney(Account* acc);
    void withdrawMoney(Account* acc);
    void searchAccount(Account* acc);
    void viewTransactions(Account* acc);
    void transferFunds(Account* acc);
    void undoLastTransaction(Account* acc);
};
