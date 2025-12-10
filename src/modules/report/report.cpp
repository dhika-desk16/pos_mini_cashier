#include "report.h"
#include "../../globals/globals.h"
#include "../../utils/utils.h"
#include "../../modules/auth/auth.h"
#include "../../modules/product/product.h"
#include <iostream>
#include <cstring>

using namespace std;

void menuReporting() {
    int choice;
    
    while (true) {
        clearScreen();
        printHeader("REPORTING & ANALYTICS");
        
        cout << "\n1. Laporan Penjualan Harian";
        cout << "\n2. Laporan Shift";
        cout << "\n3. Laporan Penjualan per Produk";
        cout << "\n4. Laporan Stok";
        cout << "\n5. Laporan Stok Menipis";
        cout << "\n0. Kembali";
        cout << "\n\nPilihan: ";
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1: dailySalesReport(); break;
            case 2: shiftReport(); break;
            case 3: productSalesReport(); break;
            case 4: stockReport(); break;
            case 5: lowStockReport(); break;
            case 0: return;
            default: printError("Pilihan tidak valid!"); pause();
        }
    }
}

void dailySalesReport() {
    clearScreen();
    printHeader("LAPORAN PENJUALAN HARIAN");
    
    char date[50];
    cout << "\nMasukkan tanggal (YYYY-MM-DD) atau 'today': ";
    cin.getline(date, 50);
    
    if (strcmp(date, "today") == 0) {
        getCurrentDate(date);
    }
    
    double totalSales = 0;
    int totalTransactions = 0;
    int totalItems = 0;
    double totalDiscount = 0;
    double totalTax = 0;
    
    
    int cashCount = 0, debitCount = 0, creditCount = 0;
    double cashTotal = 0, debitTotal = 0, creditTotal = 0;
    
    bool found = false;
    
    for (int i = 0; i < transactionCount; i++) {
        
        if (strstr(transactions[i].transactionDate, date) != nullptr) {
            found = true;
            totalSales += transactions[i].totalAmount;
            totalTransactions++;
            totalItems += transactions[i].itemCount;
            totalDiscount += transactions[i].discount;
            totalTax += transactions[i].tax;
            
            if (strcmp(transactions[i].paymentMethod, "CASH") == 0) {
                cashCount++;
                cashTotal += transactions[i].totalAmount;
            } else if (strcmp(transactions[i].paymentMethod, "DEBIT") == 0) {
                debitCount++;
                debitTotal += transactions[i].totalAmount;
            } else if (strcmp(transactions[i].paymentMethod, "CREDIT") == 0) {
                creditCount++;
                creditTotal += transactions[i].totalAmount;
            }
        }
    }
    
    if (!found) {
        printInfo("Tidak ada transaksi pada tanggal tersebut.");
        pause();
        return;
    }
    
    printLine('=', 60);
    cout << "LAPORAN PENJUALAN HARIAN\n";
    cout << "Tanggal: " << date << endl;
    printLine('=', 60);
    
    cout << "\n=== RINGKASAN PENJUALAN ===";
    cout << "\nTotal Transaksi    : " << totalTransactions;
    cout << "\nTotal Item Terjual : " << totalItems;
    cout << "\nTotal Penjualan    : Rp" << totalSales;
    cout << "\nTotal Diskon       : Rp" << totalDiscount;
    cout << "\nTotal Pajak        : Rp" << totalTax;
    cout << "\nRata-rata per Trx  : Rp" << (totalSales / totalTransactions);
    
    cout << "\n\n=== METODE PEMBAYARAN ===";
    cout << "\nCash   : " << cashCount << " transaksi (Rp" << cashTotal << ")";
    cout << "\nDebit  : " << debitCount << " transaksi (Rp" << debitTotal << ")";
    cout << "\nCredit : " << creditCount << " transaksi (Rp" << creditTotal << ")";
    
    printLine('=', 60);
    
    pause();
}

void shiftReport() {
    if (getCurrentUser()->role != CASHIER && getCurrentUser()->role != STORE_MANAGER) {
        printError("Anda tidak memiliki akses untuk melihat laporan shift!");
        pause();
        return;
    }
    
    clearScreen();
    printHeader("LAPORAN SHIFT");
    
    if (!currentSession.isShiftOpen) {
        printError("Tidak ada shift yang aktif!");
        pause();
        return;
    }
    
    Shift* shift = &currentSession.currentShift;
    
    printLine('=', 60);
    cout << "LAPORAN SHIFT\n";
    cout << "Shift ID: " << shift->shiftID << endl;
    printLine('=', 60);
    
    cout << "\n=== INFORMASI SHIFT ===";
    cout << "\nKasir              : " << getCurrentUser()->fullName;
    cout << "\nStore              : " << shift->storeID;
    cout << "\nWaktu Buka         : " << shift->openTime;
    cout << "\nStatus             : " << (shift->isOpen ? "AKTIF" : "DITUTUP");
    
    cout << "\n\n=== RINGKASAN KEUANGAN ===";
    cout << "\nSaldo Awal         : Rp" << shift->openingBalance;
    cout << "\nTotal Penjualan    : Rp" << shift->totalSales;
    cout << "\nSaldo Seharusnya   : Rp" << (shift->openingBalance + shift->totalSales);
    
    if (!shift->isOpen) {
        cout << "\nSaldo Akhir        : Rp" << shift->closingBalance;
        double difference = shift->closingBalance - (shift->openingBalance + shift->totalSales);
        cout << "\nSelisih            : Rp" << difference;
        
        if (difference > 0) {
            cout << " (Kelebihan)";
        } else if (difference < 0) {
            cout << " (Kekurangan)";
        } else {
            cout << " (Balance)";
        }
    }
    
    cout << "\n\n=== STATISTIK TRANSAKSI ===";
    cout << "\nJumlah Transaksi   : " << shift->totalTransactions;
    
    if (shift->totalTransactions > 0) {
        cout << "\nRata-rata per Trx  : Rp" << (shift->totalSales / shift->totalTransactions);
    }
    
    printLine('=', 60);
    
    pause();
}

void productSalesReport() {
    clearScreen();
    printHeader("LAPORAN PENJUALAN PER PRODUK");
    
    
    struct ProductSales {
        char productID[20];
        char productName[100];
        int quantitySold;
        double totalRevenue;
    };
    
    ProductSales salesData[MAX_PRODUCTS];
    int salesCount = 0;
    
    
    for (int i = 0; i < MAX_PRODUCTS; i++) {
        salesData[i].quantitySold = 0;
        salesData[i].totalRevenue = 0;
    }
    
    
    for (int i = 0; i < productCount; i++) {
        strcpy(salesData[i].productID, products[i].productID);
        strcpy(salesData[i].productName, products[i].productName);
        salesData[i].quantitySold = 0;
        salesData[i].totalRevenue = 0;
        salesCount++;
    }
    
    printLine('-', 80);
    cout << "Produk ID    Nama Produk              Qty Terjual  Total Revenue\n";
    printLine('-', 80);
    
    bool found = false;
    for (int i = 0; i < salesCount; i++) {
        if (salesData[i].quantitySold > 0) {
            printf("%-12s %-24s %-13d Rp%.0f\n",
                salesData[i].productID,
                salesData[i].productName,
                salesData[i].quantitySold,
                salesData[i].totalRevenue);
            found = true;
        }
    }
    
    printLine('-', 80);
    
    if (!found) {
        printInfo("Belum ada penjualan.");
    }
    
    pause();
}

void stockReport() {
    clearScreen();
    printHeader("LAPORAN STOK");
    
    if (productCount == 0) {
        printInfo("Belum ada produk.");
        pause();
        return;
    }
    
    int totalProducts = 0;
    int activeProducts = 0;
    int lowStockProducts = 0;
    int outOfStockProducts = 0;
    double totalStockValue = 0;
    
    printLine('-', 90);
    cout << "ID          Nama Produk              Stok    Min     Nilai Stok\n";
    printLine('-', 90);
    
    for (int i = 0; i < productCount; i++) {
        totalProducts++;
        if (products[i].isActive) activeProducts++;
        if (products[i].stock <= products[i].minStock) lowStockProducts++;
        if (products[i].stock == 0) outOfStockProducts++;
        
        double stockValue = products[i].stock * products[i].purchasePrice;
        totalStockValue += stockValue;
        
        printf("%-12s %-24s %-8d %-8d Rp%.0f\n",
            products[i].productID,
            products[i].productName,
            products[i].stock,
            products[i].minStock,
            stockValue);
    }
    
    printLine('-', 90);
    
    cout << "\n=== RINGKASAN STOK ===";
    cout << "\nTotal Produk       : " << totalProducts;
    cout << "\nProduk Aktif       : " << activeProducts;
    cout << "\nStok Menipis       : " << lowStockProducts;
    cout << "\nStok Habis         : " << outOfStockProducts;
    cout << "\nNilai Total Stok   : Rp" << totalStockValue;
    
    pause();
}

void lowStockReport() {
    clearScreen();
    printHeader("LAPORAN STOK MENIPIS");
    
    bool found = false;
    
    printLine('-', 80);
    cout << "ID          Nama Produk              Stok    Min     Status\n";
    printLine('-', 80);
    
    for (int i = 0; i < productCount; i++) {
        if (products[i].stock <= products[i].minStock && products[i].isActive) {
            printf("%-12s %-24s %-8d %-8d %s\n",
                products[i].productID,
                products[i].productName,
                products[i].stock,
                products[i].minStock,
                products[i].stock == 0 ? "HABIS!" : "Menipis");
            found = true;
        }
    }
    
    printLine('-', 80);
    
    if (!found) {
        printInfo("Tidak ada produk dengan stok menipis.");
    } else {
        printWarning("Segera lakukan restock untuk produk di atas!");
    }
    
    pause();
}


void calculateDailySales(const char* date, double* totalSales, int* totalTransactions) {
    *totalSales = 0;
    *totalTransactions = 0;
    
    for (int i = 0; i < transactionCount; i++) {
        if (strstr(transactions[i].transactionDate, date) != nullptr) {
            *totalSales += transactions[i].totalAmount;
            (*totalTransactions)++;
        }
    }
}

void getMostSoldProducts(int topN) {
    
    
}