#include "product.h"
#include "../../globals/globals.h"
#include "../../utils/utils.h"
#include "../../utils/menuInput.h"
#include "../../fileio.h"
#include "../../modules/auth/auth.h"
#include <iostream>
#include <cstring>

using namespace std;

void menuProductManagement()
{
    if (!hasPermission(getCurrentUser()->role, "products", "read"))
    {
        printError("Anda tidak memiliki akses ke menu ini!");
        pause();
        return;
    }

    int choice;

    while (true)
    {
        clearScreen();
        printHeader("PRODUCT MANAGEMENT");

        cout << "\n1. Tambah Produk";
        cout << "\n2. Lihat Semua Produk";
        cout << "\n3. Update Produk";
        cout << "\n4. Hapus Produk";
        cout << "\n5. Detail Produk";
        cout << "\n6. Update Stok";
        cout << "\n7. Cek Stok Menipis";
        cout << "\n8. Kelola Kategori";
        cout << "\n0. Kembali";
        int choice = getMenuChoice("\nPilihan: ");
        if (choice == -1) { pause(); continue; }

        switch (choice)
        {
        case 1:
            createProduct();
            break;
        case 2:
            listProducts();
            break;
        case 3:
            updateProduct();
            break;
        case 4:
            deleteProduct();
            break;
        case 5:
            viewProductDetail();
            break;
        case 6:
            updateStock();
            break;
        case 7:
            checkLowStock();
            break;
        case 8:
            manageCategoriesMenu();
            break;
        case 0:
            return;
        default:
            printError("Pilihan tidak valid!");
            pause();
        }
    }
}

void createProduct()
{
    if (!hasPermission(getCurrentUser()->role, "products", "create"))
    {
        printError("Anda tidak memiliki akses untuk membuat produk!");
        pause();
        return;
    }

    clearScreen();
    printHeader("TAMBAH PRODUK BARU");

    listProducts();

    if (productCount >= MAX_PRODUCTS)
    {
        printError("Database produk penuh!");
        pause();
        return;
    }

    Product newProduct;

    generateProductID(newProduct.productID);

    cout << "\nBarcode: ";
    cin.getline(newProduct.barcode, 50);

    if (findProductByBarcode(newProduct.barcode) != nullptr)
    {
        printError("Barcode sudah digunakan!");
        pause();
        return;
    }

    cout << "Nama Produk: ";
    cin.getline(newProduct.productName, 100);

    cout << "\nDaftar Kategori:\n";
    for (int i = 0; i < categoryCount; i++)
    {
        cout << categories[i].categoryID << " - " << categories[i].categoryName << "\n";
    }
    cout << "Category ID: ";
    cin.getline(newProduct.categoryID, 20);

    cout << "Stok Awal: ";
    cin >> newProduct.stock;

    cout << "Stok Minimal (alert): ";
    cin >> newProduct.minStock;

    cout << "Harga Beli: Rp";
    cin >> newProduct.purchasePrice;

    cout << "Harga Jual: Rp";
    cin >> newProduct.sellingPrice;
    cin.ignore();

    User *currentUser = getCurrentUser();
    if (currentUser->role == SUPER_ADMIN || strcmp(currentUser->storeID, "ALL") == 0)
    {
        cout << "Store ID: ";
        cin.getline(newProduct.storeID, 20);
    }
    else
    {
        strcpy(newProduct.storeID, currentUser->storeID);
    }

    newProduct.isActive = true;

    products[productCount++] = newProduct;
    saveProducts();

    printSuccess("Produk berhasil ditambahkan!");
    cout << "Product ID: " << newProduct.productID << endl;
    pause();
}

void listProducts()
{
    clearScreen();
    printHeader("DAFTAR PRODUK");

    if (productCount == 0)
    {
        printInfo("Belum ada produk.");
        pause();
        return;
    }

    displayProductTable();
    pause();
}

void updateProduct()
{
    if (!hasPermission(getCurrentUser()->role, "products", "update"))
    {
        printError("Anda tidak memiliki akses untuk update produk!");
        pause();
        return;
    }

    clearScreen();
    printHeader("UPDATE PRODUK");

    listProducts();

    char productID[20];
    cout << "\nProduct ID: ";
    cin.getline(productID, 20);

    Product *product = findProductByID(productID);

    if (product == nullptr)
    {
        printError("Produk tidak ditemukan!");
        pause();
        return;
    }

    cout << "\n=== Data Saat Ini ===";
    cout << "\nNama: " << product->productName;
    cout << "\nBarcode: " << product->barcode;
    cout << "\nStok: " << product->stock;
    cout << "\nHarga Jual: Rp" << product->sellingPrice;

    cout << "\n\n=== Update Data ===";
    cout << "\n1. Nama Produk";
    cout << "\n2. Harga Jual";
    cout << "\n3. Harga Beli";
    cout << "\n4. Stok Minimal";
    cout << "\n5. Toggle Status";
    cout << "\n0. Batal";
    cout << "\n\nPilihan: ";

    int choice;
    cin >> choice;
    cin.ignore();

    switch (choice)
    {
    case 1:
        cout << "Nama Produk Baru: ";
        cin.getline(product->productName, 100);
        printSuccess("Nama berhasil diupdate!");
        break;

    case 2:
        cout << "Harga Jual Baru: Rp";
        cin >> product->sellingPrice;
        printSuccess("Harga jual berhasil diupdate!");
        break;

    case 3:
        cout << "Harga Beli Baru: Rp";
        cin >> product->purchasePrice;
        printSuccess("Harga beli berhasil diupdate!");
        break;

    case 4:
        cout << "Stok Minimal Baru: ";
        cin >> product->minStock;
        printSuccess("Stok minimal berhasil diupdate!");
        break;

    case 5:
        product->isActive = !product->isActive;
        printSuccess(product->isActive ? "Produk diaktifkan!" : "Produk dinonaktifkan!");
        break;

    case 0:
        return;
    }

    saveProducts();
    pause();
}

void deleteProduct()
{
    if (!hasPermission(getCurrentUser()->role, "products", "delete"))
    {
        printError("Anda tidak memiliki akses untuk hapus produk!");
        pause();
        return;
    }

    clearScreen();
    printHeader("HAPUS PRODUK");

    listProducts();

    char productID[20];
    cout << "\nProduct ID: ";
    cin.getline(productID, 20);

    int index = -1;
    for (int i = 0; i < productCount; i++)
    {
        if (strcmp(products[i].productID, productID) == 0)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        printError("Produk tidak ditemukan!");
        pause();
        return;
    }

    cout << "\nYakin ingin menghapus produk: " << products[index].productName << "? (y/n): ";
    char confirm;
    cin >> confirm;

    if (confirm == 'y' || confirm == 'Y')
    {
        for (int i = index; i < productCount - 1; i++)
        {
            products[i] = products[i + 1];
        }
        productCount--;

        saveProducts();
        printSuccess("Produk berhasil dihapus!");
    }
    else
    {
        printInfo("Penghapusan dibatalkan.");
    }

    pause();
}

void viewProductDetail()
{
    clearScreen();
    printHeader("DETAIL PRODUK");

    listProducts();

    char productID[20];
    cout << "\nProduct ID atau Barcode: ";
    cin.getline(productID, 20);

    Product *product = findProductByID(productID);
    if (product == nullptr)
    {
        product = findProductByBarcode(productID);
    }

    if (product == nullptr)
    {
        printError("Produk tidak ditemukan!");
        pause();
        return;
    }

    cout << "\n=== INFORMASI PRODUK ===";
    cout << "\nProduct ID    : " << product->productID;
    cout << "\nBarcode       : " << product->barcode;
    cout << "\nNama Produk   : " << product->productName;
    cout << "\nKategori      : " << product->categoryID;
    cout << "\nStok          : " << product->stock;
    cout << "\nStok Minimal  : " << product->minStock;
    cout << "\nHarga Beli    : Rp" << product->purchasePrice;
    cout << "\nHarga Jual    : Rp" << product->sellingPrice;
    cout << "\nMargin        : Rp" << (product->sellingPrice - product->purchasePrice);
    cout << "\nStore         : " << product->storeID;
    cout << "\nStatus        : " << (product->isActive ? "Active" : "Inactive");

    if (product->stock <= product->minStock)
    {
        printWarning("Stok menipis! Perlu restock.");
    }

    pause();
}

void updateStock()
{
    if (!hasPermission(getCurrentUser()->role, "products", "update"))
    {
        printError("Anda tidak memiliki akses untuk update stok!");
        pause();
        return;
    }

    clearScreen();
    printHeader("UPDATE STOK");

    listProducts();

    char productID[20];
    cout << "\nProduct ID atau Barcode: ";
    cin.getline(productID, 20);

    Product *product = findProductByID(productID);
    if (product == nullptr)
    {
        product = findProductByBarcode(productID);
    }

    if (product == nullptr)
    {
        printError("Produk tidak ditemukan!");
        pause();
        return;
    }

    cout << "\nProduk: " << product->productName;
    cout << "\nStok saat ini: " << product->stock;

    cout << "\n\n1. Tambah Stok";
    cout << "\n2. Kurangi Stok";
    cout << "\n3. Set Stok Baru";
    int choice = getMenuChoice("\nPilihan: ");
    if (choice == -1) { pause(); return; }

    int amount;
    switch (choice)
    {
    case 1:
        cout << "Jumlah tambahan: ";
        cin >> amount;
        product->stock += amount;
        printSuccess("Stok berhasil ditambah!");
        break;

    case 2:
        cout << "Jumlah pengurangan: ";
        cin >> amount;
        if (amount > product->stock)
        {
            printError("Pengurangan melebihi stok tersedia!");
        }
        else
        {
            product->stock -= amount;
            printSuccess("Stok berhasil dikurangi!");
        }
        break;

    case 3:
        cout << "Stok baru: ";
        cin >> product->stock;
        printSuccess("Stok berhasil diupdate!");
        break;
    default : 
        cout << "Pilihan tidak valid!";
        break;
    }

    cout << "Stok sekarang: " << product->stock << endl;

    saveProducts();
    pause();
}

void checkLowStock()
{
    clearScreen();
    printHeader("CEK STOK MENIPIS");

    bool found = false;

    printLine('-', 80);
    cout << "ID          Nama Produk              Stok    Min     Status\n";
    printLine('-', 80);

    for (int i = 0; i < productCount; i++)
    {
        if (products[i].stock <= products[i].minStock && products[i].isActive)
        {
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

    if (!found)
    {
        printInfo("Tidak ada produk dengan stok menipis.");
    }

    pause();
}

Product *findProductByID(const char *productID)
{
    for (int i = 0; i < productCount; i++)
    {
        if (strcmp(products[i].productID, productID) == 0)
        {
            return &products[i];
        }
    }
    return nullptr;
}

Product *findProductByBarcode(const char *barcode)
{
    for (int i = 0; i < productCount; i++)
    {
        if (strcmp(products[i].barcode, barcode) == 0)
        {
            return &products[i];
        }
    }
    return nullptr;
}

void displayProductTable()
{
    printLine('-', 90);
    cout << "ID          Barcode        Nama Produk              Stok    Harga       Status\n";
    printLine('-', 90);

    for (int i = 0; i < productCount; i++)
    {
        printf("%-12s %-15s %-24s %-8d Rp%-9.0f %s\n",
               products[i].productID,
               products[i].barcode,
               products[i].productName,
               products[i].stock,
               products[i].sellingPrice,
               products[i].isActive ? "Active" : "Inactive");
    }

    printLine('-', 90);
    cout << "Total: " << productCount << " products\n";
}

void manageCategoriesMenu()
{
    int choice;

    while (true)
    {
        clearScreen();
        printHeader("KELOLA KATEGORI");

        cout << "\n1. Tambah Kategori";
        cout << "\n2. Lihat Kategori";
        cout << "\n0. Kembali";
        cout << "\n\nPilihan: ";
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
            createCategory();
            break;
        case 2:
            listCategories();
            break;
        case 0:
            return;
        default:
            printError("Pilihan tidak valid!");
            pause();
        }
    }
}

void createCategory()
{
    clearScreen();
    printHeader("TAMBAH KATEGORI");

    if (categoryCount >= MAX_CATEGORIES)
    {
        printError("Database kategori penuh!");
        pause();
        return;
    }

    Category newCategory;

    generateCategoryID(newCategory.categoryID);

    cout << "\nNama Kategori: ";
    cin.getline(newCategory.categoryName, 50);

    for (int i = 0; i < categoryCount; i++) {
        if (strcmp(categories[i].categoryName, newCategory.categoryName) == 0) {
            printError("Nama kategori sudah ada!");
            pause();
            return;
        }
    }

    categories[categoryCount++] = newCategory;
    saveCategories();

    printSuccess("Kategori berhasil ditambahkan!");
    cout << "Category ID: " << newCategory.categoryID << endl;
    pause();
}

void listCategories()
{
    clearScreen();
    printHeader("DAFTAR KATEGORI");

    if (categoryCount == 0)
    {
        printInfo("Belum ada kategori.");
        pause();
        return;
    }

    printLine('-', 50);
    cout << "ID          Nama Kategori\n";
    printLine('-', 50);

    for (int i = 0; i < categoryCount; i++)
    {
        printf("%-12s %s\n", categories[i].categoryID, categories[i].categoryName);
    }

    printLine('-', 50);
    cout << "Total: " << categoryCount << " categories\n";

    pause();
}