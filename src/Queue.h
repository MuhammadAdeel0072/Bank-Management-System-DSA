#ifndef QUEUE_H
#define QUEUE_H

#include "Account.h"

class QueueNode {
private:
    Account* fromAccount;
    Account* toAccount;
    double amount;

public:
    QueueNode* next;

    QueueNode(Account* from, Account* to, double amt);
    Account* getFromAccount() const;
    Account* getToAccount() const;
    double getAmount() const;
};

class Queue {
private:
    QueueNode* front;
    QueueNode* rear;

public:
    Queue();
    ~Queue();

    void enqueue(Account* from, Account* to, double amount);
    QueueNode* dequeue();
    bool isEmpty() const;
};

#endif
