#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helpers.h"
#include "users.h"

User* user_list_head = NULL;

User* createUser(int user_id, char* full_name, char* login_name, char* password, char* email, enum credential title, enum user_status status)
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
    user->status = status;
    user->next = NULL;

    return user;
}

void insertUser(User* user, User** head)
{
    if (!*head)
    {
        *head = user;
        return;
    }
    User* current = *head;
    while (current->next)
        current = current->next;
    current->next = user;
}

void copyUserToResult(User* user, User** result_list)
{
    User* result = createUser(user->user_id, user->full_name, user->login_name, user->password,\
    user->email, user->title, user->status);
    insertUser(result, result_list);
}

void printUser(User* head)
{
    User* user = head;
    while (user)
    {
        printf("User ID: %i\n", user->user_id);
        printf("Full name: %s\n", user->full_name);
        printf("Login name: %s\n", user->login_name);
        printf("Password: %s\n", user->password);
        printf("Email: %s\n", user->email);
        printf("Title: %s\n", formatUserCred(user->title));
        printf("Status: %s\n\n", formatUserStatus(user->status));
        user = user->next;
    }
}

User* searchUsers(char* keyword, User* head)
{
    User* search_result = NULL;
    User* current = head;
    if (!current)
    {
        printf("User list is empty\n");
        return NULL;
    }

    while (current)
    {
        if (isContain(toString(current->user_id), keyword) || isContain(current->full_name, keyword) || \
        isContain(current->login_name, keyword) || isContain(current->email, keyword) || \
        isContain(formatUserCred(current->title), keyword) || isContain(formatUserStatus(current->status), keyword))
        {
            copyUserToResult(current, &search_result);
        }
        current = current->next;
    }
    return search_result;
}

User* filterByUserID(int input_ID, User* head)
{
    User* filtered_result = NULL;
    User* current = head;
    while (current)
    {
        if (current->user_id == input_ID)
        {
            copyUserToResult(current, &filtered_result);
            return filtered_result; // unique, should only return 1 record
        }
        current = current->next;
    }
}

User* filterByFullName(char* input_full_name, User* head)
{
    User* filtered_result = NULL;
    User* current = head;
    while (current)
    {
        if (isMatch(current->full_name, input_full_name))
            copyUserToResult(current, &filtered_result);
        current = current->next;
    }
    return filtered_result;
}

User* filterByLoginName(char* input_login_name, User* head)
{
    User* filtered_result = NULL;
    User* current = head;
    while (current)
    {
        if (isMatch(current->login_name, input_login_name))
            copyUserToResult(current, &filtered_result);
        current = current->next;
    }
    return filtered_result;
}

User* filterByEmail(char* input_email, User* head)
{
    User* filtered_result = NULL;
    User* current = head;
    while (current)
    {
        if (isMatch(current->email, input_email))
            copyUserToResult(current, &filtered_result);
        current = current->next;
    }
    return filtered_result;
}

User* filterByUserTitle(enum credential input_title, User* head)
{
    User* filtered_result = NULL;
    User* current = head;
    while (current)
    {
        if (current->title == input_title)
            copyUserToResult(current, &filtered_result);
        current = current->next;
    }
    return filtered_result;
}

User* filterByUserStatus(enum user_status input_status, User* head)
{
    User* filtered_result = NULL;
    User* current = head;
    while (current)
    {
        if (current->status == input_status)
            copyUserToResult(current, &filtered_result);
        current = current->next;
    }
    return filtered_result;
}

User* filterUsers(User* parameters, User* head)
{
    User* filtered_users = head;
    if (parameters->user_id > 0) filtered_users = filterByUserID(parameters->user_id, filtered_users);
    if (parameters->full_name) filtered_users = filterByFullName(parameters->full_name, filtered_users);
    if (parameters->login_name) filtered_users = filterByLoginName(parameters->login_name, filtered_users);
    if (parameters->email) filtered_users = filterByEmail(parameters->email, filtered_users);
    if (parameters->title > 0) filtered_users = filterByUserTitle(parameters->title, filtered_users);
    if (parameters->status > 0) filtered_users = filterByUserStatus(parameters->status, filtered_users);
    return filtered_users;
}

int countUsers(User* head)
{
    int count = 0;
    User* current = head;
    while (current)
    {
        count++;
        current = current->next;
    }
    return count;
}

