#include "auth.h"
#include "../../globals/globals.h"
#include "../../utils/utils.h"
#include <iostream>
#include <cstring>

using namespace std;

bool login()
{
    clearScreen();
    printHeader("LOGIN SISTEM POS");

    char username[50], password[50];

    cout << "\nUsername: ";
    cin >> username;

    cout << "Password: ";
    cin >> password;

    for (int i = 0; i < userCount; i++)
    {
        if (strcmp(users[i].username, username) == 0 && users[i].isActive)
        {
            if (verifyPassword(password, users[i].password))
            {
                currentSession.currentUser = users[i];
                currentSession.isLoggedIn = true;

                printSuccess("Login berhasil!");
                cout << "Selamat datang, " << users[i].fullName << endl;
                cout << "Role: " << getRoleName(users[i].role) << endl;

                pause();
                return true;
            }
        }
    }

    printError("Username atau password salah!");
    pause();
    return false;
}

void logout()
{
    if (currentSession.isShiftOpen)
    {
        printWarning("Shift masih terbuka! Tutup shift terlebih dahulu.");
        return;
    }

    currentSession.isLoggedIn = false;
    printSuccess("Logout berhasil!");
    pause();
}

bool isLoggedIn()
{
    return currentSession.isLoggedIn;
}

User *getCurrentUser()
{
    return &currentSession.currentUser;
}

const char *getRoleName(Role role)
{
    switch (role)
    {
    case SUPER_ADMIN:
        return "Super Admin";
    case STORE_MANAGER:
        return "Store Manager";
    case CASHIER:
        return "Cashier";
    case STOCK_CLERK:
        return "Stock Clerk";
    default:
        return "Unknown";
    }
}