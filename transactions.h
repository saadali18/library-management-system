#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H
#include "users.h" // adding header
#include "books.h"
#include "helpers.h"

enum transaction_status { no_trans_status, open, close, past_due, purchased };

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

BookTransaction* createTransaction(int book_uid, Date* check_out_date, Date* due_date, Date* return_date, int user_id, enum transaction_status status);
void insertTransaction(BookTransaction* transaction, BookTransaction** head);
void printTransactions(BookTransaction* head);

Book* findSourceBook(char* ISBN);

BookTransaction* searchTransaction(char* keyword, BookTransaction* transaction_head, User* user_head);
BookTransaction* filterTransactionByUserID(int user_id, BookTransaction** head);
BookTransaction* filterTransactions(BookTransaction* parameters, BookTransaction* head);

void rentBook(Book* book, User* user);
void returnBook(int book_uid);
void purchaseBook(Book* book, User* user);

int countTransactions(BookTransaction* head);
void freeTransactionList(BookTransaction* head);

#endif
