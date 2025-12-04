#include "globals.h"

User users[MAX_USERS];
int userCount = 0;

Store stores[MAX_STORES];
int storeCount = 0;

Category categories[MAX_CATEGORIES];
int categoryCount = 0;

Product products[MAX_PRODUCTS];
int productCount = 0;

Transaction transactions[MAX_TRANSACTIONS];
int transactionCount = 0;

CartItem* cartHead = nullptr;
TransactionDetail* transactionDetailHead = nullptr;

Session currentSession = {
    .isLoggedIn = false,
    .isShiftOpen = false
};