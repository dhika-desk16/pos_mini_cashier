#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "../../structs.h"

void menuCashier();

void scanProduct();
void viewCart();
void removeFromCart();
void clearCart();

void processCheckout();
void printReceipt(Transaction *transaction);

void addToCart(Product *product, int quantity);
double calculateCartTotal();
int countCartItems();

Transaction *findTransactionByID(const char *transactionID);
void viewTransactionHistory();
void viewTransactionDetail();

#endif