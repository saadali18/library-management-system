#ifndef AUTH_H
#define AUTH_H

#include <stdbool.h>

char* getLoginName();
char* getPassword();
User* login(char* login_name, char* password);
void logout();

#endif
