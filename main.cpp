
#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>
#include <string>
#include <vector>

using namespace std;

const string FILE_NAME = "accounts.txt";

// Class representing a bank account
class BankAccount {
private:
    int accountNumber;
    string accountHolder;
    double balance;

public:
    BankAccount(int number, const string& holder, double initialBalance)
        : accountNumber(number),
          accountHolder(holder),
          balance(initialBalance) {}

    // Public functions provide controlled access to private data
    int getAccountNumber() const {
        return accountNumber;
    }

    string getAccountHolder() const {
        return accountHolder;
    }

    double getBalance() const {
        return balance;
    }

    void deposit(double amount) {
        balance += amount;
    }

    bool withdraw(double amount) {
        if (amount <= 0 || amount > balance) {
            return false;
        }

        balance -= amount;
        return true;
    }
};

// Class managing all bank operations
class Bank {
private:
    vector<BankAccount> accounts;
    int nextAccountNumber = 1001;

    // Find an account and return its position in the vector
    int findAccountIndex(int number) const {
        for (size_t i = 0; i < accounts.size(); ++i) {
            if (accounts[i].getAccountNumber() == number) {
                return static_cast<int>(i);
            }
        }

        return -1;
    }

    // Read previously saved accounts from the file
    void loadAccounts() {
        ifstream inputFile(FILE_NAME);

        if (!inputFile) {
            return;
        }

        int number;
        string holder;
        double balance;

        while (inputFile >> number >> quoted(holder) >> balance) {
            if (balance >= 0) {
                accounts.emplace_back(number, holder, balance);

                if (number >= nextAccountNumber) {
                    nextAccountNumber = number + 1;
                }
            }
        }
    }

    // Save all accounts to the file
    bool saveAccounts() const {
        ofstream outputFile(FILE_NAME);

        if (!outputFile) {
            cout << "Error: Could not save account data.\n";
            return false;
        }

        outputFile << fixed << setprecision(2);

        for (const BankAccount& account : accounts) {
            outputFile << account.getAccountNumber() << ' '
                       << quoted(account.getAccountHolder()) << ' '
                       << account.getBalance() << '\n';
        }

        return true;
    }

    // Read an integer safely
    static int readInteger(const string& prompt) {
        int value;

        while (true) {
            cout << prompt;

            if (cin >> value) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            }

            cout << "Invalid input. Enter a whole number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    // Read a positive amount
    static double readAmount(const string& prompt) {
        double amount;

        while (true) {
            cout << prompt;

            if (cin >> amount && amount > 0) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return amount;
            }

            cout << "Enter a valid amount greater than zero.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    // Open a new account
    void createAccount() {
        string holder;

        cout << "Enter account holder's full name: ";
        getline(cin, holder);

        if (holder.empty()) {
            cout << "Name cannot be empty. Account not created.\n";
            return;
        }

        double initialBalance;

        cout << "Enter initial deposit (0 or more): ";

        while (!(cin >> initialBalance) || initialBalance < 0) {
            cout << "Enter a valid amount (0 or more): ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        accounts.emplace_back(
            nextAccountNumber, holder, initialBalance
        );

        cout << "Account created successfully!\n";
        cout << "Your account number is: "
             << nextAccountNumber << '\n';

        ++nextAccountNumber;
        saveAccounts();
    }

    // Deposit money into an account
    void depositMoney() {
        int number = readInteger("Enter account number: ");
        int index = findAccountIndex(number);

        if (index == -1) {
            cout << "Account not found.\n";
            return;
        }

        double amount = readAmount("Enter deposit amount: ");

        accounts[index].deposit(amount);

        cout << "Deposit successful. New balance: Rs. "
             << fixed << setprecision(2)
             << accounts[index].getBalance() << '\n';

        saveAccounts();
    }

    // Withdraw money from an account
    void withdrawMoney() {
        int number = readInteger("Enter account number: ");
        int index = findAccountIndex(number);

        if (index == -1) {
            cout << "Account not found.\n";
            return;
        }

        double amount = readAmount("Enter withdrawal amount: ");

        if (accounts[index].withdraw(amount)) {
            cout << "Withdrawal successful. New balance: Rs. "
                 << fixed << setprecision(2)
                 << accounts[index].getBalance() << '\n';

            saveAccounts();
        } else {
            cout << "Insufficient balance. Withdrawal cancelled.\n";
        }
    }

    // Display the balance of a particular account
    void checkBalance() const {
        int number = readInteger("Enter account number: ");
        int index = findAccountIndex(number);

        if (index == -1) {
            cout << "Account not found.\n";
            return;
        }

        cout << "Account holder: "
             << accounts[index].getAccountHolder() << '\n';

        cout << "Account number: "
             << accounts[index].getAccountNumber() << '\n';

        cout << "Current balance: Rs. "
             << fixed << setprecision(2)
             << accounts[index].getBalance() << '\n';
    }

    // Display all saved accounts
    void displayAllAccounts() const {
        if (accounts.empty()) {
            cout << "No accounts have been created yet.\n";
            return;
        }

        cout << '\n' << left
             << setw(15) << "Account No."
             << setw(25) << "Account Holder"
             << "Balance (Rs.)\n";

        cout << string(55, '-') << '\n';
        cout << fixed << setprecision(2);

        for (const BankAccount& account : accounts) {
            cout << left
                 << setw(15) << account.getAccountNumber()
                 << setw(25) << account.getAccountHolder()
                 << account.getBalance() << '\n';
        }
    }

public:
    // Load saved data when the Bank object is created
    Bank() {
        loadAccounts();
    }

    // Main menu
    void run() {
        int choice;

        do {
            cout << "\n========== BANK MANAGEMENT SYSTEM ==========\n";
            cout << "1. Open New Account\n";
            cout << "2. Deposit Money\n";
            cout << "3. Withdraw Money\n";
            cout << "4. Check Account Balance\n";
            cout << "5. Display All Accounts\n";
            cout << "6. Exit\n";

            choice = readInteger("Enter your choice: ");

            switch (choice) {
                case 1:
                    createAccount();
                    break;

                case 2:
                    depositMoney();
                    break;

                case 3:
                    withdrawMoney();
                    break;

                case 4:
                    checkBalance();
                    break;

                case 5:
                    displayAllAccounts();
                    break;

                case 6:
                    saveAccounts();
                    cout << "Thank you for using the Bank Management System.\n";
                    break;

                default:
                    cout << "Invalid choice. Please select 1 to 6.\n";
            }

        } while (choice != 6);
    }
};

int main() {
    Bank bank;
    bank.run();

    return 0;
}
