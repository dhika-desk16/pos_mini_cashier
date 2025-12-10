#include "user.h"
#include "../../globals/globals.h"
#include "../../utils/utils.h"
#include "../../utils/menuInput.h"
#include "../../fileio.h"
#include "../../modules/auth/auth.h"
#include <iostream>
#include <cstring>

using namespace std;

void menuUserManagement()
{
    if (!hasPermission(getCurrentUser()->role, "users", "read"))
    {
        printError("Anda tidak memiliki akses ke menu ini!");
        pause();
        return;
    }

    int choice;

    while (true)
    {
        clearScreen();
        printHeader("USER MANAGEMENT");

        cout << "\n1. Tambah User";
        cout << "\n2. Lihat Semua User";
        cout << "\n3. Update User";
        cout << "\n4. Hapus User";
        cout << "\n5. Detail User";
        cout << "\n0. Kembali";
        int choice = getMenuChoice("\nPilihan: ");
        if (choice == -1) { pause(); continue; }

        switch (choice)
        {
        case 1:
            createUser();
            break;
        case 2:
            listUsers();
            break;
        case 3:
            updateUser();
            break;
        case 4:
            deleteUser();
            break;
        case 5:
            viewUserDetail();
            break;
        case 0:
            return;
        default:
            printError("Pilihan tidak valid!");
            pause();
        }
    }
}

void createUser()
{
    if (!hasPermission(getCurrentUser()->role, "users", "create"))
    {
        printError("Anda tidak memiliki akses untuk membuat user!");
        pause();
        return;
    }

    clearScreen();
    printHeader("TAMBAH USER BARU");
    displayUserTable();

    if (userCount >= MAX_USERS)
    {
        printError("Database user penuh!");
        pause();
        return;
    }

    User newUser;

    generateUserID(newUser.userID);

    cout << "\nUsername: ";
    cin.getline(newUser.username, 50);

    if (findUserByUsername(newUser.username) != nullptr)
    {
        printError("Username sudah digunakan!");
        pause();
        return;
    }

    char password[50];
    cout << "Password: ";
    cin.getline(password, 50);
    hashPassword(password, newUser.password);

    cout << "Nama Lengkap: ";
    cin.getline(newUser.fullName, 100);

    cout << "\nPilih Role:";
    cout << "\n1. Super Admin";
    cout << "\n2. Store Manager";
    cout << "\n3. Cashier";
    cout << "\n4. Stock Clerk";
    int roleChoice = getMenuChoice("\nPilihan: ");
        if (roleChoice == -1) { pause(); return; }

    switch (roleChoice)
    {
    case 1:
        newUser.role = SUPER_ADMIN;
        break;
    case 2:
        newUser.role = STORE_MANAGER;
        break;
    case 3:
        newUser.role = CASHIER;
        break;
    case 4:
        newUser.role = STOCK_CLERK;
        break;
    default:
        printError("Role tidak valid!");
        pause();
        return;
    }

    if (newUser.role == SUPER_ADMIN)
    {
        strcpy(newUser.storeID, "ALL");
    }
    else
    {
        cout << "\nStore ID (atau ketik 'list' untuk melihat): ";
        char input[20];
        cin.getline(input, 20);

        if (strcmp(input, "list") == 0)
        {
            cout << "\nDaftar Store:\n";
            for (int i = 0; i < storeCount; i++)
            {
                cout << stores[i].storeID << " - " << stores[i].storeName << "\n";
            }
            cout << "\nStore ID: ";
            cin.getline(newUser.storeID, 20);
        }
        else
        {
            strcpy(newUser.storeID, input);
        }
    }

    newUser.isActive = true;
    getCurrentDateTime(newUser.createdAt);

    users[userCount++] = newUser;
    saveUsers();

    printSuccess("User berhasil ditambahkan!");
    cout << "User ID: " << newUser.userID << endl;
    pause();
}

void listUsers()
{
    clearScreen();
    printHeader("DAFTAR USER");

    if (userCount == 0)
    {
        printInfo("Belum ada user.");
        pause();
        return;
    }

    displayUserTable();
    pause();
}

void updateUser()
{
    if (!hasPermission(getCurrentUser()->role, "users", "update"))
    {
        printError("Anda tidak memiliki akses untuk update user!");
        pause();
        return;
    }

    clearScreen();
    printHeader("UPDATE USER");

    displayUserTable();

    char userID[20];
    cout << "\nUser ID: ";
    cin.getline(userID, 20);

    User *user = findUserByID(userID);

    if (user == nullptr)
    {
        printError("User tidak ditemukan!");
        pause();
        return;
    }

    cout << "\n=== Data Saat Ini ===";
    cout << "\nNama: " << user->fullName;
    cout << "\nRole: " << getRoleName(user->role);
    cout << "\nStore: " << user->storeID;
    cout << "\nStatus: " << (user->isActive ? "Active" : "Inactive");

    cout << "\n\n=== Update Data ===";
    cout << "\n1. Nama Lengkap";
    cout << "\n2. Role";
    cout << "\n3. Store";
    cout << "\n4. Reset Password";
    cout << "\n5. Toggle Status (Active/Inactive)";
    cout << "\n0. Batal";
    int choice = getMenuChoice("\nPilihan: ");
    if (choice == -1) { pause(); return; }

    switch (choice)
    {
    case 1:
        cout << "Nama Lengkap Baru: ";
        cin.getline(user->fullName, 100);
        printSuccess("Nama berhasil diupdate!");
        break;

    case 2:
        cout << "\n1. Super Admin";
        cout << "\n2. Store Manager";
        cout << "\n3. Cashier";
        cout << "\n4. Stock Clerk";
        cout << "\nPilihan: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            user->role = SUPER_ADMIN;
            break;
        case 2:
            user->role = STORE_MANAGER;
            break;
        case 3:
            user->role = CASHIER;
            break;
        case 4:
            user->role = STOCK_CLERK;
            break;
        }
        printSuccess("Role berhasil diupdate!");
        break;

    case 3:
        cout << "Store ID Baru: ";
        cin.getline(user->storeID, 20);
        printSuccess("Store berhasil diupdate!");
        break;

    case 4:
    {
        char newPassword[50];
        cout << "Password Baru: ";
        cin.getline(newPassword, 50);
        hashPassword(newPassword, user->password);
        printSuccess("Password berhasil direset!");
        break;
    }

    case 5:
        user->isActive = !user->isActive;
        printSuccess(user->isActive ? "User diaktifkan!" : "User dinonaktifkan!");
        break;

    case 0:
        return;
    }

    saveUsers();
    pause();
}

void deleteUser()
{
    if (!hasPermission(getCurrentUser()->role, "users", "delete"))
    {
        printError("Anda tidak memiliki akses untuk hapus user!");
        pause();
        return;
    }

    clearScreen();
    printHeader("HAPUS USER");

    displayUserTable();

    char userID[20];
    cout << "\nUser ID: ";
    cin.getline(userID, 20);

    int index = -1;
    for (int i = 0; i < userCount; i++)
    {
        if (strcmp(users[i].userID, userID) == 0)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        printError("User tidak ditemukan!");
        pause();
        return;
    }

    if (strcmp(users[index].userID, getCurrentUser()->userID) == 0)
    {
        printError("Tidak dapat menghapus akun sendiri!");
        pause();
        return;
    }

    cout << "\nYakin ingin menghapus user: " << users[index].fullName << "? (y/n): ";
    char confirm;
    cin >> confirm;

    if (confirm == 'y' || confirm == 'Y')
    {

        for (int i = index; i < userCount - 1; i++)
        {
            users[i] = users[i + 1];
        }
        userCount--;

        saveUsers();
        printSuccess("User berhasil dihapus!");
    }
    else
    {
        printInfo("Penghapusan dibatalkan.");
    }

    pause();
}

void viewUserDetail()
{
    clearScreen();
    printHeader("DETAIL USER");

    listUsers();

    char userID[20];
    cout << "\nUser ID: ";
    cin.getline(userID, 20);

    User *user = findUserByID(userID);

    if (user == nullptr)
    {
        printError("User tidak ditemukan!");
        pause();
        return;
    }

    cout << "\n=== INFORMASI USER ===";
    cout << "\nUser ID      : " << user->userID;
    cout << "\nUsername     : " << user->username;
    cout << "\nNama Lengkap : " << user->fullName;
    cout << "\nRole         : " << getRoleName(user->role);
    cout << "\nStore ID     : " << user->storeID;
    cout << "\nStatus       : " << (user->isActive ? "Active" : "Inactive");
    cout << "\nDibuat       : " << user->createdAt;

    pause();
}

User *findUserByID(const char *userID)
{
    for (int i = 0; i < userCount; i++)
    {
        if (strcmp(users[i].userID, userID) == 0)
        {
            return &users[i];
        }
    }
    return nullptr;
}

User *findUserByUsername(const char *username)
{
    for (int i = 0; i < userCount; i++)
    {
        if (strcmp(users[i].username, username) == 0)
        {
            return &users[i];
        }
    }
    return nullptr;
}

void displayUserTable()
{
    printLine('-', 80);
    cout << "ID      Username        Nama                 Role              Status\n";
    printLine('-', 80);

    for (int i = 0; i < userCount; i++)
    {
        printf("%-8s %-15s %-20s %-17s %s\n",
               users[i].userID,
               users[i].username,
               users[i].fullName,
               getRoleName(users[i].role),
               users[i].isActive ? "Active" : "Inactive");
    }

    printLine('-', 80);
    cout << "Total: " << userCount << " users\n";
}