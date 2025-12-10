#include <iostream>
#include <cstring>

#include "structs.h"
#include "./globals/globals.h"
#include "./utils/utils.h"
#include "fileio.h"
#include "./modules/auth/auth.h"
#include "./modules/user/user.h"
#include "./modules/store/store.h"
#include "./modules/product/product.h"
#include "./modules/shift/shift.h"
#include "./modules/transaction/transaction.h"
#include "./modules/report/report.h"

using namespace std;

void showMainMenu();
void showSuperAdminMenu();

void showStoreManagerMenu();
void showCashierMenu();
void showStockClerkMenu();

int main()
{

    initializeDataFolder();
    loadAllData();

    if (userCount == 0)
    {
        clearScreen();
        printHeader("SETUP AWAL SISTEM");
        cout << "\nSistem baru pertama kali dijalankan.\n";
        cout << "Membuat akun admin default...\n\n";
        createDefaultData();
        pause();
    }

    while (true)
    {
        if (!isLoggedIn())
        {
            clearScreen();
            printHeader("SISTEM POS MINI MARKET");
            cout << "\n1. Login";
            cout << "\n0. Keluar";
            cout << "\n\nPilihan: ";

            int choice;
            cin >> choice;
            cin.ignore();

            if (choice == 1)
            {
                if (login())
                {
                    continue;
                }
            }
            else if (choice == 0)
            {
                clearScreen();
                printHeader("TERIMA KASIH");
                cout << "\nTerima kasih telah menggunakan Sistem POS!\n";
                break;
            }
        }
        else
        {
            showMainMenu();
        }
    }

    return 0;
}

void showMainMenu()
{
    User *currentUser = getCurrentUser();

    switch (currentUser->role)
    {
    case SUPER_ADMIN:
        showSuperAdminMenu();
        break;
    case STORE_MANAGER:
        showStoreManagerMenu();
        break;
    case CASHIER:
        showCashierMenu();
        break;
    case STOCK_CLERK:
        showStockClerkMenu();
        break;
    default:
        printError("Role tidak dikenal!");
        logout();
    }
}

void showSuperAdminMenu()
{
    int choice;

    clearScreen();
    printHeader("MENU SUPER ADMIN");

    cout << "\nSelamat datang, " << getCurrentUser()->fullName;
    cout << "\nRole: Super Admin\n";

    cout << "\n=== MANAGEMENT ===";
    cout << "\n1. User Management";
    cout << "\n2. Store Management";

    cout << "\n\n=== VIEW ONLY ===";
    cout << "\n3. Lihat Produk";
    cout << "\n4. Lihat Transaksi";
    cout << "\n5. Laporan";

    cout << "\n\n=== SYSTEM ===";
    cout << "\n0. Logout";

    cout << "\n\nPilihan: ";
    cin >> choice;
    cin.ignore();

    switch (choice)
    {
    case 1:
        menuUserManagement();
        break;
    case 2:
        menuStoreManagement();
        break;
    case 3:
        listProducts();
        break;
    case 4:
        viewTransactionHistory();
        break;
    case 5:
        menuReporting();
        break;
    case 0:
        logout();
        break;
    default:
        printError("Pilihan tidak valid!");
        pause();
    }
}

void showStoreManagerMenu()
{
    int choice;

    clearScreen();
    printHeader("MENU STORE MANAGER");

    cout << "\nSelamat datang, " << getCurrentUser()->fullName;
    cout << "\nRole: Store Manager";
    cout << "\nStore: " << getCurrentUser()->storeID << "\n";

    cout << "\n=== MANAGEMENT ===";
    cout << "\n1. Product Management";
    cout << "\n2. Lihat User";
    cout << "\n3. Lihat Store";

    cout << "\n\n=== TRANSACTIONS ===";
    cout << "\n4. Lihat Transaksi";
    cout << "\n5. Detail Transaksi";

    cout << "\n\n=== REPORTS ===";
    cout << "\n6. Laporan";

    cout << "\n\n=== SYSTEM ===";
    cout << "\n0. Logout";

    cout << "\n\nPilihan: ";
    cin >> choice;
    cin.ignore();

    switch (choice)
    {
    case 1:
        menuProductManagement();
        break;
    case 2:
        listUsers();
        break;
    case 3:
        listStores();
        break;
    case 4:
        viewTransactionHistory();
        break;
    case 5:
        viewTransactionDetail();
        break;
    case 6:
        menuReporting();
        break;
    case 0:
        logout();
        break;
    default:
        printError("Pilihan tidak valid!");
        pause();
    }
}

void showCashierMenu()
{
    int choice;

    clearScreen();
    printHeader("MENU CASHIER");

    cout << "\nSelamat datang, " << getCurrentUser()->fullName;
    cout << "\nRole: Cashier";
    cout << "\nStore: " << getCurrentUser()->storeID << "\n";

    if (currentSession.isShiftOpen)
    {
        cout << "\n[SHIFT AKTIF: " << currentSession.currentShift.shiftID << "]";
        cout << "\nTotal Transaksi: " << currentSession.currentShift.totalTransactions;
        cout << "\nTotal Penjualan: Rp" << currentSession.currentShift.totalSales;
    }
    else
    {
        cout << "\n[TIDAK ADA SHIFT AKTIF]";
    }

    cout << "\n\n=== SHIFT ===";
    cout << "\n1. Shift Management";

    cout << "\n\n=== TRANSAKSI ===";
    cout << "\n2. Kasir (Transaksi)";
    cout << "\n3. Riwayat Transaksi";

    cout << "\n\n=== VIEW ===";
    cout << "\n4. Lihat Produk";
    cout << "\n5. Lihat User";

    cout << "\n\n=== SYSTEM ===";
    cout << "\n0. Logout";

    cout << "\n\nPilihan: ";
    cin >> choice;
    cin.ignore();

    switch (choice)
    {
    case 1:
        menuShiftManagement();
        break;
    case 2:
        menuCashier();
        break;
    case 3:
        viewTransactionHistory();
        break;
    case 4:
        listProducts();
        break;
    case 5:
        listUsers();
        break;
    case 0:
        logout();
        break;
    default:
        printError("Pilihan tidak valid!");
        pause();
    }
}

void showStockClerkMenu()
{
    int choice;

    clearScreen();
    printHeader("MENU STOCK CLERK");

    cout << "\nSelamat datang, " << getCurrentUser()->fullName;
    cout << "\nRole: Stock Clerk";
    cout << "\nStore: " << getCurrentUser()->storeID << "\n";

    cout << "\n=== INVENTORY ===";
    cout << "\n1. Product Management";
    cout << "\n2. Update Stok";
    cout << "\n3. Cek Stok Menipis";

    cout << "\n\n=== REPORTS ===";
    cout << "\n4. Laporan Stok";

    cout << "\n\n=== VIEW ===";
    cout << "\n5. Lihat User";
    cout << "\n6. Lihat Transaksi";

    cout << "\n\n=== SYSTEM ===";
    cout << "\n0. Logout";

    cout << "\n\nPilihan: ";
    cin >> choice;
    cin.ignore();

    switch (choice)
    {
    case 1:
        menuProductManagement();
        break;
    case 2:
        updateStock();
        break;
    case 3:
        checkLowStock();
        break;
    case 4:
        stockReport();
        break;
    case 5:
        listUsers();
        break;
    case 6:
        viewTransactionHistory();
        break;
    case 0:
        logout();
        break;
    default:
        printError("Pilihan tidak valid!");
        pause();
    }
}