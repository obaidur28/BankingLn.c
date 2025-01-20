#include <stdio.h>
#include <string.h>

// Define a structure for an account
typedef struct {
    int accountNumber;
    char name[50];
    float balance;
    int pin; // 5-digit PIN for security
} Account;

 char *FILENAME = "Accounts.txt";
 char *OUTPUT_FILE = "Output.txt";

// Function prototypes
void loadAccounts(Account accounts[], int *count);
void saveAccountsToFile(const Account accounts[], int count, const char *filename);
void createAccount(Account accounts[], int *count);
void displayAccounts(const Account accounts[], int count);
void updateBalance(Account accounts[], int count, int isDeposit);
void searchAccount(const Account accounts[], int count);
int verifyPIN(const Account *account);
void copyOutputToAccounts();

int main() {
    Account accounts[100];
    int count = 0;
    int choice;

    // Load accounts from Accounts.txt
    loadAccounts(accounts, &count);

    // Display existing accounts after loading
    printf("\n--- Existing Accounts ---\n");
    displayAccounts(accounts, count);  // Show all accounts

    do {
        printf("\n--- Banking System Menu ---\n");
        printf("1. Create Account\n");
        printf("2. Display Accounts\n");
        printf("3. Deposit Money\n");
        printf("4. Withdraw Money\n");
        printf("5. Search Account\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                createAccount(accounts, &count);
                break;
            case 2:
                displayAccounts(accounts, count);
                break;
            case 3:
                updateBalance(accounts, count, 1); // Deposit
                break;
            case 4:
                updateBalance(accounts, count, 0); // Withdraw
                break;
            case 5:
                searchAccount(accounts, count);
                break;
            case 6:
                // Save updated accounts data to Output.txt
                saveAccountsToFile(accounts, count, OUTPUT_FILE);
                // Copy Output.txt back to Accounts.txt
                copyOutputToAccounts();
                printf("Exiting the program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 6);

    return 0;
}

// Load accounts from Accounts.txt
void loadAccounts(Account accounts[], int *count) {
    FILE *file = fopen(FILENAME, "r");
    if (file) {
        while (fscanf(file, "%d %s %f %d", &accounts[*count].accountNumber,
                      accounts[*count].name, &accounts[*count].balance,
                      &accounts[*count].pin) == 4) {
            (*count)++;
        }
        fclose(file);
        printf("Loaded %d accounts from Accounts.txt.\n", *count);
    } else {
        printf("No existing accounts found in Accounts.txt. Starting with an empty database.\n");
    }
}

// Save account information to a file (used for saving to Output.txt)
void saveAccountsToFile(const Account accounts[], int count, const char *filename) {
    FILE *file = fopen(filename, "w");  // "w" mode to overwrite the file
    if (file) {
        for (int i = 0; i < count; i++) {
            fprintf(file, "%d %s %.2f %d\n", accounts[i].accountNumber,
                    accounts[i].name, accounts[i].balance, accounts[i].pin);
        }
        fclose(file);
        printf("Accounts have been saved to %s.\n", filename);
    }
}

// Copy the contents of Output.txt to Accounts.txt
void copyOutputToAccounts() {
    FILE *outputFile = fopen(OUTPUT_FILE, "r");
    FILE *accountsFile = fopen(FILENAME, "w"); // Open Accounts.txt in write mode to overwrite

    if (outputFile && accountsFile) {
        char ch;
        while ((ch = fgetc(outputFile)) != EOF) {
            fputc(ch, accountsFile);
        }
        fclose(outputFile);
        fclose(accountsFile);
        printf("Contents from Output.txt copied to Accounts.txt.\n");
    } else {
        printf("Error copying data from Output.txt to Accounts.txt.\n");
    }
}

// Create a new account
void createAccount(Account accounts[], int *count) {
    printf("\n--- Create Account ---\n");
    Account newAccount;

    // Allow user to specify account number
    printf("Enter account number: ");
    scanf("%d", &newAccount.accountNumber);

    // Check for duplicate account number
    for (int i = 0; i < *count; i++) {
        if (accounts[i].accountNumber == newAccount.accountNumber) {
            printf("Account number already exists. Please try again.\n");
            return;
        }
    }

    printf("Enter name: ");
    scanf(" %[^\n]", newAccount.name);
    printf("Enter initial deposit: ");
    scanf("%f", &newAccount.balance);

    // Set PIN
    do {
        printf("Set a 5-digit PIN: ");
        scanf("%d", &newAccount.pin);
        if (newAccount.pin < 10000 || newAccount.pin > 99999) {
            printf("Invalid PIN. Please enter a 5-digit PIN.\n");
        }
    } while (newAccount.pin < 10000 || newAccount.pin > 99999);

    accounts[(*count)++] = newAccount;
    printf("Account created successfully!\n");

    // Save the updated accounts to Output.txt
    saveAccountsToFile(accounts, *count, OUTPUT_FILE);
}

// Display all accounts
void displayAccounts(const Account accounts[], int count) {
    printf("\n--- Account List ---\n");
    if (count == 0) {
        printf("No accounts found.\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        printf("Account Number: %d | Name: %s | Balance: %.2f\n",
               accounts[i].accountNumber, accounts[i].name, accounts[i].balance);
    }
}

// Verify PIN
int verifyPIN(const Account *account) {
    int enteredPIN;
    printf("Enter your 5-digit PIN: ");
    scanf("%d", &enteredPIN);
    if (enteredPIN == account->pin) {
        return 1;
    } else {
        printf("Incorrect PIN.\n");
        return 0;
    }
}

// Deposit or Withdraw Money
void updateBalance(Account accounts[], int count, int isDeposit) {
    int accNo;
    float amount;

    printf("\n--- %s Money ---\n", isDeposit ? "Deposit" : "Withdraw");
    printf("Enter account number: ");
    scanf("%d", &accNo);

    for (int i = 0; i < count; i++) {
        if (accounts[i].accountNumber == accNo) {
            if (!verifyPIN(&accounts[i])) return;

            printf("Enter amount: ");
            scanf("%f", &amount);

            if (!isDeposit && accounts[i].balance < amount) {
                printf("Insufficient balance.\n");
                return;
            }

            accounts[i].balance += isDeposit ? amount : -amount;
            printf("Transaction successful! New Balance: %.2f\n", accounts[i].balance);

            // Save the updated accounts to Output.txt
            saveAccountsToFile(accounts, count, OUTPUT_FILE);
            return;
        }
    }
    printf("Account not found.\n");
}

// Search for an account
void searchAccount(const Account accounts[], int count) {
    int accNo;
    printf("\n--- Search Account ---\n");
    printf("Enter account number: ");
    scanf("%d", &accNo);

    for (int i = 0; i < count; i++) {
        if (accounts[i].accountNumber == accNo) {
            if (!verifyPIN(&accounts[i])) return;

            printf("Account Number: %d | Name: %s | Balance: %.2f\n",
                   accounts[i].accountNumber, accounts[i].name, accounts[i].balance);
            return;
        }
    }
    printf("Account not found.\n");
}
