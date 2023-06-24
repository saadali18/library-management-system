#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helpers.h"
#include "books.h"

Book* library = NULL;


Book* createBook(char* ISBN, char* title, int author_id, enum book_tag* tags, int tag_count, int total_count)
{
    Book* book = malloc(sizeof(Book));
    if (!book)
    {
        printf("Fail to allocate memory for book.\n");
        return NULL;
    }

    book->ISBN = ISBN;
    book->title = title;
    book->author_id = author_id;
    book->status = active;

    if (tag_count > 0)
    {
        book->tags = malloc(tag_count * sizeof(enum book_tag));
        if (!book->tags)
        {
            printf("Fail to allocate memory for book tags.\n");
            free(book->title);
            free(book);
            return NULL;
        }
        for (int i = 0; i < tag_count; i++)	book->tags[i] = tags[i];

        book->tag_count = tag_count;
        book->total_count = book->in_stock_count = total_count;
    }
    book->likes = 0;
    book->next = NULL;
    return book;
}

void insertBook(Book* book, Book** head)
{
    if (!*head)
    {
        *head = book;
        return;
    }
    Book* current = *head;
    while (current->next)
        current = current->next;
    current->next = book;
}


void printBooks(Book* head)
{
    Book* book = head;
    while (book){
        printf("ISBN number: %s\n", book->ISBN);
        printf("Title: %s\n", book->title);
        printf("author ID: %i\n", book->author_id);
        printf("Book Status: %s\n", formatBookStatus(book->status));
        printf("Book Tags: ");
        for (int i = 0; i < book->tag_count; i++)
        {
            printf("%s ", formatBookTag(book->tags[i]));
        }
        printf("\n");
        printf("Total count: %i\n In-stock count: %i\n Likes: %i\n", book->total_count, book->in_stock_count, book->likes);
        printf("--------------\n");
        book = book->next;
    }
}

Book* searchBooks(char* keyword, Book* head)
{
    int count = 0;
    Book* search_result = NULL;
    Book* current = head;
    if (!current)
    {
        printf("Library is empty\n");
        return NULL;
    }

    while (current)
    {
        if (strstr(current->ISBN, keyword) || strstr(current->title, keyword) || \
			strstr(toString(current->author_id), keyword) || \
			strstr(formatBookStatus(current->status), keyword))
        {
            insertBook(current, &search_result);
            count++;
        }
        current = current->next;
    }
    return search_result;
}


Book* filterByISBN(char* input_ISBN, Book* head)
{
    Book* filtered_result = NULL;
    Book* current = head;
    while (current)
    {
        if (!strcmp(current->ISBN, input_ISBN))
            insertBook(current, &filtered_result);
        current = current->next;
    }
    return filtered_result;
}

Book* filterByTitle(char* input_title, Book* head)
{
    Book* filtered_result = NULL;
    Book* current = head;
    while (current)
    {
        if (!strcmp(current->title, input_title))
            insertBook(current, &filtered_result);
        current = current->next;
    }
    return filtered_result;
}

Book* filterByTotalCount(int input_count, Book* head)
{
    Book* filtered_result = NULL;
    Book* current = head;
    while (current)
    {
        if (head->tag_count == input_count)
            insertBook(current, &filtered_result);
        current = current->next;
    }
    return filtered_result;
}

Book* filterBooks(Book* parameters, Book* head){
    Book* filtered_books = head;

    if (parameters->ISBN) filtered_books = filterByISBN(parameters->ISBN, filtered_books);
    if (parameters->title) filtered_books = filterByTitle(parameters->title, filtered_books);
    if (parameters->total_count > 0) filtered_books = filterByTotalCount(parameters->total_count, filtered_books);
    return filtered_books;

//    NOTE: for fields like tag_count, author_id, instead of parameters->author_id check, we need parameters->author > 0
}