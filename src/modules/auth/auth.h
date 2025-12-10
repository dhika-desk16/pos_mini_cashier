#ifndef AUTH_H
#define AUTH_H

#include "../../structs.h"

bool login();
void logout();
bool isLoggedIn();
User *getCurrentUser();
const char *getRoleName(Role role);

#endif