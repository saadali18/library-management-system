#include <stdio.h>
#include <stdlib.h>
#include "transactions.h"


const int rental_days = 90;

Date* addDaystoDates(Date* date, int days)
{
    Date* calculated_date = date;
    int days_in_month[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    calculated_date->day += days;

    while (calculated_date->day > days_in_month[calculated_date->month])
    {
        calculated_date->day -= days_in_month[calculated_date->month];
        calculated_date->month++;

        if (calculated_date->month > 12)
        {
            calculated_date->month -= 12;
            calculated_date->year++;
        }
    }

    return calculated_date;
}

BookTransaction* createTransaction(int book_uid, Date* check_out_date, int user_id)
{
    BookTransaction* transaction = malloc(sizeof(BookTransaction));
    if (!transaction)
    {
        printf("Fail to allocate memory.\n");
        return NULL;
    }

    transaction->book_uid = book_uid;
    transaction->check_out_date = check_out_date;
    transaction->due_date = addDaystoDates(check_out_date, rental_days);
    transaction->user_id = user_id;
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

BookTransaction* searchbyBookID(int book_uid, BookTransaction** head)
{
    BookTransaction* search_result = NULL;
    BookTransaction* current = *head;
    if (!current)
    {
        printf("No transaction found\n");
        return NULL;
    }

    while (current)
    {
        if (current->book_uid == book_uid)
            insertTransaction(current, &search_result);
        current = current->next;
    }
    return search_result;
}

BookTransaction* searchbyCheckoutDate(Date* input, BookTransaction** head)
{
    BookTransaction* search_result = NULL;
    BookTransaction* current = *head;
    if (!current)
    {
        printf("No transaction found\n");
        return NULL;
    }

    while (current)
    {
        if (current->check_out_date->year == input->year && current->check_out_date->month == input->month \
			&& current->check_out_date->day == input->day)		insertTransaction(current, &search_result);
        current = current->next;
    }
    return search_result;
}

BookTransaction* searchbyDueDate(Date* input, BookTransaction** head) // same logic, different field
{
    BookTransaction* search_result = NULL;
    BookTransaction* current = *head;
    if (!current)
    {
        printf("No transaction found\n");
        return NULL;
    }

    while (current)
    {
        if (current->due_date->year == input->year && current->due_date->month == input->month \
			&& current->due_date->day == input->day)		insertTransaction(current, &search_result);
        current = current->next;
    }
    return search_result;
}

BookTransaction* searchbyReturnDate(Date* input, BookTransaction** head) // same logic, different field
{
    BookTransaction* search_result = NULL;
    BookTransaction* current = *head;
    if (!current)
    {
        printf("No transaction found\n");
        return NULL;
    }

    while (current)
    {
        if (current->return_date->year == input->year && current->return_date->month == input->month \
			&& current->return_date->day == input->day)		insertTransaction(current, &search_result);
        current = current->next;
    }
    return search_result;
}

BookTransaction* searchbyUserID(int user_id, BookTransaction** head)
{
    BookTransaction* search_result = NULL;
    BookTransaction* current = *head;
    if (!current)
    {
        printf("No transaction found\n");
        return NULL;
    }

    while (current)
    {
        if (current->user_id == user_id)
            insertTransaction(current, &search_result);
        current = current->next;
    }
    return search_result;
}


BookTransaction* searchTransactions(BookTransaction* parameters, BookTransaction** head)
{
    BookTransaction* search_result = *head;
    if (!search_result)
    {
        printf("No transaction found\n");
        return NULL;
    }

    if (parameters->book_uid)	search_result = searchbyBookID(parameters->book_uid, &search_result);
    if (parameters->check_out_date)		search_result = searchbyCheckoutDate(parameters->check_out_date, &search_result);
    if (parameters->due_date)	search_result = searchbyDueDate(parameters->due_date, &search_result);
    if (parameters->return_date)	search_result = searchbyReturnDate(parameters->return_date, &search_result);
    if (parameters->user_id)	search_result = searchbyUserID(parameters->user_id, &search_result);

    return search_result;
}