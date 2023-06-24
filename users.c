#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helpers.h"
#include "users.h"


User* createUser(int user_id, char* full_name, char* login_name, char* password, char* email, enum credential title)
{
    User* user = malloc(sizeof(User));
    if (!user)
    {
        printf("Fail to allocate memory.\n");
        return NULL;
    }

    user->user_id = user_id;
    user->full_name = full_name;
    user->login_name = login_name;
    user->password = password;
    user->email = email;
    user->title = title;
    user->status = active_user;
    user->next = NULL; // Add new user to end of list

    return user;
}

void insertUser(User* user, User** head)
{
    if (!*head) // exception thrown - access violation here - error msg: **head** was nullptr
    {
        *head = user;
        return;
    }
    User* current = *head;
    while (current->next)
        current = current->next;
    current->next = user;
}

void printUser(User* user)
{
    printf("User ID: %i\n", user->user_id);
    printf("Full name: %s\n", user->full_name);
    printf("Login name: %s\n", user->login_name);
    printf("Password: %s\n", user->password);
    printf("Email: %s\n", user->email);
    printf("Title: %s\n", formatUserCred(user->title));
    printf("Status: %s\n\n", formatUserStatus(user->status));
}


User* searchUsers(char* keyword, User** head)
{
//    TODO: Build it like books.c search function
}

User* filterbyUserID(int input_ID, User** head)
{
    User* filtered_result = NULL;
    User* current = *head;
    while (current)
    {
        if (current->user_id == input_ID)
            insertUser(current, &filtered_result);
        current = current->next;
    }
    return filtered_result;
}

User* filterbyFullName(char* input_full_name, User** head)
{
    User* filtered_result = NULL;
    User* current = *head;
    while (current)
    {
        if (!strcmp(current->full_name, input_full_name)) // exception thrown, access violation reading location
            insertUser(current, &filtered_result);
        current = current->next;
    }
    return filtered_result;
}

User* filterbyLoginName(char* input_login_name, User** head)
{
    User* filtered_result = NULL;
    User* current = *head;
    while (current)
    {
        if (!strcmp(current->login_name, input_login_name)) // if match
            insertUser(current, &filtered_result);
        current = current->next;
    }
    return filtered_result;
}

User* filterbyEmail(char* input_email, User** head)
{
    User* filtered_result = NULL;
    User* current = *head;
    while (current)
    {
        if (!strcmp(current->email, input_email)) // if match
            insertUser(current, &filtered_result);
        current = current->next;
    }
    return filtered_result;
}

User* filterbyUserTitle(int input_title, User** head)
{
    User* filtered_result = NULL;
    User* current = *head;
    while (current)
    {
        if (current->title == input_title)
            insertUser(current, &filtered_result);
        current = current->next;
    }
    return filtered_result;
}

User* filterbyUserStatus(int input_status, User** head)
{
    User* filtered_result = NULL;
    User* current = *head;
    while (current)
    {
        if (current->status == input_status)
            insertUser(current, &filtered_result);
        current = current->next;
    }
    return filtered_result;
}

User* filterUsers(User* parameters, User** head)
{
//    TODO: Build it like books.c filter function
}

