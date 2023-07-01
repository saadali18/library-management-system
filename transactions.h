#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H
#include "users.h" // adding header
#include "books.h"
#include "helpers.h"

enum transaction_status { no_trans_status, open, close, past_due };

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

BookTransaction* createTransaction(int book_uid, Date* check_out_date, int user_id, enum transaction_status status);
void insertTransaction(BookTransaction* transaction, BookTransaction** head);
void printTransactions(BookTransaction* head);
BookTransaction* searchTransaction(char* keyword, BookTransaction* transaction_head, User* user_head);
BookTransaction* filterTransactions(BookTransaction* parameters, BookTransaction* head);

void rentBook(BookTransaction* transaction, Book* book_head, BookCopy* book_copy_head);
void returnBook(int book_uid, Book* book_head, BookCopy* book_copy_head, BookTransaction* transaction_head);

void freeTransactionList(BookTransaction* head);

#endif
