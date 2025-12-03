#ifndef STRUCTS_H
#define STRUCTS_H

#include <ctime>

enum Role
{
    SUPER_ADMIN = 1,
    STORE_MANAGER = 2,
    CASHIER = 3,
    STOCK_CLERK = 4
};

struct User
{
    char userID[20];
    char username[50];
    char password[100];
    char fullName[100];
    Role role;
    char storeID[20];
    bool isActive;
    char createdAt[50];
};

struct Store
{
    char storeID[20];
    char storeName[100];
    char address[200];
    char phone[15];
    bool isActive;
    char createdAt[50];
};

struct Category
{
    char categoryID[20];
    char categoryName[50];
};

struct Product
{
    char productID[20];
    char barcode[50];
    char productName[100];
    char categoryID[20];
    int stock;
    int minStock;
    double purchasePrice;
    double sellingPrice;
    char storeID[20];
    bool isActive;
};

struct CartItem
{
    Product *product;
    int quantity;
    double subtotal;
    CartItem *next;
};

struct Shift
{
    char shiftID[30];
    char cashierID[20];
    char storeID[20];
    char openTime[50];
    char closeTime[50];
    double openingBalance;
    double closingBalance;
    double totalSales;
    int totalTransactions;
    bool isOpen;
};

struct Transaction
{
    char transactionID[30];
    char shiftID[30];
    char cashierID[20];
    char storeID[20];
    char transactionDate[50];
    double subtotal;
    double discount;
    double tax;
    double totalAmount;
    double paidAmount;
    double changeAmount;
    int itemCount;
    char paymentMethod[20];
};

struct TransactionDetail
{
    char transactionID[30];
    char productID[20];
    char productName[100];
    int quantity;
    double price;
    double subtotal;
    TransactionDetail *next;
};

struct Session
{
    User currentUser;
    Shift currentShift;
    bool isLoggedIn;
    bool isShiftOpen;
};

#endif