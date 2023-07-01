#include <stdio.h>
#include <stdlib.h>
#include "transactions.h"
#include "books.h"
#include "users.h"
#include "helpers.h"

BookTransaction* transaction_list = NULL;

BookTransaction* createTransaction(int book_uid, Date* check_out_date, int user_id, enum transaction_status status)
{
    BookTransaction* transaction = malloc(sizeof(BookTransaction));
    if (!transaction)
    {
        printf("Fail to allocate memory.\n");
        return NULL;
    }

    transaction->book_uid = book_uid;
    transaction->check_out_date = check_out_date;
    transaction->due_date = addDaysToDates(check_out_date, rental_days);
    transaction->return_date = NULL;
    transaction->user_id = user_id;
    transaction->status = status;
    transaction->next = NULL;

    return transaction;
}

void insertTransaction(BookTransaction* transaction, BookTransaction** head)
{
    if (!*head)
    {
        *head = transaction;
        return;
    }
    BookTransaction* current = *head;
    while (current->next)
        current = current->next;
    current->next = transaction;
}

void copyTransactionToResult(BookTransaction* transaction, BookTransaction** result_list)
{
    BookTransaction* result = createTransaction(transaction->book_uid, transaction->check_out_date, transaction->user_id, transaction->status);
    insertTransaction(result, result_list);
}

BookCopy* findBookCopy(int book_uid, BookCopy* book_copy_head)
{
    BookCopy* current = book_copy_head;
    while (current)
    {
        if (current->book_uid == book_uid)  return current;
        current = current->next;
    }
}

Book* findSourceBook(char* ISBN, Book* book_head)
{
    Book* current = book_head;
    while(current)
    {
        if (isMatch(current->ISBN, ISBN))   return current;
        current = current->next;
    }
}

BookTransaction* findOriginalTransaction(int book_uid, BookTransaction* transaction_head)
{
    BookTransaction* current = transaction_head;
    while (current)
    {
        if (current->book_uid == book_uid)  return current;
        current = current->next;
    }
}

User* findUserFromID(int user_id, User* head)
{
    User* current = head;
    while (current)
    {
        if (current->user_id == user_id)    return current;
        current = current->next;
    }
}

void rentBook(BookTransaction* transaction, Book* book_head, BookCopy* book_copy_head)
{
    // Find the copy being rented out
    BookCopy* rented_copy = findBookCopy(transaction->book_uid, book_copy_head);
    // Check if this book copy is available for rent
    if (rented_copy->status == inactive)
    {
        printf("This copy is unavailable. Please choose a different copy of this book!\n");
        return;
    }
    else rented_copy->status = inactive; // Update book copy status
    // Update source book count and status
    Book* source_book = findSourceBook(rented_copy->ISBN, book_head);
    source_book->in_stock_count -= 1;
    if (source_book->in_stock_count < 1)    source_book->status = inactive;
}

void returnBook(int book_uid, Book* book_head, BookCopy* book_copy_head, BookTransaction* transaction_head)
{
    //Update Book Copy status after return
    BookCopy* returned_book_copy = findBookCopy(book_uid, book_copy_head);
    returned_book_copy->status = active;
    // Update source book in stock count and status (if needed) after return
    Book* source_book = findSourceBook(returned_book_copy->ISBN, book_head);
    source_book->in_stock_count += 1;
    if (source_book->in_stock_count > 0)    source_book->status = active;
    // Update original book transaction return date and status
    BookTransaction* original_transaction = findOriginalTransaction(book_uid, transaction_head);
    original_transaction->return_date  = getTodaysDate();
    original_transaction->status = close;
}

void printTransactions(BookTransaction* head)
{
    BookTransaction* transaction = head;
    while (transaction)
    {
        printf("Book UID: %02i\n", transaction->book_uid);
        printf("Check-out date: %i/%i/%i\n", transaction->check_out_date->month, transaction->check_out_date->day, transaction->check_out_date->year);
        printf("Due date: %i/%i/%i\n", transaction->due_date->month, transaction->due_date->day, transaction->due_date->year);
        printf("Return date: ");
        if (transaction->return_date)
            printf("%i/%i/%i\n", transaction->return_date->month, transaction->return_date->day, transaction->return_date->year);
        else printf("pending\n");
        printf("User ID: %i\n", transaction->user_id);
        printf("Transaction status: %s\n", formatTransactionStatus(transaction->status));
        printf("--------------------------\n");
        transaction = transaction->next;
    }
}

BookTransaction* searchTransaction(char* keyword, BookTransaction* transaction_head, User* user_head)
{
    BookTransaction* search_result = NULL;
    BookTransaction* current = transaction_head;
    if (!current)
    {
        printf("Transaction list is empty\n");
        return NULL;
    }

    while (current)
    {
        User* user = findUserFromID(current->user_id, user_head);

        if (isContain(toString(current->book_uid), keyword) || isContain(user->full_name, keyword) || \
        isContain(formatBookStatus(current->status), keyword))
        {
            copyTransactionToResult(current, &search_result);
        }
        current = current->next;
    }
    return search_result;
}

BookTransaction* filterByBookID(int book_uid, BookTransaction** head)
{
    BookTransaction* filtered_result = NULL;
    BookTransaction* current = *head;
    while (current)
    {
        if (current->book_uid == book_uid)
            copyTransactionToResult(current, &filtered_result);
        current = current->next;
    }
    return filtered_result;
}

BookTransaction* filterByCheckoutDate(Date* input, BookTransaction** head)
{
    BookTransaction* filtered_result = NULL;
    BookTransaction* current = *head;
    while (current)
    {
        if (current->check_out_date->year == input->year && current->check_out_date->month == input->month \
			&& current->check_out_date->day == input->day)		copyTransactionToResult(current, &filtered_result);
        current = current->next;
    }
    return filtered_result;
}

BookTransaction* filterByDueDate(Date* input, BookTransaction** head) // same logic, different field
{
    BookTransaction* filtered_result = NULL;
    BookTransaction* current = *head;
    while (current)
    {
        if (compareDate(current->due_date, input))
            copyTransactionToResult(current, &filtered_result);
        current = current->next;
    }
    return filtered_result;
}

BookTransaction* filterByReturnDate(Date* input, BookTransaction** head) // same logic, different field
{
    BookTransaction* filtered_result = NULL;
    BookTransaction* current = *head;
    while (current)
    {
        if (compareDate(current->return_date, input))
            copyTransactionToResult(current, &filtered_result);
        current = current->next;
    }
    return filtered_result;
}

BookTransaction* filterTransactionByUserID(int user_id, BookTransaction** head)
{
    BookTransaction* filtered_result = NULL;
    BookTransaction* current = *head;
    while (current)
    {
        if (current->user_id == user_id)
            copyTransactionToResult(current, &filtered_result);
        current = current->next;
    }
    return filtered_result;
}

BookTransaction* filterTransactions(BookTransaction* parameters, BookTransaction* head)
{
    BookTransaction* filtered_transactions = head;

    if (parameters->book_uid > 0)	filtered_transactions = filterByBookID(parameters->book_uid, &filtered_transactions);
    if (parameters->check_out_date)		filtered_transactions = filterByCheckoutDate(parameters->check_out_date, &filtered_transactions);
    if (parameters->due_date)	filtered_transactions = filterByDueDate(parameters->due_date, &filtered_transactions);
    if (parameters->return_date)	filtered_transactions = filterByReturnDate(parameters->return_date, &filtered_transactions);
    if (parameters->user_id > 0)	filtered_transactions = filterTransactionByUserID(parameters->user_id, &filtered_transactions);

    return filtered_transactions;
}

int countTransactions(BookTransaction* head)
{
    int count = 0;
    BookTransaction* current = head;
    while (current)
    {
        count++;
        current = current->next;
    }
    return count;
}


void freeTransactionList(BookTransaction* head)
{
    while (head)
    {
        BookTransaction* temp = head;
        head = head->next;
        free(temp);
    }
}