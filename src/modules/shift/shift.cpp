#include "shift.h"
#include "../../globals/globals.h"
#include "../../utils/utils.h"
#include "../../fileio.h"
#include "../../modules/auth/auth.h"
#include <iostream>
#include <cstring>

using namespace std;

void menuShiftManagement()
{
    if (getCurrentUser()->role != CASHIER)
    {
        printError("Menu ini hanya untuk Kasir!");
        pause();
        return;
    }

    int choice;

    while (true)
    {
        clearScreen();
        printHeader("SHIFT MANAGEMENT");

        if (currentSession.isShiftOpen)
        {
            cout << "\n[SHIFT AKTIF]";
            cout << "\nShift ID: " << currentSession.currentShift.shiftID;
            cout << "\nDibuka: " << currentSession.currentShift.openTime;
            cout << "\nSaldo Awal: Rp" << currentSession.currentShift.openingBalance;
            cout << "\nTotal Transaksi: " << currentSession.currentShift.totalTransactions;
            cout << "\nTotal Penjualan: Rp" << currentSession.currentShift.totalSales;
        }
        else
        {
            cout << "\n[TIDAK ADA SHIFT AKTIF]";
        }

        cout << "\n\n1. Buka Shift";
        cout << "\n2. Tutup Shift";
        cout << "\n3. Info Shift Saat Ini";
        cout << "\n0. Kembali";
        cout << "\n\nPilihan: ";
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
            openShift();
            break;
        case 2:
            closeShift();
            break;
        case 3:
            viewCurrentShift();
            break;
        case 0:
            return;
        default:
            printError("Pilihan tidak valid!");
            pause();
        }
    }
}

void openShift()
{
    if (currentSession.isShiftOpen)
    {
        printError("Shift sudah terbuka! Tutup shift terlebih dahulu.");
        pause();
        return;
    }

    clearScreen();
    printHeader("BUKA SHIFT");

    Shift newShift;

    generateShiftID(newShift.shiftID);
    strcpy(newShift.cashierID, getCurrentUser()->userID);
    strcpy(newShift.storeID, getCurrentUser()->storeID);
    getCurrentDateTime(newShift.openTime);
    strcpy(newShift.closeTime, "-");

    cout << "\nMasukkan saldo awal kas: Rp";
    cin >> newShift.openingBalance;
    cin.ignore();

    newShift.closingBalance = 0;
    newShift.totalSales = 0;
    newShift.totalTransactions = 0;
    newShift.isOpen = true;

    currentSession.currentShift = newShift;
    currentSession.isShiftOpen = true;

    printSuccess("Shift berhasil dibuka!");
    cout << "Shift ID: " << newShift.shiftID << endl;
    cout << "Waktu Buka: " << newShift.openTime << endl;
    cout << "Saldo Awal: Rp" << newShift.openingBalance << endl;

    pause();
}

void closeShift()
{
    if (!currentSession.isShiftOpen)
    {
        printError("Tidak ada shift yang terbuka!");
        pause();
        return;
    }

    clearScreen();
    printHeader("TUTUP SHIFT");

    cout << "\n=== RINGKASAN SHIFT ===";
    cout << "\nShift ID: " << currentSession.currentShift.shiftID;
    cout << "\nDibuka: " << currentSession.currentShift.openTime;
    cout << "\nSaldo Awal: Rp" << currentSession.currentShift.openingBalance;
    cout << "\nTotal Transaksi: " << currentSession.currentShift.totalTransactions;
    cout << "\nTotal Penjualan: Rp" << currentSession.currentShift.totalSales;

    cout << "\n\nMasukkan saldo akhir kas (fisik): Rp";
    cin >> currentSession.currentShift.closingBalance;
    cin.ignore();

    double expectedBalance = currentSession.currentShift.openingBalance +
                             currentSession.currentShift.totalSales;
    double difference = currentSession.currentShift.closingBalance - expectedBalance;

    cout << "\n=== HASIL PERHITUNGAN ===";
    cout << "\nSaldo yang seharusnya: Rp" << expectedBalance;
    cout << "\nSaldo aktual: Rp" << currentSession.currentShift.closingBalance;
    cout << "\nSelisih: Rp" << difference;

    if (difference > 0)
    {
        printInfo("Ada kelebihan kas.");
    }
    else if (difference < 0)
    {
        printWarning("Ada kekurangan kas!");
    }
    else
    {
        printSuccess("Kas balance!");
    }

    getCurrentDateTime(currentSession.currentShift.closeTime);
    currentSession.currentShift.isOpen = false;

    saveShiftLog(&currentSession.currentShift);

    cout << "\nYakin ingin menutup shift? (y/n): ";
    char confirm;
    cin >> confirm;

    if (confirm == 'y' || confirm == 'Y')
    {
        currentSession.isShiftOpen = false;
        printSuccess("Shift berhasil ditutup!");
    }
    else
    {
        currentSession.currentShift.isOpen = true;
        printInfo("Penutupan shift dibatalkan.");
    }

    pause();
}

void viewCurrentShift()
{
    if (!currentSession.isShiftOpen)
    {
        printError("Tidak ada shift yang terbuka!");
        pause();
        return;
    }

    clearScreen();
    printHeader("INFO SHIFT SAAT INI");

    cout << "\n=== INFORMASI SHIFT ===";
    cout << "\nShift ID         : " << currentSession.currentShift.shiftID;
    cout << "\nKasir            : " << getCurrentUser()->fullName;
    cout << "\nStore            : " << currentSession.currentShift.storeID;
    cout << "\nWaktu Buka       : " << currentSession.currentShift.openTime;
    cout << "\nSaldo Awal       : Rp" << currentSession.currentShift.openingBalance;
    cout << "\nTotal Transaksi  : " << currentSession.currentShift.totalTransactions;
    cout << "\nTotal Penjualan  : Rp" << currentSession.currentShift.totalSales;
    cout << "\nSaldo Seharusnya : Rp" << (currentSession.currentShift.openingBalance + currentSession.currentShift.totalSales);

    pause();
}

bool isShiftOpen()
{
    return currentSession.isShiftOpen;
}