#ifndef USERS_H
#define USERS_H

enum credential { no_credential, author, customer, librarian };

enum user_status { no_user_status, inactive_user, active_user, delinquent, deleted_user }; // add deleted status for soft delete

typedef struct User
{
    int user_id;
    char* full_name;
    char* login_name;
    char* password;
    char* email;
    enum credential title;
    enum user_status status;
    struct User* next;
} User;

extern User* user_list_head;

User* createUser(int user_id, char* full_name, char* login_name, char* password, char* email, enum credential title, enum user_status status);
void insertUser(User* user, User** head);
void printUser(User* head);
User* searchUsers(char* keyword, User* head);
User* filterByUserID(int input_ID, User* head);
User* filterByFullName(char* input_full_name, User* head);
User* filterByUserTitle(enum credential input_title, User* head);
User* filterUsers(User* parameters, User* head);

int countUsers(User* head);
void freeUserList(User* head);


#endif
