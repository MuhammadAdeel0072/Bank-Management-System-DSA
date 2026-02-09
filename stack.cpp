#include "Stack.h"
#include <iostream>
using namespace std;

// ---------------- StackNode -----------------
StackNode::StackNode(Account* acc, Transaction* tr) {
    account = acc;
    transaction = tr;
    next = nullptr;
}

Transaction* StackNode::getTransaction() const { return transaction; }
Account* StackNode::getAccount() const { return account; }

// ---------------- Stack -----------------
Stack::Stack() { topNode = nullptr; }

Stack::~Stack() {
    while (!isEmpty()) {
        StackNode* temp = pop();
        delete temp;
    }
}

void Stack::push(Account* account, Transaction* transaction) {
    StackNode* node = new StackNode(account, transaction);
    node->next = topNode;
    topNode = node;
}

StackNode* Stack::pop() {
    if (isEmpty()) return nullptr;
    StackNode* temp = topNode;
    topNode = topNode->next;
    return temp;
}

bool Stack::isEmpty() const {
    return topNode == nullptr;
}
