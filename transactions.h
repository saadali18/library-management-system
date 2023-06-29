#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H
#include "users.h" // adding header
enum transaction_status { no_trans_status, open, close };

typedef struct Date
{
    int year;
    int month;
    int day;
} Date;

typedef struct BookTransaction
{
    int book_uid;
    Date* check_out_date;
    Date* due_date;
    Date* return_date;
    int user_id;
    enum transaction_status status;
    struct BookTransaction* next;
} BookTransaction;

extern BookTransaction* transaction_list;

Date* addDaysToDates(Date* date, int days);
BookTransaction* createTransaction(int book_uid, Date* check_out_date, int user_id, enum transaction_status status);
void insertTransaction(BookTransaction* transaction, BookTransaction** head);
void printTransactions(BookTransaction* head);
BookTransaction* searchTransaction(char* keyword, BookTransaction* transaction_head, User* user_head); // program doesn't recognize struct User unless using #include here
BookTransaction* filterTransactions(BookTransaction* parameters, BookTransaction* head);

#endif
