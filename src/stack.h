#ifndef STACK_H
#define STACK_H

#include "Account.h"

class StackNode {
private:
    Transaction* transaction;
    Account* account;

public:
    StackNode* next;

    StackNode(Account* account, Transaction* transaction);
    Transaction* getTransaction() const;
    Account* getAccount() const;
};

class Stack {
private:
    StackNode* topNode;

public:
    Stack();
    ~Stack();

    void push(Account* account, Transaction* transaction);
    StackNode* pop();
    bool isEmpty() const;
};

#endif
