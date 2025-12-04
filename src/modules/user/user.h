#ifndef USER_H
#define USER_H

#include "../../structs.h"

void menuUserManagement();

void createUser();
void listUsers();
void updateUser();
void deleteUser();
void viewUserDetail();

User *findUserByID(const char *userID);
User *findUserByUsername(const char *username);
void displayUserTable();

#endif