#include "AVLTree.h"
#include <iostream>
using namespace std;

// ---------------- AVLNode -----------------
AVLNode::AVLNode(Account* a) {
    account = a;
    left = right = nullptr;
    height = 1;
}

Account* AVLNode::getAccount() const { return account; }
void AVLNode::setAccount(Account* acc) { account = acc; }

// ---------------- AVLTree -----------------
AVLTree::AVLTree() { root = nullptr; }

AVLTree::~AVLTree() { destroyAll(root); }

int AVLTree::height(AVLNode* node) { return node ? node->height : 0; }

int AVLTree::balanceFactor(AVLNode* node) {
    return node ? height(node->left) - height(node->right) : 0;
}

AVLNode* AVLTree::rotateRight(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = 1 + max(height(y->left), height(y->right));
    x->height = 1 + max(height(x->left), height(x->right));

    return x;
}

AVLNode* AVLTree::rotateLeft(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = 1 + max(height(x->left), height(x->right));
    y->height = 1 + max(height(y->left), height(y->right));

    return y;
}

// ---------------- INSERTION -----------------
AVLNode* AVLTree::insertNode(AVLNode* node, Account* a) {
    if (!node) return new AVLNode(a);

    if (a->getAccountNumber() < node->getAccount()->getAccountNumber())
        node->left = insertNode(node->left, a);
    else if (a->getAccountNumber() > node->getAccount()->getAccountNumber())
        node->right = insertNode(node->right, a);
    else
        return node;

    node->height = 1 + max(height(node->left), height(node->right));
    int bf = balanceFactor(node);

    if (bf > 1) {
        if (a->getAccountNumber() < node->left->getAccount()->getAccountNumber())
            return rotateRight(node);
        else {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
    }

    if (bf < -1) {
        if (a->getAccountNumber() > node->right->getAccount()->getAccountNumber())
            return rotateLeft(node);
        else {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
    }

    return node;
}

void AVLTree::insert(Account* a) { root = insertNode(root, a); }

// ---------------- SEARCH -----------------
AVLNode* AVLTree::searchNode(AVLNode* node, long accNo) {
    if (!node) return nullptr;

    if (node->getAccount()->getAccountNumber() == accNo) return node;
    if (accNo < node->getAccount()->getAccountNumber())
        return searchNode(node->left, accNo);
    else
        return searchNode(node->right, accNo);
}

Account* AVLTree::search(long accNo) {
    AVLNode* node = searchNode(root, accNo);
    return node ? node->getAccount() : nullptr;
}

// ---------------- DISPLAY -----------------
void AVLTree::inorderPrint(AVLNode* node) {
    if (!node) return;
    inorderPrint(node->left);
    // Table-style row
    Account* acc = node->getAccount();
    cout << "-------------------------------------------------------------\n";
    cout << "ID: " << acc->getAccountNumber()
         << "  |  Name: " << acc->getName()
         << "  |  Email: " << acc->getEmail()
         << "  |  Balance: " << acc->getBalance() << "\n";
    inorderPrint(node->right);
}

void AVLTree::displayAll() {
    if (!root) {
        cout << "No accounts available.\n";
        return;
    }
    // Header
    cout << "================== ALL ACCOUNT DETAILS ==================\n";
    inorderPrint(root);
    cout << "======================== END ============================\n";
}

// ---------------- DELETE HELPERS -----------------
AVLNode* AVLTree::findMin(AVLNode* node) {
    AVLNode* cur = node;
    while (cur && cur->left) cur = cur->left;
    return cur;
}

AVLNode* AVLTree::deleteNode(AVLNode* node, long accNo) {
    if (!node) return nullptr;

    if (accNo < node->getAccount()->getAccountNumber())
        node->left = deleteNode(node->left, accNo);
    else if (accNo > node->getAccount()->getAccountNumber())
        node->right = deleteNode(node->right, accNo);
    else {
        // node to be deleted found
        if (!node->left || !node->right) {
            AVLNode* temp = node->left ? node->left : node->right;
            if (!temp) {
                // no child
                // delete account owned by this node (if any)
                Account* accToDelete = node->getAccount();
                delete accToDelete;
                // delete node itself
                delete node;
                return nullptr;
            } else {
                // one child: copy contents from temp into node structure
                // We must preserve ownership correctly:
                // - delete current node's Account (owned by node)
                // - take ownership of temp->getAccount() by assigning pointer to node
                Account* oldAcc = node->getAccount();
                Account* tempAcc = temp->getAccount();

                // set node's account pointer to tempAcc (using setter)
                node->setAccount(tempAcc);

                // adopt temp's children into node
                node->left = temp->left;
                node->right = temp->right;
                node->height = temp->height;

                // avoid deleting tempAcc twice: temp must no longer own the pointer
                temp->setAccount(nullptr);

                // delete old account and temp node
                delete oldAcc;
                delete temp;
            }
        } else {
            // two children: get inorder successor (smallest in right subtree)
            AVLNode* temp = findMin(node->right);
            Account* succAcc = temp->getAccount();

            // replace node's account pointer with successor's pointer
            // delete current node's account first
            Account* oldAcc = node->getAccount();
            node->setAccount(succAcc);

            // prevent successor node from deleting the Account pointer when it's removed:
            temp->setAccount(nullptr);

            // delete old account object
            delete oldAcc;

            // remove successor node from right subtree
            node->right = deleteNode(node->right, succAcc->getAccountNumber());
        }
    }

    if (!node) return nullptr;

    // update height and balance
    node->height = 1 + max(height(node->left), height(node->right));
    int bf = balanceFactor(node);

    // balance the node
    if (bf > 1 && balanceFactor(node->left) >= 0) return rotateRight(node);
    if (bf > 1 && balanceFactor(node->left) < 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if (bf < -1 && balanceFactor(node->right) <= 0) return rotateLeft(node);
    if (bf < -1 && balanceFactor(node->right) > 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

bool AVLTree::remove(long accNo) {
    if (!search(accNo)) return false;
    root = deleteNode(root, accNo);
    return true;
}

// ---------------- DESTROY -----------------
void AVLTree::destroyAll(AVLNode* node) {
    if (!node) return;
    destroyAll(node->left);
    destroyAll(node->right);
    delete node->getAccount();
    delete node;
}
