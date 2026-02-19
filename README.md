# 💳 Bank Account Management System

<p align="center">
  <strong>A secure, file-based banking system built in C</strong>
</p>

<p align="center">
  Advanced account management • Safe file handling • Transaction logging • GitHub-ready console architecture
</p>

<p align="center">
  <a href="https://pixellinx.co.tz/">Pixellinx</a> •
  <a href="#features">Features</a> •
  <a href="#screenshots">Preview</a> •
  <a href="#getting-started">Getting Started</a> •
  <a href="#full-source-code">Full Source Code</a>
</p>

---

## 📌 Overview

The **Bank Account Management System** is a professional, file-based banking application written in pure C.

Originally built as a systems programming project, it has been upgraded into a robust, production-style console application demonstrating:

- Structured programming
- Safe file manipulation (temp-file rewrite pattern)
- Data integrity handling
- Transaction logging (audit trail)
- Financial precision (stores money as cents — no floating-point errors)

This project reflects practical backend logic often used in real banking systems — simplified for educational and open-source demonstration purposes.

---

## 🖼 Screenshots

> Main preview image (place this file in the repo root): **`main-acc.jpg`**

![Bank Account System](main-acc.jpg)

---

## 🚀 Features

### 🏦 Core Banking Operations
- ✅ Create account (unique account number validation)
- ✅ Deposit money
- ✅ Withdraw money (overdraft prevention)
- ✅ Transfer funds between accounts
- ✅ Display account details
- ✅ List all accounts
- ✅ Update account holder name
- ✅ Delete account

---

### 📂 File Handling (Safe & Professional)

Accounts are stored in a structured text format:

```
accountNumber|holderName|balanceCents
```

Example:

```
1001|Japhary Hashim|250000
1002|Pixellinx Client|500000
```

The system uses a **temporary file rewrite strategy** for all updates.  
This prevents corruption issues that occur when attempting unsafe in-place edits in text files.

---

### 💰 Financial Precision

- Money is stored as **cents (`long long`)**
- Eliminates floating-point rounding errors
- Ensures safe deposits, withdrawals, and transfers
- Accurate currency formatting during display

---

### 📊 Transaction Logging (Audit Trail)

Every action is recorded in:

```
transactions.txt
```

Log structure:

```
timestamp|transactionType|details|amountCents|result
```

Example:

```
2026-02-19 14:32:10|DEPOSIT|Acc:1001|50000|OK
```

This creates a full audit trail similar to real banking systems.

---

### 🛡 Data Validation & Protection

- Prevents duplicate account numbers
- Prevents negative deposits or withdrawals
- Prevents overdraft transactions
- Validates numeric input
- Graceful handling of missing files
- Controlled transaction history display (avoids runaway console output)

---

## 🧠 Architecture Highlights

This project demonstrates strong backend fundamentals:

- Modular function-based architecture
- Clean CLI menu loop
- Structured data parsing using delimiters
- Safe file rewriting pattern
- Timestamp generation
- Reusable mutation logic
- Clear separation of concerns

Designed to reflect production-level thinking in a simple console application.

---

## 🛠 Tech Stack

| Layer | Technology |
|------:|------------|
| Language | C |
| Storage | Text Files |
| Financial Handling | Integer Cents (`long long`) |
| Logging | Custom Transaction Log |
| Compiler | GCC / Clang |

---

## 📦 Getting Started

### ✅ Prerequisites

- GCC (or any C compiler)
- Terminal / Command Prompt

---

### 🔹 Clone the Repository

```bash
git clone https://github.com/japhry/Bank-Account-Management-System-in-C-with-File-Handling.git
cd Bank-Account-Management-System-in-C-with-File-Handling
```

### 🔹 Compile

```bash
gcc bank.c -o bank
```

Replace `bank.c` with your actual filename (e.g. `sample.c`) if different.

### 🔹 Run

**Linux / macOS:**

```bash
./bank
```

**Windows:**

```bash
bank.exe
```

---

## 📁 Project Structure

```
.
├── bank.c              # or sample.c — main source
├── accounts.txt        # created at runtime (account data)
├── transactions.txt    # created at runtime (audit log)
├── main-acc.jpg        # optional screenshot
└── README.md           # or BANK_README.md
```

---

## 🧪 Example Workflow

1. **Create Account**
2. **Deposit** 1000.00
3. **Withdraw** 250.00
4. **Transfer** 100.00 to another account
5. **View transaction history**
6. **List all accounts**

All actions are logged and safely persisted.

---

## 🎯 Why This Project Matters

This project showcases:

- Real-world file-based data management
- Safe update strategies for text databases
- Audit logging mechanisms
- Financial-safe computation
- Clean CLI program structure
- Strong C programming fundamentals

Ideal for:

- Systems programming portfolios
- Backend development showcases
- University / academic submissions
- Technical GitHub presence

---

## 🔮 Future Enhancements

- Account PIN authentication
- Admin mode
- CSV export
- Binary storage mode
- Search & sorting capabilities
- Data encryption
- Unit testing suite

---

## 📜 License

This project is open source.  
Feel free to use, modify, and build upon it. Attribution is appreciated.

---

## 👨‍💻 Author

**Coded by Japhary from [Pixellinx](https://pixellinx.co.tz/)**

---

## 📄 Full Source Code

The complete C implementation is below. Save it as `bank.c` (or `sample.c`) and compile with:

```bash
gcc bank.c -o bank
```

<details>
<summary>Click to expand full source (bank.c)</summary>

```c
/*
===============================================================================
 Bank Account Management System (File-based) — C
-------------------------------------------------------------------------------
 Coded by Japhary from Pixellinx (https://pixellinx.co.tz/)
 GitHub-ready version: safer I/O, better features, and robust file updating.
-------------------------------------------------------------------------------
 Features:
  - Create account (unique account numbers)
  - Deposit / Withdraw (validated)
  - Transfer between accounts
  - Display one account
  - List all accounts
  - Update account holder name
  - Delete account
  - Transaction history log (transactions.txt)
  - Stores money as "cents" (avoids floating-point errors)
  - Uses temp-file rewrite method (safe with text files)

 Files:
  - accounts.txt       (accountNumber|holderName|balanceCents)
  - transactions.txt   (timestamp|type|details|amountCents|result)

 Build:
  - gcc bank.c -o bank
 Run:
  - ./bank
===============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define ACCOUNTS_FILE      "accounts.txt"
#define TRANSACTIONS_FILE  "transactions.txt"

#define NAME_MAX_LEN 100
#define LINE_MAX_LEN 256

typedef long long cents_t;

typedef struct {
    int accountNumber;
    char accountHolder[NAME_MAX_LEN];
    cents_t balanceCents;
} BankAccount;

/* ----------------------------- Utilities ----------------------------- */

static void trim_newline(char *s) {
    size_t n = strlen(s);
    if (n > 0 && (s[n - 1] == '\n' || s[n - 1] == '\r')) s[n - 1] = '\0';
    n = strlen(s);
    if (n > 0 && (s[n - 1] == '\r')) s[n - 1] = '\0';
}

static void read_line(const char *prompt, char *out, size_t outSize) {
    if (prompt) printf("%s", prompt);
    if (!fgets(out, (int)outSize, stdin)) {
        out[0] = '\0';
        return;
    }
    trim_newline(out);
}

static int read_int(const char *prompt, int *value) {
    char buf[64];
    read_line(prompt, buf, sizeof(buf));
    if (buf[0] == '\0') return 0;
    char *end = NULL;
    long v = strtol(buf, &end, 10);
    if (end == buf || *end != '\0') return 0;
    if (v < -2147483648L || v > 2147483647L) return 0;
    *value = (int)v;
    return 1;
}

static int read_money_cents(const char *prompt, cents_t *centsOut) {
    char buf[64];
    read_line(prompt, buf, sizeof(buf));
    if (buf[0] == '\0') return 0;
    int sign = 1;
    char *p = buf;
    while (isspace((unsigned char)*p)) p++;
    if (*p == '-') { sign = -1; p++; }
    else if (*p == '+') { p++; }
    if (!isdigit((unsigned char)*p)) return 0;
    long long whole = 0;
    while (isdigit((unsigned char)*p)) {
        whole = whole * 10 + (*p - '0');
        p++;
    }
    long long frac = 0;
    int fracDigits = 0;
    if (*p == '.') {
        p++;
        while (isdigit((unsigned char)*p) && fracDigits < 2) {
            frac = frac * 10 + (*p - '0');
            fracDigits++;
            p++;
        }
        if (isdigit((unsigned char)*p)) return 0;
    }
    while (isspace((unsigned char)*p)) p++;
    if (*p != '\0') return 0;
    if (fracDigits == 0) frac = 0;
    else if (fracDigits == 1) frac *= 10;
    *centsOut = (cents_t)sign * ((cents_t)whole * 100 + (cents_t)frac);
    return 1;
}

static void format_money(cents_t cents, char *out, size_t outSize) {
    long long absC = (cents < 0) ? -cents : cents;
    long long whole = absC / 100;
    long long frac = absC % 100;
    snprintf(out, outSize, "%s%lld.%02lld", (cents < 0 ? "-" : ""), whole, frac);
}

static void now_timestamp(char *out, size_t outSize) {
    time_t t = time(NULL);
    struct tm *tmv = localtime(&t);
    strftime(out, outSize, "%Y-%m-%d %H:%M:%S", tmv);
}

/* ----------------------------- File Parsing ----------------------------- */

static int parse_account_line(const char *line, BankAccount *acc) {
    char buf[LINE_MAX_LEN];
    strncpy(buf, line, sizeof(buf));
    buf[sizeof(buf) - 1] = '\0';
    trim_newline(buf);
    char *a = strtok(buf, "|");
    char *b = strtok(NULL, "|");
    char *c = strtok(NULL, "|");
    if (!a || !b || !c) return 0;
    char *end = NULL;
    long n = strtol(a, &end, 10);
    if (end == a || *end != '\0') return 0;
    end = NULL;
    long long bal = strtoll(c, &end, 10);
    if (end == c || *end != '\0') return 0;
    acc->accountNumber = (int)n;
    strncpy(acc->accountHolder, b, sizeof(acc->accountHolder));
    acc->accountHolder[sizeof(acc->accountHolder) - 1] = '\0';
    acc->balanceCents = (cents_t)bal;
    return 1;
}

static void write_account_line(FILE *f, const BankAccount *acc) {
    fprintf(f, "%d|%s|%lld\n", acc->accountNumber, acc->accountHolder, (long long)acc->balanceCents);
}

static void log_txn(const char *type, const char *details, cents_t amount, const char *result) {
    FILE *f = fopen(TRANSACTIONS_FILE, "a");
    if (!f) return;
    char ts[32];
    now_timestamp(ts, sizeof(ts));
    fprintf(f, "%s|%s|%s|%lld|%s\n", ts, type, details, (long long)amount, result ? result : "");
    fclose(f);
}

static int account_exists(int accountNumber) {
    FILE *f = fopen(ACCOUNTS_FILE, "r");
    if (!f) return 0;
    char line[LINE_MAX_LEN];
    BankAccount acc;
    while (fgets(line, sizeof(line), f)) {
        if (parse_account_line(line, &acc) && acc.accountNumber == accountNumber) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

static int load_account(int accountNumber, BankAccount *out) {
    FILE *f = fopen(ACCOUNTS_FILE, "r");
    if (!f) return 0;
    char line[LINE_MAX_LEN];
    BankAccount acc;
    while (fgets(line, sizeof(line), f)) {
        if (parse_account_line(line, &acc) && acc.accountNumber == accountNumber) {
            *out = acc;
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

static int rewrite_accounts_with_update(
    int targetAccount,
    int (*mutator)(BankAccount *acc, void *ctx),
    void *ctx,
    int requireFound
) {
    FILE *in = fopen(ACCOUNTS_FILE, "r");
    if (!in) {
        if (requireFound) return 0;
        in = fopen(ACCOUNTS_FILE, "w+");
        if (!in) return 0;
        fclose(in);
        in = fopen(ACCOUNTS_FILE, "r");
        if (!in) return 0;
    }
    FILE *out = fopen("accounts.tmp", "w");
    if (!out) { fclose(in); return 0; }
    char line[LINE_MAX_LEN];
    BankAccount acc;
    int found = 0;
    while (fgets(line, sizeof(line), in)) {
        if (!parse_account_line(line, &acc)) continue;
        if (acc.accountNumber == targetAccount) {
            found = 1;
            if (mutator) mutator(&acc, ctx);
        }
        write_account_line(out, &acc);
    }
    fclose(in);
    fclose(out);
    if (requireFound && !found) {
        remove("accounts.tmp");
        return 0;
    }
    remove(ACCOUNTS_FILE);
    if (rename("accounts.tmp", ACCOUNTS_FILE) != 0) return 0;
    return found || !requireFound;
}

/* ----------------------------- Operations ----------------------------- */

static void create_account(void) {
    int accountNumber;
    char name[NAME_MAX_LEN];
    if (!read_int("Enter new account number: ", &accountNumber) || accountNumber <= 0) {
        printf("Invalid account number.\n");
        return;
    }
    if (account_exists(accountNumber)) {
        printf("Account number already exists.\n");
        return;
    }
    read_line("Enter account holder full name: ", name, sizeof(name));
    if (name[0] == '\0') {
        printf("Name cannot be empty.\n");
        return;
    }
    BankAccount acc;
    acc.accountNumber = accountNumber;
    strncpy(acc.accountHolder, name, sizeof(acc.accountHolder));
    acc.accountHolder[sizeof(acc.accountHolder) - 1] = '\0';
    acc.balanceCents = 0;
    FILE *f = fopen(ACCOUNTS_FILE, "a");
    if (!f) {
        printf("Error opening accounts file.\n");
        return;
    }
    write_account_line(f, &acc);
    fclose(f);
    log_txn("CREATE", name, 0, "OK");
    printf("Account created successfully.\n");
}

typedef struct { cents_t amount; } MoneyCtx;

static int deposit_mutator(BankAccount *acc, void *ctx) {
    MoneyCtx *m = (MoneyCtx *)ctx;
    acc->balanceCents += m->amount;
    return 1;
}

static void deposit(void) {
    int accountNumber;
    cents_t amount;
    if (!read_int("Enter account number: ", &accountNumber) || accountNumber <= 0) {
        printf("Invalid account number.\n");
        return;
    }
    if (!read_money_cents("Enter amount to deposit (e.g., 1000.50): ", &amount) || amount <= 0) {
        printf("Invalid deposit amount.\n");
        return;
    }
    MoneyCtx ctx = { amount };
    int ok = rewrite_accounts_with_update(accountNumber, deposit_mutator, &ctx, 1);
    if (!ok) {
        printf("Account not found.\n");
        log_txn("DEPOSIT", "Account not found", amount, "FAIL");
        return;
    }
    char amt[32]; format_money(amount, amt, sizeof(amt));
    char details[128];
    snprintf(details, sizeof(details), "Acc:%d", accountNumber);
    log_txn("DEPOSIT", details, amount, "OK");
    printf("Deposit successful. Amount: %s\n", amt);
}

typedef struct { cents_t amount; int success; } WithdrawCtx;

static int withdraw_mutator(BankAccount *acc, void *ctx) {
    WithdrawCtx *w = (WithdrawCtx *)ctx;
    if (acc->balanceCents >= w->amount) {
        acc->balanceCents -= w->amount;
        w->success = 1;
    } else {
        w->success = 0;
    }
    return 1;
}

static void withdraw_money(void) {
    int accountNumber;
    cents_t amount;
    if (!read_int("Enter account number: ", &accountNumber) || accountNumber <= 0) {
        printf("Invalid account number.\n");
        return;
    }
    if (!read_money_cents("Enter amount to withdraw (e.g., 250.00): ", &amount) || amount <= 0) {
        printf("Invalid withdrawal amount.\n");
        return;
    }
    WithdrawCtx ctx = { amount, 0 };
    int found = rewrite_accounts_with_update(accountNumber, withdraw_mutator, &ctx, 1);
    if (!found) {
        printf("Account not found.\n");
        log_txn("WITHDRAW", "Account not found", amount, "FAIL");
        return;
    }
    if (!ctx.success) {
        printf("Insufficient balance.\n");
        char details[128];
        snprintf(details, sizeof(details), "Acc:%d Insufficient", accountNumber);
        log_txn("WITHDRAW", details, amount, "FAIL");
        return;
    }
    char amt[32]; format_money(amount, amt, sizeof(amt));
    char details[128];
    snprintf(details, sizeof(details), "Acc:%d", accountNumber);
    log_txn("WITHDRAW", details, amount, "OK");
    printf("Withdrawal successful. Amount: %s\n", amt);
}

static void display_account(void) {
    int accountNumber;
    if (!read_int("Enter account number: ", &accountNumber) || accountNumber <= 0) {
        printf("Invalid account number.\n");
        return;
    }
    BankAccount acc;
    if (!load_account(accountNumber, &acc)) {
        printf("Account not found.\n");
        return;
    }
    char bal[32];
    format_money(acc.balanceCents, bal, sizeof(bal));
    printf("\n--- Account Details ---\n");
    printf("Account Number : %d\n", acc.accountNumber);
    printf("Holder Name    : %s\n", acc.accountHolder);
    printf("Balance        : %s\n", bal);
}

static void list_all_accounts(void) {
    FILE *f = fopen(ACCOUNTS_FILE, "r");
    if (!f) {
        printf("No accounts found (accounts file missing).\n");
        return;
    }
    printf("\n--- All Accounts ---\n");
    printf("%-12s | %-30s | %-12s\n", "Account No", "Holder", "Balance");
    printf("---------------------------------------------------------------\n");
    char line[LINE_MAX_LEN];
    BankAccount acc;
    int count = 0;
    while (fgets(line, sizeof(line), f)) {
        if (!parse_account_line(line, &acc)) continue;
        char bal[32];
        format_money(acc.balanceCents, bal, sizeof(bal));
        printf("%-12d | %-30.30s | %-12s\n", acc.accountNumber, acc.accountHolder, bal);
        count++;
    }
    fclose(f);
    if (count == 0) printf("(No valid accounts in file.)\n");
}

typedef struct { char newName[NAME_MAX_LEN]; } NameCtx;

static int rename_mutator(BankAccount *acc, void *ctx) {
    NameCtx *n = (NameCtx *)ctx;
    strncpy(acc->accountHolder, n->newName, sizeof(acc->accountHolder));
    acc->accountHolder[sizeof(acc->accountHolder) - 1] = '\0';
    return 1;
}

static void update_holder_name(void) {
    int accountNumber;
    if (!read_int("Enter account number: ", &accountNumber) || accountNumber <= 0) {
        printf("Invalid account number.\n");
        return;
    }
    char name[NAME_MAX_LEN];
    read_line("Enter new full name: ", name, sizeof(name));
    if (name[0] == '\0') {
        printf("Name cannot be empty.\n");
        return;
    }
    NameCtx ctx;
    strncpy(ctx.newName, name, sizeof(ctx.newName));
    ctx.newName[sizeof(ctx.newName) - 1] = '\0';
    int ok = rewrite_accounts_with_update(accountNumber, rename_mutator, &ctx, 1);
    if (!ok) {
        printf("Account not found.\n");
        log_txn("RENAME", "Account not found", 0, "FAIL");
        return;
    }
    char details[160];
    snprintf(details, sizeof(details), "Acc:%d NewName:%s", accountNumber, name);
    log_txn("RENAME", details, 0, "OK");
    printf("Account holder name updated.\n");
}

static void delete_account(void) {
    int accountNumber;
    if (!read_int("Enter account number to delete: ", &accountNumber) || accountNumber <= 0) {
        printf("Invalid account number.\n");
        return;
    }
    FILE *in = fopen(ACCOUNTS_FILE, "r");
    if (!in) {
        printf("Accounts file not found.\n");
        return;
    }
    FILE *out = fopen("accounts.tmp", "w");
    if (!out) { fclose(in); printf("Error preparing temp file.\n"); return; }
    char line[LINE_MAX_LEN];
    BankAccount acc;
    int found = 0;
    while (fgets(line, sizeof(line), in)) {
        if (!parse_account_line(line, &acc)) continue;
        if (acc.accountNumber == accountNumber) {
            found = 1;
            continue;
        }
        write_account_line(out, &acc);
    }
    fclose(in);
    fclose(out);
    if (!found) {
        remove("accounts.tmp");
        printf("Account not found.\n");
        log_txn("DELETE", "Account not found", 0, "FAIL");
        return;
    }
    remove(ACCOUNTS_FILE);
    if (rename("accounts.tmp", ACCOUNTS_FILE) != 0) {
        printf("Error finalizing delete.\n");
        return;
    }
    char details[64];
    snprintf(details, sizeof(details), "Acc:%d", accountNumber);
    log_txn("DELETE", details, 0, "OK");
    printf("Account deleted successfully.\n");
}

static void transfer_money(void) {
    int fromAcc, toAcc;
    cents_t amount;
    if (!read_int("Transfer FROM account number: ", &fromAcc) || fromAcc <= 0) {
        printf("Invalid FROM account.\n");
        return;
    }
    if (!read_int("Transfer TO account number: ", &toAcc) || toAcc <= 0) {
        printf("Invalid TO account.\n");
        return;
    }
    if (fromAcc == toAcc) {
        printf("FROM and TO accounts must be different.\n");
        return;
    }
    if (!read_money_cents("Enter transfer amount: ", &amount) || amount <= 0) {
        printf("Invalid amount.\n");
        return;
    }
    BankAccount from, to;
    if (!load_account(fromAcc, &from)) {
        printf("FROM account not found.\n");
        log_txn("TRANSFER", "FROM not found", amount, "FAIL");
        return;
    }
    if (!load_account(toAcc, &to)) {
        printf("TO account not found.\n");
        log_txn("TRANSFER", "TO not found", amount, "FAIL");
        return;
    }
    if (from.balanceCents < amount) {
        printf("Insufficient balance in FROM account.\n");
        log_txn("TRANSFER", "Insufficient funds", amount, "FAIL");
        return;
    }
    WithdrawCtx wctx = { amount, 0 };
    int ok1 = rewrite_accounts_with_update(fromAcc, withdraw_mutator, &wctx, 1);
    if (!ok1 || !wctx.success) {
        printf("Transfer failed during withdraw.\n");
        log_txn("TRANSFER", "Failed at withdraw step", amount, "FAIL");
        return;
    }
    MoneyCtx dctx = { amount };
    int ok2 = rewrite_accounts_with_update(toAcc, deposit_mutator, &dctx, 1);
    if (!ok2) {
        printf("Transfer partially failed (deposit step). Please check accounts.\n");
        log_txn("TRANSFER", "Deposit step failed after withdraw (manual fix needed)", amount, "FAIL");
        return;
    }
    char amt[32]; format_money(amount, amt, sizeof(amt));
    char details[200];
    snprintf(details, sizeof(details), "From:%d To:%d", fromAcc, toAcc);
    log_txn("TRANSFER", details, amount, "OK");
    printf("Transfer successful. Amount: %s\n", amt);
}

static void view_transactions(void) {
    FILE *f = fopen(TRANSACTIONS_FILE, "r");
    if (!f) {
        printf("No transactions found.\n");
        return;
    }
    printf("\n--- Transaction History ---\n");
    printf("Format: timestamp | type | details | amount | result\n");
    printf("-------------------------------------------------------\n");
    char line[LINE_MAX_LEN];
    int count = 0;
    while (fgets(line, sizeof(line), f)) {
        printf("%s", line);
        count++;
        if (count >= 200) {
            printf("... (showing first 200 lines)\n");
            break;
        }
    }
    fclose(f);
}

static void print_menu(void) {
    printf("\n==================== Bank Account System ====================\n");
    printf("1.  Create Account\n");
    printf("2.  Deposit\n");
    printf("3.  Withdraw\n");
    printf("4.  Transfer\n");
    printf("5.  Display Account Details\n");
    printf("6.  List All Accounts\n");
    printf("7.  Update Account Holder Name\n");
    printf("8.  Delete Account\n");
    printf("9.  View Transaction History\n");
    printf("10. Exit\n");
    printf("=============================================================\n");
}

int main(void) {
    int choice;
    while (1) {
        print_menu();
        if (!read_int("Enter your choice: ", &choice)) {
            printf("Invalid input.\n");
            continue;
        }
        switch (choice) {
            case 1:  create_account();        break;
            case 2:  deposit();                break;
            case 3:  withdraw_money();        break;
            case 4:  transfer_money();        break;
            case 5:  display_account();       break;
            case 6:  list_all_accounts();     break;
            case 7:  update_holder_name();    break;
            case 8:  delete_account();        break;
            case 9:  view_transactions();      break;
            case 10:
                printf("\nThank you for using the Bank Account System.\n");
                printf("Coded by Japhary from Pixellinx (https://pixellinx.co.tz/)\n");
                return 0;
            default:
                printf("Invalid choice.\n");
        }
    }
}
```

</details>

---

<p align="center">
  <sub>Bank Account Management System — Built with precision & structure in C</sub>
</p>
