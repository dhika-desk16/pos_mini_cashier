#include "fileio.h"
#include "./globals/globals.h"
#include "./utils/utils.h"
#include <iostream>
#include <cstdio>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstring>

using namespace std;

void initializeDataFolder()
{
#ifdef _WIN32
    mkdir("data");
#else
    mkdir("data", 0777);
#endif
}

void createDefaultData()
{
    if (userCount == 0)
    {
        User admin{};
        strcpy(admin.userID, "US001");
        strcpy(admin.username, "admin");
        hashPassword("admin123", admin.password);
        strcpy(admin.fullName, "Super Administrator");
        admin.role = SUPER_ADMIN;
        strcpy(admin.storeID, "ST001");
        admin.isActive = true;
        getCurrentDateTime(admin.createdAt);

        users[userCount++] = admin;
        saveUsers();
    }

    if (storeCount == 0)
    {
        Store store{};
        strcpy(store.storeID, "ST001");
        strcpy(store.storeName, "Main Store");
        strcpy(store.address, "Default Address");
        strcpy(store.phone, "0000000000");
        store.isActive = true;
        getCurrentDateTime(store.createdAt);

        stores[storeCount++] = store;
        saveStores();
    }

    if (categoryCount == 0)
    {
        Category cat1{};
        strcpy(cat1.categoryID, "CT001");
        strcpy(cat1.categoryName, "Food");

        Category cat2{};
        strcpy(cat2.categoryID, "CT002");
        strcpy(cat2.categoryName, "Drink");

        categories[categoryCount++] = cat1;
        categories[categoryCount++] = cat2;
        saveCategories();
    }
    if (productCount == 0)
    {
        Product items[] = {
            {"PR001", "899999900001", "Aqua 600ml", "CT002", 120, 20, 2500, 3500, "ST001", true},
            {"PR002", "899999900002", "Aqua 1500ml", "CT002", 80, 15, 5000, 6500, "ST001", true},
            {"PR003", "899999900003", "Le Minerale 600ml", "CT002", 100, 20, 2400, 3300, "ST001", true},
            {"PR004", "899999900004", "Teh Pucuk 350ml", "CT002", 90, 15, 2800, 4000, "ST001", true},
            {"PR005", "899999900005", "Coca Cola 390ml", "CT002", 70, 15, 3500, 5500, "ST001", true},

            {"PR006", "899999900006", "Indomie Goreng", "CT001", 200, 30, 2500, 3500, "ST001", true},
            {"PR007", "899999900007", "Indomie Soto", "CT001", 180, 30, 2500, 3500, "ST001", true},
            {"PR008", "899999900008", "Mie Sedaap Goreng", "CT001", 170, 30, 2400, 3400, "ST001", true},
            {"PR009", "899999900009", "Chitato Sapi Panggang", "CT001", 90, 20, 8500, 11500, "ST001", true},
            {"PR010", "899999900010", "Qtela Singkong", "CT001", 85, 20, 8000, 11000, "ST001", true},

            {"PR011", "899999900011", "Beras Ramos 5kg", "CT001", 40, 10, 58000, 65000, "ST001", true},
            {"PR012", "899999900012", "Minyak Goreng Bimoli 1L", "CT001", 60, 15, 14000, 16500, "ST001", true},
            {"PR013", "899999900013", "Gula Pasir 1kg", "CT001", 70, 15, 13000, 15000, "ST001", true},
            {"PR014", "899999900014", "Telur Ayam 1kg", "CT001", 50, 10, 23000, 27000, "ST001", true},

            {"PR015", "899999900015", "Pepsodent 190g", "CT001", 75, 15, 11000, 14500, "ST001", true},
            {"PR016", "899999900016", "Lifebuoy Sabun", "CT001", 90, 20, 3500, 5000, "ST001", true},
            {"PR017", "899999900017", "Sunlight 780ml", "CT001", 60, 15, 9500, 12500, "ST001", true},
            {"PR018", "899999900018", "Dettol Cair", "CT001", 50, 10, 18000, 22000, "ST001", true},

            {"PR019", "899999900019", "Sampoerna Mild 16", "CT001", 40, 10, 28000, 32000, "ST001", true},
            {"PR020", "899999900020", "Djarum Super 12", "CT001", 45, 10, 24000, 28000, "ST001", true}};

        int totalItems = sizeof(items) / sizeof(Product);

        for (int i = 0; i < totalItems; i++)
        {
            products[productCount++] = items[i];
        }

        saveProducts();
    }

    saveTransactions();

    printSuccess("All default data initialized.");
}

void loadAllData()
{
    loadUsers();
    loadStores();
    loadCategories();
    loadProducts();
    loadTransactions();
}

void loadUsers()
{
    FILE *file = fopen(FILE_USERS, "rb");
    if (file)
    {
        fread(&userCount, sizeof(int), 1, file);
        fread(users, sizeof(User), userCount, file);
        fclose(file);
    }
}

void loadStores()
{
    FILE *file = fopen(FILE_STORES, "rb");
    if (file)
    {
        fread(&storeCount, sizeof(int), 1, file);
        fread(stores, sizeof(Store), storeCount, file);
        fclose(file);
    }
}

void loadCategories()
{
    FILE *file = fopen(FILE_CATEGORIES, "rb");
    if (file)
    {
        fread(&categoryCount, sizeof(int), 1, file);
        fread(categories, sizeof(Category), categoryCount, file);
        fclose(file);
    }
}

void loadProducts()
{
    FILE *file = fopen(FILE_PRODUCTS, "rb");
    if (file)
    {
        fread(&productCount, sizeof(int), 1, file);
        fread(products, sizeof(Product), productCount, file);
        fclose(file);
    }
}

void loadTransactions()
{
    FILE *file = fopen(FILE_TRANSACTIONS, "rb");
    if (file)
    {
        fread(&transactionCount, sizeof(int), 1, file);
        fread(transactions, sizeof(Transaction), transactionCount, file);
        fclose(file);
    }
}

void saveUsers()
{
    FILE *file = fopen(FILE_USERS, "wb");
    if (file)
    {
        fwrite(&userCount, sizeof(int), 1, file);
        fwrite(users, sizeof(User), userCount, file);
        fclose(file);
    }
}

void saveStores()
{
    FILE *file = fopen(FILE_STORES, "wb");
    if (file)
    {
        fwrite(&storeCount, sizeof(int), 1, file);
        fwrite(stores, sizeof(Store), storeCount, file);
        fclose(file);
    }
}

void saveCategories()
{
    FILE *file = fopen(FILE_CATEGORIES, "wb");
    if (file)
    {
        fwrite(&categoryCount, sizeof(int), 1, file);
        fwrite(categories, sizeof(Category), categoryCount, file);
        fclose(file);
    }
}

void saveProducts()
{
    FILE *file = fopen(FILE_PRODUCTS, "wb");
    if (file)
    {
        fwrite(&productCount, sizeof(int), 1, file);
        fwrite(products, sizeof(Product), productCount, file);
        fclose(file);
    }
}

void saveTransactions()
{
    FILE *file = fopen(FILE_TRANSACTIONS, "wb");
    if (file)
    {
        fwrite(&transactionCount, sizeof(int), 1, file);
        fwrite(transactions, sizeof(Transaction), transactionCount, file);
        fclose(file);
    }
}

void saveTransactionDetails(const char *transactionID)
{
    FILE *file = fopen(FILE_TRANSACTION_DETAILS, "a");
    if (file)
    {
        fprintf(file, "=== TRANSACTION: %s ===\n", transactionID);

        TransactionDetail *current = transactionDetailHead;
        while (current != nullptr)
        {
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

void saveShiftLog(Shift *shift)
{
    FILE *file = fopen(FILE_SHIFTS, "a");
    if (file)
    {
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