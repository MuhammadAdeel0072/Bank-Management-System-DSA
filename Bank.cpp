#include "Bank.h"
#include <iostream>
#include <limits>
#include <regex>
using namespace std;

// ---------------- Bank -----------------
Bank::Bank() {
    nextAccountNumber = 1001;
}

// Manual email validation for VS2012 (but we use regex for stricter check)
bool Bank::validateEmail(const string& email) const {
    // VS2012 requires double escaping for '\.'
    std::regex pattern("^[A-Za-z0-9._%+\\-]+@[A-Za-z0-9.\\-]+\\.[A-Za-z]{2,}$");
    return std::regex_match(email, pattern);
}

bool Bank::validateName(const string& name) const {
    std::regex pattern("^[A-Za-z ]+$");
    return std::regex_match(name, pattern);
}
// Input helpers
int Bank::getIntInput(const string& prompt) const {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Enter an integer.\n";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }
}

double Bank::getDoubleInput(const string& prompt) const {
    double value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Enter a number.\n";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }
}

string Bank::getStringInput(const string& prompt) const {
    string input;
    cout << prompt;
    // Use getline to allow spaces in names (but trim leading/trailing spaces)
    getline(cin, input);
    if (input.size() == 0) {
        // if previous extraction left newline, try again
        getline(cin, input);
    }
    // simple trim (leading/trailing)
    size_t start = input.find_first_not_of(" \t\r\n");
    size_t end = input.find_last_not_of(" \t\r\n");
    if (start == string::npos) return "";
    return input.substr(start, end - start + 1);
}

void Bank::clearScreen() const {
#if defined(_WIN32) || defined(_WIN64)
    system("CLS");
#else
    system("clear");
#endif
}

// Persistent system start
void Bank::start() {
    while (true) {
        cout << "\n===== BANK SYSTEM =====\n";
        cout << "1) Admin\n";
        cout << "2) Customer\n";
        cout << "3) Clear Screen\n";
        cout << "4) Exit\n";
        int choice = getIntInput("Enter choice: ");

        switch (choice) {
            case 1: adminLogin(); break;
            case 2: customerLogin(); break;
            case 3: clearScreen(); break;
            case 4:
                cout << "Goodbye.\n";
                return;
            default: cout << "Invalid choice.\n"; break;
        }
    }
}

// ---------------- LOGIN (kept for compatibility) -----------------
void Bank::login() {
    string role;
    while (true) {
        cout << "\nLogin as (admin/customer): ";
        cin >> role;
        if (role == "admin") {
            string username = getStringInput("Enter username: ");
            string password = getStringInput("Enter password: ");

            if (username == "Bank" && password == "0072") {
                cout << "Admin login successful.\n";
                adminMenu();
                break;
            } else {
                cout << "Invalid username or password.\n";
            }
        } else if (role == "customer") {
            long accNo = getIntInput("Enter Account#: ");
            string enteredPIN = getStringInput("Enter PIN: ");

            Account* acc = accounts.search(accNo);
            if(!acc) { cout << "Account not found.\n"; continue; }

            if(acc->getPIN() == enteredPIN) {
                cout << "Customer login successful.\n";
                customerMenu(acc);  // pass logged-in account
                break;
            } else {
                cout << "Incorrect PIN.\n";
            }
        } else {
            cout << "Invalid role. Try again.\n";
        }
    }
}

// admin login helper used by start()
void Bank::adminLogin() {
    string username;
    string password;
    cout << "\n-- Admin Login --\n";
    username = getStringInput("Enter username: ");
    password = getStringInput("Enter password: ");

    if (username == "Bank" && password == "0072") {
        cout << "Admin login successful.\n";
        adminMenu();
    } else {
        cout << "Invalid username or password.\n";
    }
}

// customer login helper used by start()
void Bank::customerLogin() {
    cout << "\n-- Customer Login --\n";
    long accNo = getIntInput("Enter Account#: ");
    string enteredPIN = getStringInput("Enter PIN: ");

    Account* acc = accounts.search(accNo);
    if(!acc) { cout << "Account not found.\n"; return; }

    if(acc->getPIN() == enteredPIN) {
        cout << "Customer login successful.\n";
        customerMenu(acc);
    } else {
        cout << "Incorrect PIN.\n";
    }
}

// ---------------- ADMIN MENU -----------------
void Bank::adminMenu() {
    int choice;
    while (true) {
        cout << "\n===== ADMIN MENU =====\n";
        cout << "1. Create Account\n";
        cout << "2. Delete Account\n";
        cout << "3. Search Account\n";
        cout << "4. Display All Accounts\n";
        cout << "0. Logout\n";
        choice = getIntInput("Enter choice: ");

        switch (choice) {
            case 0: return; // logout to start menu
            case 1: createAccount(); break;
            case 2: deleteAccount(); break;
            case 3: searchAccount(); break;
            case 4: displayAllAccounts(); break;
            default: cout << "Invalid choice.\n"; break;
        }
    }
}

// ---------------- CUSTOMER MENU -----------------
void Bank::customerMenu(Account* acc) {
    int choice;
    while(true) {
        cout << "\n===== CUSTOMER MENU =====\n";
        cout << "1. Deposit Money\n";
        cout << "2. Withdraw Money\n";
        cout << "3. View Account Details\n";
        cout << "4. View Transactions\n";
        cout << "5. Transfer Funds\n";
        cout << "6. Undo Last Transaction\n";
        cout << "0. Logout\n";
        choice = getIntInput("Enter choice: ");

        switch(choice) {
            case 0: return;
            case 1: depositMoney(acc); break;
            case 2: withdrawMoney(acc); break;
            case 3: searchAccount(acc); break;
            case 4: viewTransactions(acc); break;
            case 5: transferFunds(acc); break;
            case 6: undoLastTransaction(acc); break;
            default: cout << "Invalid choice.\n"; break;
        }
    }
}

// ---------------- ACCOUNT OPERATIONS -----------------
void Bank::createAccount() {
    string name;
    while (true) {
        cout << "Enter Name (letters and spaces only): ";
        name = getStringInput("");
        if (!validateName(name)) cout << "Invalid name. Only letters and spaces allowed.\n";
        else break;
    }

    string email;
    while (true) {
        cout << "Enter Email: ";
        email = getStringInput("");
        if (!validateEmail(email)) cout << "Invalid email format. Example: user@example.com\n";
        else break;
    }

    double balance = getDoubleInput("Enter Opening Balance: ");
    while(balance < 0) {
         cout << "Balance cannot be negative.\n"; 
         balance = getDoubleInput("Enter Opening Balance: "); 
    }
    string pin;
    while(true) {
        cout << "Enter 4-digit PIN for account: ";
        pin = getStringInput("");
        if(pin.length() != 4) cout << "PIN must be 4 digits. Try again.\n";
        else break;
    }

    Account* acc = new Account(nextAccountNumber, name, email, balance, pin);
    accounts.insert(acc);
    cout << "\nAccount created successfully. Account#: " << nextAccountNumber << "\n";
    nextAccountNumber++;
}

void Bank::deleteAccount() {
    long accNo = getIntInput("Enter Account# to delete: ");
    Account* acc = accounts.search(accNo);
    if (!acc) { cout << "Account not found.\n"; return; }

    cout << "\nAccount found:\n";
    acc->showAccount();
    cout << "Are you sure you want to delete this account? (y/n): ";
    string ans = getStringInput("");
    if (ans == "y" || ans == "Y") {
        if (accounts.remove(accNo)) cout << "Account deleted successfully.\n";
        else cout << "Failed to delete account.\n";
    } else {
        cout << "Delete cancelled.\n";
    }
}

void Bank::depositMoney(Account* acc) {
    double amt = getDoubleInput("Enter Deposit Amount: ");
    acc->deposit(amt);
    undoStack.push(acc, acc->getTransactions());
    cout << "Deposit successful.\n";
}

void Bank::withdrawMoney(Account* acc) {
    double amt = getDoubleInput("Enter Withdraw Amount: ");

    // Minimum withdrawal
    if (amt < 100) {
        cout << "Minimum withdrawal amount is Rs. 100.\n";
        return;
    }

    // Maximum withdrawal per transaction
    if (amt > 500000) {
        cout << "Withdrawal limit exceeded. Max allowed is Rs. 500,000.\n";
        return;
    }

    // Confirm transaction 
    string confirm;
    while (true) {
        confirm = getStringInput("Do you want to continue? (y/n): ");
        if (confirm == "y" || confirm == "Y") break;
        else if (confirm == "n" || confirm == "N") {
            cout << "Withdrawal cancelled.\n";
            return;
        } else {
            cout << "Invalid input. Enter 'y' or 'n'.\n";
        }
    }

    // Re-enter PIN at last step
    string pinCheck = getStringInput("Re-enter your PIN to confirm: ");
    if (pinCheck != acc->getPIN()) {
        cout << "Incorrect PIN. Withdrawal cancelled.\n";
        return;
    }

    // Perform withdrawal
    if(acc->withdraw(amt)) {
        undoStack.push(acc, acc->getTransactions());
        cout << "Withdraw successful.\n";
    }
}


void Bank::searchAccount() {
    long accNo = getIntInput("Enter Account#: ");
    Account* acc = accounts.search(accNo);
    if (!acc) { cout << "Account not found.\n"; return; }
    cout << "\nAccount detail:\n";
    acc->showAccount();
}

void Bank::searchAccount(Account* acc) {
    cout << "\nAccount detail:\n";
    acc->showAccount();
}

void Bank::displayAllAccounts() {
    accounts.displayAll();
}

void Bank::viewTransactions(Account* acc) {
    acc->showTransactions();
}

void Bank::transferFunds(Account* fromAcc) {
    long toAccNo = getIntInput("Enter To Account#: ");
    double amt = getDoubleInput("Enter Amount(Min 100) : ");

    // Self-transfer not allowed
    if (toAccNo == fromAcc->getAccountNumber()) {
        cout << "You cannot transfer money to the same account.\n";
        return;
    }

    // Minimum transfer amount
    if (amt < 100) {
        cout << "Minimum transfer amount is Rs. 100.\n";
        return;
    }

    // Maximum transfer amount
    if (amt > 500000) {
        cout << "Transfer limit exceeded. Max allowed is Rs. 500,000.\n";
        return;
    }

    Account* toAcc = accounts.search(toAccNo);
    if (!toAcc) {
        cout << "Recipient account not found.\n";
        return;
    }

    // Show confirmation BEFORE asking PIN
    cout << "\n--- Transfer Confirmation ---\n";
    cout << "From Account#: " << fromAcc->getAccountNumber() << "\n";
    cout << "To Account#:   " << toAccNo << "\n";
    cout << "Amount:        " << amt << "\n";
    cout << "-----------------------------\n";

    string confirm = getStringInput("Do you want to continue? (y/n): ");
    if (confirm != "y" && confirm != "Y") {
        cout << "Transfer cancelled.\n";
        return;
    }

    // NOW take PIN after everything else is OK
    string pinCheck = getStringInput("Re-enter your PIN to confirm: ");
    if (pinCheck != fromAcc->getPIN()) {
        cout << "Incorrect PIN. Transfer cancelled.\n";
        return;
    }

    // Perform transaction
    if (fromAcc->withdraw(amt)) {
        toAcc->deposit(amt);

        transferQueue.enqueue(fromAcc, toAcc, amt);

        cout << "Transfer successful.\n";
    }
}


void Bank::undoLastTransaction(Account* acc) {
    if (undoStack.isEmpty()) { cout << "No transactions to undo.\n"; return; }

    StackNode* node = undoStack.pop();
    if (!node) { cout << "No transactions to undo.\n"; return; }

    Account* trAcc = node->getAccount();
    Transaction* tr = node->getTransaction();

    // Only allow undo for logged-in account
    if(trAcc != acc) {
        undoStack.push(trAcc, tr);  // push it back
        cout << "No transactions to undo for your account.\n";
        return;
    }

    if (tr) {
        if (tr->getType() == "Deposit") acc->setBalance(acc->getBalance() - tr->getAmount());
        else if (tr->getType() == "Withdraw") acc->setBalance(acc->getBalance() + tr->getAmount());

        acc->setTransactions(tr->getNext());
        delete tr;
    }
    delete node;
    cout << "Last transaction undone.\n";
}
