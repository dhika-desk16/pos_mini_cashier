#ifndef UTILS_H
#define UTILS_H

#include "../structs.h"

void trim(char *str);
void toUpperCase(char *str);
void toLowerCase(char *str);
bool isEmpty(const char *str);

void getCurrentDateTime(char *buffer);
void getCurrentDate(char *buffer);
void getCurrentTime(char *buffer);
long long getTimestamp();

void generateUserID(char *buffer);
void generateStoreID(char *buffer);
void generateProductID(char *buffer);
void generateCategoryID(char *buffer);
void generateTransactionID(char *buffer);
void generateShiftID(char *buffer);

void hashPassword(const char *password, char *hashed);
bool verifyPassword(const char *password, const char *hashed);

bool isValidPhone(const char *phone);
bool isNumeric(const char *str);

void clearScreen();
void pause();
void printHeader(const char *title);
void printLine(char c, int length);
void printSuccess(const char *message);
void printError(const char *message);
void printWarning(const char *message);
void printInfo(const char *message);

bool hasPermission(Role role, const char *resource, const char *action);

#endif