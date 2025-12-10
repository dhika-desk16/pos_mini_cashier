#include "transaction.h"
#include "../../globals/globals.h"
#include "../../utils/utils.h"
#include "../../fileio.h"
#include "../../modules/auth/auth.h"
#include "../../modules/shift/shift.h"
#include "../../modules/product/product.h"
#include <iostream>
#include <cstring>

using namespace std;

void menuCashier()
{
    if (getCurrentUser()->role != CASHIER)
    {
        printError("Menu ini hanya untuk Kasir!");
        pause();
        return;
    }

    if (!currentSession.isShiftOpen)
    {
        printError("Buka shift terlebih dahulu!");
        pause();
        return;
    }

    int choice;

    while (true)
    {
        clearScreen();
        printHeader("KASIR - TRANSAKSI");

        cout << "\n[SHIFT: " << currentSession.currentShift.shiftID << "]";
        cout << "\nKasir: " << getCurrentUser()->fullName;

        int itemCount = countCartItems();
        double total = calculateCartTotal();

        cout << "\n\n[KERANJANG: " << itemCount << " item | Total: Rp" << total << "]";

        cout << "\n\n1. Scan Produk";
        cout << "\n2. Lihat Keranjang";
        cout << "\n3. Hapus Item dari Keranjang";
        cout << "\n4. Checkout";
        cout << "\n5. Batal (Kosongkan Keranjang)";
        cout << "\n6. Riwayat Transaksi";
        cout << "\n0. Kembali";
        cout << "\n\nPilihan: ";
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
            scanProduct();
            break;
        case 2:
            viewCart();
            break;
        case 3:
            removeFromCart();
            break;
        case 4:
            processCheckout();
            break;
        case 5:
            clearCart();
            printSuccess("Keranjang dikosongkan!");
            pause();
            break;
        case 6:
            viewTransactionHistory();
            break;
        case 0:
            return;
        default:
            printError("Pilihan tidak valid!");
            pause();
        }
    }
}

void scanProduct()
{
    clearScreen();
    printHeader("SCAN PRODUK");

    listProducts();

    char barcode[50];
    cout << "\nBarcode (atau Product ID): ";
    cin.getline(barcode, 50);

    Product *product = findProductByBarcode(barcode);
    if (product == nullptr)
    {
        product = findProductByID(barcode);
    }

    if (product == nullptr)
    {
        printError("Produk tidak ditemukan!");
        pause();
        return;
    }

    if (!product->isActive)
    {
        printError("Produk tidak aktif!");
        pause();
        return;
    }

    cout << "\n=== INFORMASI PRODUK ===";
    cout << "\nNama    : " << product->productName;
    cout << "\nHarga   : Rp" << product->sellingPrice;
    cout << "\nStok    : " << product->stock;

    if (product->stock == 0)
    {
        printError("Stok habis!");
        pause();
        return;
    }

    int quantity;
    cout << "\n\nJumlah: ";
    cin >> quantity;
    cin.ignore();

    if (quantity <= 0)
    {
        printError("Jumlah harus positif!");
        pause();
        return;
    }

    if (quantity > product->stock)
    {
        printError("Stok tidak cukup!");
        cout << "Tersedia: " << product->stock << " unit\n";
        pause();
        return;
    }

    addToCart(product, quantity);

    printSuccess("Produk ditambahkan ke keranjang!");
    cout << quantity << " x " << product->productName << " = Rp" << (quantity * product->sellingPrice) << endl;

    pause();
}

void viewCart()
{
    clearScreen();
    printHeader("KERANJANG BELANJA");

    if (cartHead == nullptr)
    {
        printInfo("Keranjang kosong!");
        pause();
        return;
    }

    printLine('-', 80);
    cout << "Nama Produk              Harga       Qty     Subtotal\n";
    printLine('-', 80);

    CartItem *current = cartHead;
    while (current != nullptr)
    {
        printf("%-24s Rp%-10.0f %-8d Rp%.0f\n",
               current->product->productName,
               current->product->sellingPrice,
               current->quantity,
               current->subtotal);
        current = current->next;
    }

    printLine('-', 80);
    cout << "TOTAL: Rp" << calculateCartTotal() << endl;
    printLine('-', 80);

    pause();
}

void removeFromCart()
{
    if (cartHead == nullptr)
    {
        printError("Keranjang kosong!");
        pause();
        return;
    }

    viewCart();

    char productID[20];
    cout << "\nProduct ID atau Barcode yang akan dihapus: ";
    cin.getline(productID, 20);

    Product *product = findProductByBarcode(productID);
    if (product == nullptr)
    {
        product = findProductByID(productID);
    }

    if (product == nullptr)
    {
        printError("Produk tidak ditemukan!");
        pause();
        return;
    }

    CartItem *current = cartHead;
    CartItem *prev = nullptr;

    while (current != nullptr)
    {
        if (current->product == product)
        {
            if (prev == nullptr)
            {
                cartHead = current->next;
            }
            else
            {
                prev->next = current->next;
            }
            delete current;
            printSuccess("Produk dihapus dari keranjang!");
            pause();
            return;
        }
        prev = current;
        current = current->next;
    }

    printError("Produk tidak ada di keranjang!");
    pause();
}

void clearCart()
{
    CartItem *current = cartHead;
    while (current != nullptr)
    {
        CartItem *temp = current;
        current = current->next;
        delete temp;
    }
    cartHead = nullptr;
}
void processCheckout()
{
    if (cartHead == nullptr)
    {
        printError("Keranjang kosong!");
        pause();
        return;
    }
    clearScreen();
    printHeader("CHECKOUT");

    viewCart();

    double subtotal = calculateCartTotal();

    cout << "\n=== PERHITUNGAN ===";
    cout << "\nSubtotal: Rp" << subtotal;

    double discount = 0;
    cout << "\nDiskon (%): ";
    double discountPercent;
    cin >> discountPercent;
    discount = subtotal * (discountPercent / 100.0);

    double tax = 0;
    cout << "Pajak (%): ";
    double taxPercent;
    cin >> taxPercent;
    tax = (subtotal - discount) * (taxPercent / 100.0);

    double total = subtotal - discount + tax;

    cout << "\nDiskon: Rp" << discount;
    cout << "\nPajak: Rp" << tax;
    cout << "\n"
         << string(30, '-');
    cout << "\nTOTAL: Rp" << total;

    cout << "\n\nMetode Pembayaran:";
    cout << "\n1. Cash";
    cout << "\n2. Debit";
    cout << "\n3. Credit";
    cout << "\nPilihan: ";

    int paymentChoice;
    cin >> paymentChoice;

    char paymentMethod[20];
    switch (paymentChoice)
    {
    case 1:
        strcpy(paymentMethod, "CASH");
        break;
    case 2:
        strcpy(paymentMethod, "DEBIT");
        break;
    case 3:
        strcpy(paymentMethod, "CREDIT");
        break;
    default:
        strcpy(paymentMethod, "CASH");
    }

    double paidAmount, changeAmount;

    if (strcmp(paymentMethod, "CASH") == 0)
    {
        cout << "\nJumlah bayar: Rp";
        cin >> paidAmount;

        if (paidAmount < total)
        {
            printError("Uang tidak cukup!");
            pause();
            return;
        }

        changeAmount = paidAmount - total;
    }
    else
    {
        paidAmount = total;
        changeAmount = 0;
    }

    Transaction newTransaction;
    generateTransactionID(newTransaction.transactionID);
    strcpy(newTransaction.shiftID, currentSession.currentShift.shiftID);
    strcpy(newTransaction.cashierID, getCurrentUser()->userID);
    strcpy(newTransaction.storeID, getCurrentUser()->storeID);
    getCurrentDateTime(newTransaction.transactionDate);
    newTransaction.subtotal = subtotal;
    newTransaction.discount = discount;
    newTransaction.tax = tax;
    newTransaction.totalAmount = total;
    newTransaction.paidAmount = paidAmount;
    newTransaction.changeAmount = changeAmount;
    newTransaction.itemCount = countCartItems();
    strcpy(newTransaction.paymentMethod, paymentMethod);

    CartItem *current = cartHead;
    while (current != nullptr)
    {
        current->product->stock -= current->quantity;
        current = current->next;
    }
    saveProducts();

    current = cartHead;
    while (current != nullptr)
    {
        TransactionDetail *detail = new TransactionDetail;
        strcpy(detail->transactionID, newTransaction.transactionID);
        strcpy(detail->productID, current->product->productID);
        strcpy(detail->productName, current->product->productName);
        detail->quantity = current->quantity;
        detail->price = current->product->sellingPrice;
        detail->subtotal = current->subtotal;
        detail->next = transactionDetailHead;
        transactionDetailHead = detail;

        current = current->next;
    }

    transactions[transactionCount++] = newTransaction;
    saveTransactions();
    saveTransactionDetails(newTransaction.transactionID);

    currentSession.currentShift.totalTransactions++;
    currentSession.currentShift.totalSales += total;

    printReceipt(&newTransaction);

    clearCart();

    TransactionDetail *detailCurrent = transactionDetailHead;
    while (detailCurrent != nullptr)
    {
        TransactionDetail *temp = detailCurrent;
        detailCurrent = detailCurrent->next;
        delete temp;
    }
    transactionDetailHead = nullptr;

    printSuccess("Transaksi berhasil!");
    pause();
}
void printReceipt(Transaction *transaction)
{
    cout << "\n\n";
    printLine('=', 50);
    cout << "           STRUK PEMBAYARAN\n";
    printLine('=', 50);
    cout << "Store: " << transaction->storeID << endl;
    cout << "Tanggal: " << transaction->transactionDate << endl;
    cout << "Kasir: " << getCurrentUser()->fullName << endl;
    cout << "No. Transaksi: " << transaction->transactionID << endl;
    printLine('-', 50);
    TransactionDetail *current = transactionDetailHead;
    while (current != nullptr)
    {
        cout << current->productName << endl;
        printf("  %d x Rp%.0f = Rp%.0f\n",
               current->quantity,
               current->price,
               current->subtotal);
        current = current->next;
    }

    printLine('-', 50);
    printf("Subtotal:  Rp%.0f\n", transaction->subtotal);
    printf("Diskon:    Rp%.0f\n", transaction->discount);
    printf("Pajak:     Rp%.0f\n", transaction->tax);
    printLine('-', 50);
    printf("TOTAL:     Rp%.0f\n", transaction->totalAmount);
    printf("Bayar:     Rp%.0f\n", transaction->paidAmount);
    printf("Kembalian: Rp%.0f\n", transaction->changeAmount);
    printLine('=', 50);
    cout << "    Terima kasih atas kunjungan Anda!\n";
    printLine('=', 50);
}

void addToCart(Product *product, int quantity)
{

    CartItem *current = cartHead;
    while (current != nullptr)
    {
        if (current->product == product)
        {
            current->quantity += quantity;
            current->subtotal = current->quantity * current->product->sellingPrice;
            return;
        }
        current = current->next;
    }
    CartItem *newItem = new CartItem;
    newItem->product = product;
    newItem->quantity = quantity;
    newItem->subtotal = quantity * product->sellingPrice;
    newItem->next = cartHead;
    cartHead = newItem;
}
double calculateCartTotal()
{
    double total = 0;
    CartItem *current = cartHead;
    while (current != nullptr)
    {
        total += current->subtotal;
        current = current->next;
    }
    return total;
}
int countCartItems()
{
    int count = 0;
    CartItem *current = cartHead;
    while (current != nullptr)
    {
        count++;
        current = current->next;
    }
    return count;
}
Transaction *findTransactionByID(const char *transactionID)
{
    for (int i = 0; i < transactionCount; i++)
    {
        if (strcmp(transactions[i].transactionID, transactionID) == 0)
        {
            return &transactions[i];
        }
    }
    return nullptr;
}
void viewTransactionHistory()
{
    clearScreen();
    printHeader("RIWAYAT TRANSAKSI");
    if (transactionCount == 0)
    {
        printInfo("Belum ada transaksi.");
        pause();
        return;
    }

    bool found = false;

    printLine('-', 90);
    cout << "ID Transaksi         Tanggal              Item  Total          Metode\n";
    printLine('-', 90);

    for (int i = transactionCount - 1; i >= 0; i--)
    {
        if (strcmp(transactions[i].shiftID, currentSession.currentShift.shiftID) == 0)
        {
            printf("%-20s %-20s %-6d Rp%-12.0f %s\n",
                   transactions[i].transactionID,
                   transactions[i].transactionDate,
                   transactions[i].itemCount,
                   transactions[i].totalAmount,
                   transactions[i].paymentMethod);
            found = true;
        }
    }

    printLine('-', 90);

    if (!found)
    {
        printInfo("Belum ada transaksi di shift ini.");
    }

    pause();
}
void viewTransactionDetail()
{
    clearScreen();
    printHeader("DETAIL TRANSAKSI");
    char transactionID[30];
    cout << "\nTransaction ID: ";
    cin.getline(transactionID, 30);

    Transaction *transaction = findTransactionByID(transactionID);

    if (transaction == nullptr)
    {
        printError("Transaksi tidak ditemukan!");
        pause();
        return;
    }

    cout << "\n=== INFORMASI TRANSAKSI ===";
    cout << "\nID Transaksi  : " << transaction->transactionID;
    cout << "\nTanggal       : " << transaction->transactionDate;
    cout << "\nKasir         : " << transaction->cashierID;
    cout << "\nStore         : " << transaction->storeID;
    cout << "\nJumlah Item   : " << transaction->itemCount;
    cout << "\nSubtotal      : Rp" << transaction->subtotal;
    cout << "\nDiskon        : Rp" << transaction->discount;
    cout << "\nPajak         : Rp" << transaction->tax;
    cout << "\nTotal         : Rp" << transaction->totalAmount;
    cout << "\nBayar         : Rp" << transaction->paidAmount;
    cout << "\nKembalian     : Rp" << transaction->changeAmount;
    cout << "\nMetode Bayar  : " << transaction->paymentMethod;

    pause();
}