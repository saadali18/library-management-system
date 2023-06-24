#ifndef TRANSACTIONS_H

#define TRANSACTIONS_H

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
    struct BookTransaction* next;
} BookTransaction;

Date* addDaystoDates(Date* date, int days);
BookTransaction* createTransaction(int book_uid, Date* check_out_date, int user_id);
void insertTransaction(BookTransaction* transaction, BookTransaction** head);
BookTransaction* searchbyBookID(int book_uid, BookTransaction** head);
BookTransaction* searchbyCheckoutDate(Date* input, BookTransaction** head);
BookTransaction* searchbyDueDate(Date* input, BookTransaction** head);
BookTransaction* searchbyReturnDate(Date* input, BookTransaction** head);
BookTransaction* searchbyUserID(int user_id, BookTransaction** head);
BookTransaction* searchTransactions(BookTransaction* parameters, BookTransaction** head);

#endif
