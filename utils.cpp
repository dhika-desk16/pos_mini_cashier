#include "utils.h"
#include "globals.h"
#include <iostream>
#include <cstring>
#include <ctime>
#include <cctype>

using namespace std;

void trim(char* str) {
    int len = strlen(str);
    int start = 0, end = len - 1;
    
    while (start < len && isspace(str[start])) start++;
    while (end >= 0 && isspace(str[end])) end--;
    
    int j = 0;
    for (int i = start; i <= end; i++) {
        str[j++] = str[i];
    }
    str[j] = '\0';
}

void toUpperCase(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper(str[i]);
    }
}

void toLowerCase(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

bool isEmpty(const char* str) {
    return str == nullptr || strlen(str) == 0;
}

void getCurrentDateTime(char* buffer) {
    time_t now = time(0);
    struct tm* timeinfo = localtime(&now);
    strftime(buffer, 50, "%Y-%m-%d %H:%M:%S", timeinfo);
}

void getCurrentDate(char* buffer) {
    time_t now = time(0);
    struct tm* timeinfo = localtime(&now);
    strftime(buffer, 50, "%Y-%m-%d", timeinfo);
}

void getCurrentTime(char* buffer) {
    time_t now = time(0);
    struct tm* timeinfo = localtime(&now);
    strftime(buffer, 50, "%H:%M:%S", timeinfo);
}

long long getTimestamp() {
    return (long long)time(0);
}

void generateUserID(char* buffer) {
    sprintf(buffer, "US%03d", userCount + 1);
}

void generateStoreID(char* buffer) {
    sprintf(buffer, "ST%03d", storeCount + 1);
}

void generateProductID(char* buffer) {
    sprintf(buffer, "PRD%04d", productCount + 1);
}

void generateCategoryID(char* buffer) {
    sprintf(buffer, "CAT%03d", categoryCount + 1);
}

void generateTransactionID(char* buffer) {
    sprintf(buffer, "TRX%lld", getTimestamp());
}

void generateShiftID(char* buffer) {
    sprintf(buffer, "SHF%lld", getTimestamp());
}

void hashPassword(const char* password, char* hashed) {
    int len = strlen(password);
    for (int i = 0; i < len; i++) {
        hashed[i] = password[i] ^ 0xAA;
    }
    hashed[len] = '\0';
}

bool verifyPassword(const char* password, const char* hashed) {
    char temp[100];
    hashPassword(password, temp);
    return strcmp(temp, hashed) == 0;
}

bool isValidPhone(const char* phone) {
    int len = strlen(phone);
    if (len < 10 || len > 15) return false;
    
    for (int i = 0; i < len; i++) {
        if (!isdigit(phone[i]) && phone[i] != '+' && phone[i] != '-') {
            return false;
        }
    }
    return true;
}

bool isNumeric(const char* str) {
    for (int i = 0; str[i]; i++) {
        if (!isdigit(str[i])) return false;
    }
    return true;
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pause() {
    cout << "\nTekan Enter untuk melanjutkan...";
    cin.ignore();
    cin.get();
}

void printHeader(const char* title) {
    printLine('=', 60);
    cout << "  " << title << endl;
    printLine('=', 60);
}

void printLine(char c, int length) {
    for (int i = 0; i < length; i++) {
        cout << c;
    }
    cout << endl;
}

void printSuccess(const char* message) {
    cout << "\n✓ " << message << endl;
}

void printError(const char* message) {
    cout << "\n✗ ERROR: " << message << endl;
}

void printWarning(const char* message) {
    cout << "\n⚠ WARNING: " << message << endl;
}

void printInfo(const char* message) {
    cout << "\nℹ INFO: " << message << endl;
}

bool hasPermission(Role role, const char* resource, const char* action) {
    if (role == SUPER_ADMIN) {
        if (strcmp(resource, "users") == 0 || strcmp(resource, "stores") == 0) {
            return true;
        }
        if (strcmp(resource, "products") == 0 || strcmp(resource, "transactions") == 0) {
            return strcmp(action, "read") == 0;
        }
    }
    
    if (role == STORE_MANAGER) {
        if (strcmp(resource, "users") == 0 || strcmp(resource, "stores") == 0) {
            return strcmp(action, "read") == 0;
        }
        if (strcmp(resource, "products") == 0) {
            return true;
        }
        if (strcmp(resource, "transactions") == 0) {
            return strcmp(action, "create") != 0;
        }
    }
    
    if (role == CASHIER) {
        if (strcmp(resource, "transactions") == 0) {
            return strcmp(action, "create") == 0 || strcmp(action, "read") == 0;
        }
        return strcmp(action, "read") == 0;
    }
    
    if (role == STOCK_CLERK) {
        if (strcmp(resource, "products") == 0) {
            return strcmp(action, "delete") != 0;
        }
        return strcmp(action, "read") == 0;
    }
    
    return false;
}