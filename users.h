#ifndef LMS_PRACTICE_ANNIE_CODE_USERS_H
#define LMS_PRACTICE_ANNIE_CODE_USERS_H

enum credential { author, customer, librarian };

enum user_status { inactive_user, active_user, delinquent };

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

User* createUser(int user_id, char* full_name, char* login_name, char* password, char* email, enum credential title);
void insertUser(User* user, User** head);
void printUser(User* user);
User* searchUsers(char* keyword, User** head);
User* filterbyUserID(int input_ID, User** head);
User* filterbyFullName(char* input_full_name, User** head);
User* filterbyLoginName(char* input_login_name, User** head);
User* filterbyEmail(char* input_email, User** head);
User* filterbyUserTitle(int input_title, User** head);
User* filterbyUserStatus(int input_status, User** head);
User* filterUsers(User* parameters, User** head);

#endif
