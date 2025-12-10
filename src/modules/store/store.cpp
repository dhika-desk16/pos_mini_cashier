#include "store.h"
#include "../../globals/globals.h"
#include "../../utils/utils.h"
#include "../../utils/menuInput.h"
#include "../../fileio.h"
#include "../auth/auth.h"
#include <iostream>
#include <cstring>

using namespace std;

void menuStoreManagement()
{
    if (!hasPermission(getCurrentUser()->role, "stores", "read"))
    {
        printError("Anda tidak memiliki akses ke menu ini!");
        pause();
        return;
    }

    int choice;

    while (true)
    {
        clearScreen();
        printHeader("STORE MANAGEMENT");

        cout << "\n1. Tambah Store";
        cout << "\n2. Lihat Semua Store";
        cout << "\n3. Update Store";
        cout << "\n4. Hapus Store";
        cout << "\n5. Detail Store";
        cout << "\n0. Kembali";
        int choice = getMenuChoice("Pilihan: ");
        if (choice == -1) { pause(); continue; }

        switch (choice)
        {
        case 1:
            createStore();
            break;
        case 2:
            listStores();
            break;
        case 3:
            updateStore();
            break;
        case 4:
            deleteStore();
            break;
        case 5:
            viewStoreDetail();
            break;
        case 0:
            return;
        default:
            printError("Pilihan tidak valid!");
            pause();
        }
    }
}

void createStore()
{
    if (!hasPermission(getCurrentUser()->role, "stores", "create"))
    {
        printError("Anda tidak memiliki akses untuk membuat store!");
        pause();
        return;
    }

    clearScreen();
    printHeader("TAMBAH STORE BARU");
    displayStoreTable();
    if (storeCount >= MAX_STORES)
    {
        printError("Database store penuh!");
        pause();
        return;
    }

    Store newStore;

    generateStoreID(newStore.storeID);

    cout << "\nNama Store: ";
    cin.getline(newStore.storeName, 100);

    cout << "Alamat: ";
    cin.getline(newStore.address, 200);

    cout << "Telepon: ";
    cin.getline(newStore.phone, 15);

    if (!isValidPhone(newStore.phone))
    {
        printWarning("Format telepon tidak valid, tapi tetap disimpan.");
    }

    newStore.isActive = true;
    getCurrentDateTime(newStore.createdAt);

    stores[storeCount++] = newStore;
    saveStores();

    printSuccess("Store berhasil ditambahkan!");
    cout << "Store ID: " << newStore.storeID << endl;
    pause();
}

void listStores()
{
    clearScreen();
    printHeader("DAFTAR STORE");

    if (storeCount == 0)
    {
        printInfo("Belum ada store.");
        pause();
        return;
    }

    displayStoreTable();
    pause();
}

void updateStore()
{
    if (!hasPermission(getCurrentUser()->role, "stores", "update"))
    {
        printError("Anda tidak memiliki akses untuk update store!");
        pause();
        return;
    }

    clearScreen();
    printHeader("UPDATE STORE");

    displayStoreTable();

    char storeID[20];
    cout << "\nStore ID: ";
    cin.getline(storeID, 20);

    Store *store = findStoreByID(storeID);

    if (store == nullptr)
    {
        printError("Store tidak ditemukan!");
        pause();
        return;
    }

    cout << "\n=== Data Saat Ini ===";
    cout << "\nNama: " << store->storeName;
    cout << "\nAlamat: " << store->address;
    cout << "\nTelepon: " << store->phone;
    cout << "\nStatus: " << (store->isActive ? "Active" : "Inactive");

    cout << "\n\n=== Update Data ===";
    cout << "\n1. Nama Store";
    cout << "\n2. Alamat";
    cout << "\n3. Telepon";
    cout << "\n4. Toggle Status";
    cout << "\n0. Batal";
    int choice = getMenuChoice("\nPilihan: ");
    if (choice == -1) { pause(); return; }

    switch (choice)
    {
    case 1:
        cout << "Nama Store Baru: ";
        cin.getline(store->storeName, 100);
        printSuccess("Nama berhasil diupdate!");
        break;

    case 2:
        cout << "Alamat Baru: ";
        cin.getline(store->address, 200);
        printSuccess("Alamat berhasil diupdate!");
        break;

    case 3:
        cout << "Telepon Baru: ";
        cin.getline(store->phone, 15);
        printSuccess("Telepon berhasil diupdate!");
        break;

    case 4:
        store->isActive = !store->isActive;
        printSuccess(store->isActive ? "Store diaktifkan!" : "Store dinonaktifkan!");
        break;

    case 0:
        return;
    }

    saveStores();
    pause();
}

void deleteStore()
{
    if (!hasPermission(getCurrentUser()->role, "stores", "delete"))
    {
        printError("Anda tidak memiliki akses untuk hapus store!");
        pause();
        return;
    }

    clearScreen();
    printHeader("HAPUS STORE");

    displayStoreTable();

    char storeID[20];
    cout << "\nStore ID: ";
    cin.getline(storeID, 20);

    int index = -1;
    for (int i = 0; i < storeCount; i++)
    {
        if (strcmp(stores[i].storeID, storeID) == 0)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        printError("Store tidak ditemukan!");
        pause();
        return;
    }

    cout << "\nYakin ingin menghapus store: " << stores[index].storeName << "? (y/n): ";
    char confirm;
    cin >> confirm;

    if (confirm == 'y' || confirm == 'Y')
    {
        for (int i = index; i < storeCount - 1; i++)
        {
            stores[i] = stores[i + 1];
        }
        storeCount--;

        saveStores();
        printSuccess("Store berhasil dihapus!");
    }
    else
    {
        printInfo("Penghapusan dibatalkan.");
    }

    pause();
}

void viewStoreDetail()
{
    clearScreen();
    printHeader("DETAIL STORE");
    
    displayStoreTable();

    char storeID[20];
    cout << "\nStore ID: ";
    cin.getline(storeID, 20);

    Store *store = findStoreByID(storeID);

    if (store == nullptr)
    {
        printError("Store tidak ditemukan!");
        pause();
        return;
    }

    cout << "\n=== INFORMASI STORE ===";
    cout << "\nStore ID  : " << store->storeID;
    cout << "\nNama      : " << store->storeName;
    cout << "\nAlamat    : " << store->address;
    cout << "\nTelepon   : " << store->phone;
    cout << "\nStatus    : " << (store->isActive ? "Active" : "Inactive");
    cout << "\nDibuat    : " << store->createdAt;

    pause();
}

Store *findStoreByID(const char *storeID)
{
    for (int i = 0; i < storeCount; i++)
    {
        if (strcmp(stores[i].storeID, storeID) == 0)
        {
            return &stores[i];
        }
    }
    return nullptr;
}

void displayStoreTable()
{
    printLine('-', 80);
    cout << "ID      Nama Store              Telepon          Status\n";
    printLine('-', 80);

    for (int i = 0; i < storeCount; i++)
    {
        printf("%-8s %-24s %-16s %s\n",
               stores[i].storeID,
               stores[i].storeName,
               stores[i].phone,
               stores[i].isActive ? "Active" : "Inactive");
    }

    printLine('-', 80);
    cout << "Total: " << storeCount << " stores\n";
}