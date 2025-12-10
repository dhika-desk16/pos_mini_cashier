#ifndef GLOBALS_H
#define GLOBALS_H

#include "../structs.h"

const int MAX_USERS = 100;
const int MAX_STORES = 50;
const int MAX_PRODUCTS = 1000;
const int MAX_CATEGORIES = 50;
const int MAX_TRANSACTIONS = 10000;

extern User users[MAX_USERS];
extern int userCount;

extern Store stores[MAX_STORES];
extern int storeCount;

extern Category categories[MAX_CATEGORIES];
extern int categoryCount;

extern Product products[MAX_PRODUCTS];
extern int productCount;

extern Transaction transactions[MAX_TRANSACTIONS];
extern int transactionCount;

extern CartItem *cartHead;
extern TransactionDetail *transactionDetailHead;

extern Session currentSession;

const char FILE_USERS[] = "data/users.dat";
const char FILE_STORES[] = "data/stores.dat";
const char FILE_CATEGORIES[] = "data/categories.dat";
const char FILE_PRODUCTS[] = "data/products.dat";
const char FILE_TRANSACTIONS[] = "data/transactions.dat";
const char FILE_TRANSACTION_DETAILS[] = "data/transaction_details.txt";
const char FILE_SHIFTS[] = "data/shifts.txt";

#endif