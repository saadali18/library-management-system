#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "users.h"

const int max_length = 30;

char* getLoginName()
{
    char* login_name = malloc(max_length * sizeof(char));
    printf("Enter login name: ");
    scanf("%s", login_name);
    return login_name;
}

char* getPassword()
{
    char* password = malloc(max_length * sizeof(char));
    printf("Enter password: ");
    scanf("%s", password);
    return password;
}

User* login(char* login_name, char* password)
{
    User* current = user_list_head;
    while (current)
    {
        if (!strcmp(current->login_name, login_name) && !strcmp(current->password, password)) {
            if (current->status == active_user) {
                printf("You have successfully logged in!\n");
                return current;
            } else {
                printf("You're not an active user. Please see admin.\n");
                return NULL;
            }
        }
        current = current->next;
    }
    printf("Incorrect login and/or password. Please try again.\n");
    return NULL;
}

void logout()
{
    printf("You have successfully logged out!\n");
}