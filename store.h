#ifndef STORE_H
#define STORE_H

#include "structs.h"

void menuStoreManagement();

void createStore();
void listStores();
void updateStore();
void deleteStore();
void viewStoreDetail();

Store* findStoreByID(const char* storeID);
void displayStoreTable();

#endif