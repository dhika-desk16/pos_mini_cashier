#include "fileio.h"
#include "globals.h"
#include "utils.h"
#include <iostream>
#include <cstdio>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstring> 

using namespace std;

void initializeDataFolder() {
    #ifdef _WIN32
        mkdir("data");
    #else
        mkdir("data", 0777);
    #endif
}

void createDefaultData() {
    User admin;
    strcpy(admin.userID, "US001");
    strcpy(admin.username, "admin");
    hashPassword("admin123", admin.password);
    strcpy(admin.fullName, "Super Administrator");
    admin.role = SUPER_ADMIN;
    strcpy(admin.storeID, "ALL");
    admin.isActive = true;
    getCurrentDateTime(admin.createdAt);
    
    users[userCount++] = admin;
    saveUsers();
    
    printSuccess("Default admin (username: admin, password: admin123)");
}

void loadAllData() {
    loadUsers();
    loadStores();
    loadCategories();
    loadProducts();
    loadTransactions();
}

void loadUsers() {
    FILE* file = fopen(FILE_USERS, "rb");
    if (file) {
        fread(&userCount, sizeof(int), 1, file);
        fread(users, sizeof(User), userCount, file);
        fclose(file);
    }
}

void loadStores() {
    FILE* file = fopen(FILE_STORES, "rb");
    if (file) {
        fread(&storeCount, sizeof(int), 1, file);
        fread(stores, sizeof(Store), storeCount, file);
        fclose(file);
    }
}

void loadCategories() {
    FILE* file = fopen(FILE_CATEGORIES, "rb");
    if (file) {
        fread(&categoryCount, sizeof(int), 1, file);
        fread(categories, sizeof(Category), categoryCount, file);
        fclose(file);
    }
}

void loadProducts() {
    FILE* file = fopen(FILE_PRODUCTS, "rb");
    if (file) {
        fread(&productCount, sizeof(int), 1, file);
        fread(products, sizeof(Product), productCount, file);
        fclose(file);
    }
}

void loadTransactions() {
    FILE* file = fopen(FILE_TRANSACTIONS, "rb");
    if (file) {
        fread(&transactionCount, sizeof(int), 1, file);
        fread(transactions, sizeof(Transaction), transactionCount, file);
        fclose(file);
    }
}

void saveUsers() {
    FILE* file = fopen(FILE_USERS, "wb");
    if (file) {
        fwrite(&userCount, sizeof(int), 1, file);
        fwrite(users, sizeof(User), userCount, file);
        fclose(file);
    }
}

void saveStores() {
    FILE* file = fopen(FILE_STORES, "wb");
    if (file) {
        fwrite(&storeCount, sizeof(int), 1, file);
        fwrite(stores, sizeof(Store), storeCount, file);
        fclose(file);
    }
}

void saveCategories() {
    FILE* file = fopen(FILE_CATEGORIES, "wb");
    if (file) {
        fwrite(&categoryCount, sizeof(int), 1, file);
        fwrite(categories, sizeof(Category), categoryCount, file);
        fclose(file);
    }
}

void saveProducts() {
    FILE* file = fopen(FILE_PRODUCTS, "wb");
    if (file) {
        fwrite(&productCount, sizeof(int), 1, file);
        fwrite(products, sizeof(Product), productCount, file);
        fclose(file);
    }
}

void saveTransactions() {
    FILE* file = fopen(FILE_TRANSACTIONS, "wb");
    if (file) {
        fwrite(&transactionCount, sizeof(int), 1, file);
        fwrite(transactions, sizeof(Transaction), transactionCount, file);
        fclose(file);
    }
}

void saveTransactionDetails(const char* transactionID) {
    FILE* file = fopen(FILE_TRANSACTION_DETAILS, "a");
    if (file) {
        fprintf(file, "=== TRANSACTION: %s ===\n", transactionID);
        
        TransactionDetail* current = transactionDetailHead;
        while (current != nullptr) {
            fprintf(file, "%s|%s|%d|%.2f|%.2f\n",
                current->productID,
                current->productName,
                current->quantity,
                current->price,
                current->subtotal);
            current = current->next;
        }
        
        fprintf(file, "\n");
        fclose(file);
    }
}

void saveShiftLog(Shift* shift) {
    FILE* file = fopen(FILE_SHIFTS, "a");
    if (file) {
        fprintf(file, "=====================================\n");
        fprintf(file, "Shift ID: %s\n", shift->shiftID);
        fprintf(file, "Cashier: %s\n", shift->cashierID);
        fprintf(file, "Store: %s\n", shift->storeID);
        fprintf(file, "Open: %s\n", shift->openTime);
        fprintf(file, "Close: %s\n", shift->closeTime);
        fprintf(file, "Opening Balance: Rp %.2f\n", shift->openingBalance);
        fprintf(file, "Closing Balance: Rp %.2f\n", shift->closingBalance);
        fprintf(file, "Total Sales: Rp %.2f\n", shift->totalSales);
        fprintf(file, "Total Transactions: %d\n", shift->totalTransactions);
        fprintf(file, "=====================================\n\n");
        fclose(file);
    }
}