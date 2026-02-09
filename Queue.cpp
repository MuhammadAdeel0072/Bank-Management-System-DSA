#include "Queue.h"
using namespace std;

// ---------------- QueueNode -----------------
QueueNode::QueueNode(Account* from, Account* to, double amt) {
    fromAccount = from;
    toAccount = to;
    amount = amt;
    next = nullptr;
}

Account* QueueNode::getFromAccount() const { return fromAccount; }
Account* QueueNode::getToAccount() const { return toAccount; }
double QueueNode::getAmount() const { return amount; }

// ---------------- Queue -----------------
Queue::Queue() {
    front = rear = nullptr;
}

Queue::~Queue() {
    while (!isEmpty()) {
        QueueNode* temp = dequeue();
        delete temp;
    }
}

void Queue::enqueue(Account* from, Account* to, double amount) {
    QueueNode* node = new QueueNode(from, to, amount);
    if (!rear) {
        front = rear = node;
    } else {
        rear->next = node;
        rear = node;
    }
}

QueueNode* Queue::dequeue() {
    if (isEmpty()) return nullptr;
    QueueNode* temp = front;
    front = front->next;
    if (!front) rear = nullptr;
    return temp;
}

bool Queue::isEmpty() const {
    return front == nullptr;
}
