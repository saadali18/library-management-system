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
    result->return_date = transaction->return_date;
    insertTransaction(result, result_list);
}
// Linker function
BookCopy* findBookCopy(int book_uid)
{
    BookCopy* current = inventory;
    while (current)
    {
        if (current->book_uid == book_uid)  break;
        current = current->next;
    }
    return current;
}
 // Linker function
Book* findSourceBook(char* ISBN)
{
    Book* current = library;
    while(current)
    {
        if (isMatch(current->ISBN, ISBN))   break;
        current = current->next;
    }
    return current;
}

// Linker function
BookTransaction* findOriginalTransaction(int book_uid)
{
    BookTransaction* current = transaction_list;
    while (current)
    {
        if (current->book_uid == book_uid)  break;
        current = current->next;
    }
    return current;
}

// Linker function
User* findUserFromID(int user_id)
{
    User* current = user_list_head;
    while (current)
    {
        if (current->user_id == user_id)    break;
        current = current->next;
    }
    return current;
}

void rentBook(Book* book, User* user)
{
    // Check if source book has available copies in stock
    if (book->in_stock_count < 1)
    {
        printf("Rental transaction unsuccessful. No copy available in stock for rent\n--------------\n");
        return;
    }

    // Get the first available copy
    BookCopy* rented_copy = getBookCopy(book);

    // Create and insert transaction
    BookTransaction* rental = createTransaction(rented_copy->book_uid, getTodaysDate(), user->user_id, open);
    insertTransaction(rental, &transaction_list);

    // Print transaction status
    if (rental)
    {
        printf("Rental transaction is successful!\n");
        printf("Customer: %s\nBook title: %s\nBook Copy UID: %04i\n", user->full_name, book->title, rented_copy->book_uid);
        printf("Please return the book by: %i/%i/%i\n", rental->due_date->month, rental->due_date->day, rental->due_date->year);
        printf("---------------------------\n");
    }
    else printf("Rental unsuccessful. Please try again\n");

    // Update source book count and status if needed
    book->in_stock_count -= 1;
    if (book->in_stock_count < 1)    book->status = inactive;

    // Update rented copy status
    rented_copy->status = inactive;
}

void returnBook(int book_uid)
{
    // Find and update the returned book copy
    BookCopy* returned_copy = findBookCopy(book_uid);
    if (!returned_copy)
    {
        printf("Unable to find record of this book transaction. Please input the correct book unique ID\n");
        return;
    }
    returned_copy->status = active;

    // Update source book count and status
    Book* source_book = findSourceBook(returned_copy->ISBN);
    source_book->in_stock_count += 1;
    source_book->status = active;

    // Find and update original transaction
    BookTransaction* original_transaction = findOriginalTransaction(returned_copy->book_uid);
    original_transaction->return_date = getTodaysDate();
    original_transaction->status = close;

    // Print transaction status
    printf("Successfully return \"%s\"!\n", source_book->title);
    printf("Return date: %i/%i/%i\n", original_transaction->return_date->month, original_transaction->return_date->day, original_transaction->return_date->year);
    printf("Thank you for choosing LMS!\n--------------------\n");
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
        User* user = findUserFromID(current->user_id);

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