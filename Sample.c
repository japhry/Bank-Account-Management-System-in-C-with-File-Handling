//Bank Account Management System in C with File Handling - Made With ❤️ By Japhary Hashim ... Follow me on IG As @Japhry_

#include <stdio.h>
#include <stdlib.h>

struct BankAccount {
    int accountNumber;
    char accountHolder[100];
    float balance;
};

void createAccount() {
    struct BankAccount account;
    printf("Enter account number: ");
    scanf("%d", &account.accountNumber);
    printf("Enter account holder name: ");
    scanf("%s", account.accountHolder);
    account.balance = 0.0;

    FILE *file = fopen("accounts.txt", "a");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    fprintf(file, "%d %s %.2f\n", account.accountNumber, account.accountHolder, account.balance);
    fclose(file);

    printf("Account created successfully.\n");
}

void deposit() {
    int accountNumber;
    float amount;

    printf("Enter account number: ");
    scanf("%d", &accountNumber);
    printf("Enter amount to deposit: ");
    scanf("%f", &amount);

    FILE *file = fopen("accounts.txt", "r+");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    struct BankAccount account;
    int found = 0;
    while (fscanf(file, "%d %s %f", &account.accountNumber, account.accountHolder, &account.balance) != EOF) {
        if (account.accountNumber == accountNumber) {
            found = 1;
            account.balance += amount;
            fseek(file, -sizeof(account), SEEK_CUR);
            fprintf(file, "%d %s %.2f\n", account.accountNumber, account.accountHolder, account.balance);
            break;
        }
    }

    fclose(file);

    if (found) {
        printf("Amount deposited successfully.\n");
    } else {
        printf("Account not found.\n");
    }
}

void withdraw() {
    int accountNumber;
    float amount;

    printf("Enter account number: ");
    scanf("%d", &accountNumber);
    printf("Enter amount to withdraw: ");
    scanf("%f", &amount);

    FILE *file = fopen("accounts.txt", "r+");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    struct BankAccount account;
    int found = 0;
    while (fscanf(file, "%d %s %f", &account.accountNumber, account.accountHolder, &account.balance) != EOF) {
        if (account.accountNumber == accountNumber) {
            found = 1;
            if (account.balance >= amount) {
                account.balance -= amount;
                fseek(file, -sizeof(account), SEEK_CUR);
                fprintf(file, "%d %s %.2f\n", account.accountNumber, account.accountHolder, account.balance);
            } else {
                printf("Insufficient balance.\n");
            }
            break;
        }
    }

    fclose(file);

    if (found) {
        printf("Amount withdrawn successfully.\n");
    } else {
        printf("Account not found.\n");
    }
}

void displayAccount() {
    int accountNumber;

    printf("Enter account number: ");
    scanf("%d", &accountNumber);

    FILE *file = fopen("accounts.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    struct BankAccount account;
    int found = 0;
    while (fscanf(file, "%d %s %f", &account.accountNumber, account.accountHolder, &account.balance) != EOF) {
        if (account.accountNumber == accountNumber) {
            found = 1;
            printf("Account Number: %d\n", account.accountNumber);
            printf("Account Holder: %s\n", account.accountHolder);
            printf("Balance: %.2f\n", account.balance);
            break;
        }
    }

    fclose(file);

    if (!found) {
        printf("Account not found.\n");
    }
}

int main() {
    int choice;

    do {
        printf("\nBank Account System\n");
        printf("1. Create Account\n");
        printf("2. Deposit Amount\n");
        printf("3. Withdraw Amount\n");
        printf("4. Display Account Details\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                createAccount();
                break;
            case 2:
                deposit();
                break;
            case 3:
                withdraw();
                break;
            case 4:
                displayAccount();
                break;
            case 5:
                printf("Thank you for using the Japhary bank account system.\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 5);

    return 0;
}
