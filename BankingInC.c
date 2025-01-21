#include <stdio.h>
#include <string.h>


typedef struct {
    int accountNumber;
    char name[50];
    float balance;
    int pin;
} Account;

 char *FILENAME = "Accounts.txt";
 char *OUTPUT_FILE = "Output.txt";


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


    loadAccounts(accounts, &count);
    do {
        printf("\n--- Banking System Menu ---\n");
        printf("1. Create a New Account\n");
        printf("2. Display Accounts(Authorized Use Only)\n");
        printf("3. Deposit Money\n");
        printf("4. Withdraw Money\n");
        printf("5. Search Account\n");
        printf("6. Save & Exit\n");
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
                updateBalance(accounts, count, 1);
                break;
            case 4:
                updateBalance(accounts, count, 0);
                break;
            case 5:
                searchAccount(accounts, count);
                break;
            case 6:

                saveAccountsToFile(accounts, count, OUTPUT_FILE);

                copyOutputToAccounts();
                printf("Exiting the program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 6);

    return 0;
}


void loadAccounts(Account accounts[], int *count) {
    FILE *file = fopen(FILENAME, "r");
    if (file) {
        printf("Loading existing accounts from Accounts.txt...\n");
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

void saveAccountsToFile(const Account accounts[], int count, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file) {
        for (int i = 0; i < count; i++) {
            fprintf(file, "%d %s %.2f %d\n", accounts[i].accountNumber,
                    accounts[i].name, accounts[i].balance, accounts[i].pin);
        }
        fclose(file);
        printf("Accounts have been saved to %s.\n", filename);
    }
}


void copyOutputToAccounts() {
    FILE *outputFile = fopen(OUTPUT_FILE, "r");
    FILE *accountsFile = fopen(FILENAME, "w");

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


void createAccount(Account accounts[], int *count) {
    printf("\n--- Create Account ---\n");
    Account newAccount;

    printf("Enter Phone number(Enter number without 0 in front): ");
    scanf("%d", &newAccount.accountNumber);

    for (int i = 0; i < *count; i++) {
        if (accounts[i].accountNumber == newAccount.accountNumber) {
            printf("Account number already exists. Please try again.\n");
            return;
        }
    }

    printf("Enter name(Use '_' Instead of space ): ");
    scanf(" %[^\n]", newAccount.name);
    printf("Enter initial deposit: ");
    scanf("%f", &newAccount.balance);

    do {
        printf("Set a 5-digit PIN: ");
        scanf("%d", &newAccount.pin);
        if (newAccount.pin < 10000 || newAccount.pin > 99999) {
            printf("Invalid PIN. Please enter a 5-digit PIN.\n");
        }
    } while (newAccount.pin < 10000 || newAccount.pin > 99999);

    accounts[(*count)++] = newAccount;
    printf("Account created successfully!\n");


    saveAccountsToFile(accounts, *count, OUTPUT_FILE);
}


void displayAccounts(const Account accounts[], int count) {
    int key=17122845,User_key;

    printf("Enter Key :");
    scanf("%d",&User_key);
    if(key==User_key){
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
    else{
        printf("Wrong key\n");
    }
}


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


void updateBalance(Account accounts[], int count, int isDeposit) {
    int accNo;
    float amount;

    printf("\n--- %s Money ---\n", isDeposit ? "Deposit" : "Withdraw");
    printf("Enter Account number: ");
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


            saveAccountsToFile(accounts, count, OUTPUT_FILE);
            return;
        }
    }
    printf("Account not found.\n");
}


void searchAccount(const Account accounts[], int count) {
    int accNo;
    printf("\n--- Search Account ---\n");
    printf("Enter Account number: ");
    scanf("%d", &accNo);

    for (int i = 0; i < count; i++) {
        if (accounts[i].accountNumber == accNo) {
            if (!verifyPIN(&accounts[i])) return;

            printf("Account Number: %d | Name: %s | Balance: %.2f\n",
                   accounts[i].accountNumber, accounts[i].name, accounts[i].balance);
            return;
        }
    }
    printf("Account not found.\n");
}


   
   
    


  
       
