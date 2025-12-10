#ifndef PRODUCT_H
#define PRODUCT_H

#include "../../structs.h"

void menuProductManagement();

void createProduct();
void listProducts();
void updateProduct();
void deleteProduct();
void viewProductDetail();
void updateStock();
void checkLowStock();

Product* findProductByID(const char* productID);
Product* findProductByBarcode(const char* barcode);
void displayProductTable();

void manageCategoriesMenu();
void createCategory();
void listCategories();

#endif