#ifndef FILEIO_H
#define FILEIO_H

#include "structs.h"

void loadAllData();
void loadUsers();
void loadStores();
void loadCategories();
void loadProducts();
void loadTransactions();

void saveUsers();
void saveStores();
void saveCategories();
void saveProducts();
void saveTransactions();

void saveTransactionDetails(const char* transactionID);
void saveShiftLog(Shift* shift);

void initializeDataFolder();
void createDefaultData();

#endif