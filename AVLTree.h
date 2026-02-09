#ifndef AVLTREE_H
#define AVLTREE_H

#include "Account.h"

class AVLNode {
private:
    Account* account;

public:
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(Account* account);
    Account* getAccount() const;
    void setAccount(Account* acc);   // setter to allow safe account pointer changes
};

class AVLTree {
private:
    AVLNode* root;

    int height(AVLNode* node);
    int balanceFactor(AVLNode* node);
    AVLNode* rotateRight(AVLNode* y);
    AVLNode* rotateLeft(AVLNode* x);
    AVLNode* insertNode(AVLNode* node, Account* account);
    AVLNode* searchNode(AVLNode* node, long accNo);
    AVLNode* findMin(AVLNode* node);
    AVLNode* deleteNode(AVLNode* node, long accNo);
    void inorderPrint(AVLNode* node);
    void destroyAll(AVLNode* node);

public:
    AVLTree();
    ~AVLTree();

    void insert(Account* account);
    bool remove(long accNo);      // deletes account with accNo, returns true if removed
    Account* search(long accNo);
    void displayAll();
};

#endif
