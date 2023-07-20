#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "helpers.h"
#include "books.h"
#include "transactions.h"

Book* library = NULL;
BookCopy* inventory = NULL;

Book* createBook(char* ISBN, char* title, int author_id, enum book_status status, enum book_tag* tags, int tag_count, int total_count, int in_stock_count, int likes)
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
    book->status = status;

    book->tag_count = tag_count;
    if (tag_count > 0)
    {
        book->tags = malloc(tag_count * sizeof(enum book_tag));
        if (!book->tags)
        {
            printf("Fail to allocate memory for book tags.\n");
            free(book);
            return NULL;
        }
        for (int i = 0; i < tag_count; i++)	{
            book->tags[i] = tags[i];
        }
    }
    else    book->tags = NULL;

    book->total_count = total_count;
    book->in_stock_count = in_stock_count;
    book->likes = likes;
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

void copyBookToResult(Book* book, Book** result_list)
{
    Book* result = createBook(book->ISBN,book->title, book->author_id, book->status, book->tags, book->tag_count, \
    book->total_count, book->in_stock_count, book->likes);
    insertBook(result, result_list);
}

void printBooks(Book* head)
{
    Book* book = head;
    while (book){
        printf("ISBN number: %s\n", book->ISBN);
        printf("Title: %s\n", book->title);
        printf("Author ID: %04i\n", book->author_id);
        printf("Book Status: %s\n", formatBookStatus(book->status));
        printf("Book Tags: ");
        for (int i = 0; i < book->tag_count; i++)
            {
                printf("%s ", formatBookTag(book->tags[i]));
            }
        printf("\n");
        printf("Total count: %i\nIn-stock count: %i\nLikes: %i\n", book->total_count, book->in_stock_count, book->likes);
        printf("--------------\n");
        book = book->next;
    }
}

Book* searchBooks(char* keyword, Book* head)
{
    Book* search_result = NULL;
    Book* current = head;
    if (!current)
    {
        printf("Library is empty\n");
        return NULL;
    }

    while (current)
    {
        bool is_tag = true;
        if (isContain(current->ISBN, keyword) || isContain(current->title, keyword) || \
			isContain(toString(current->author_id), keyword) || \
			isContain(formatBookStatus(current->status), keyword))
        {
            copyBookToResult(current, &search_result);
            is_tag = false;
        }
        if (is_tag){
            for (int i = 0; i < current->tag_count; i++) {
                if (isContain(formatBookTag(current->tags[i]), keyword)) {
                    copyBookToResult(current, &search_result);
                    break;
                }
            }
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
        if (isMatch(current->ISBN, input_ISBN))
        {
            copyBookToResult(current, &filtered_result);
            return filtered_result; // unique should only return 1 record;
        }
        current = current->next;
    }
}

Book* filterByTitle(char* input_title, Book* head)
{
    Book* filtered_result = NULL;
    Book* current = head;
    while (current)
    {
        if (isMatch(current->title, input_title))
            copyBookToResult(current, &filtered_result);
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
        if (current->total_count >= input_count)
            copyBookToResult(current, &filtered_result);
        current = current->next;
    }
    return filtered_result;
}

Book* filterByTags(enum book_tag* input_tags, int input_tag_count, Book* head) // return books only if all tags matched
{
    Book* filtered_result = NULL;
    Book* current = head;
    while (current)
    {
        int total_match = 0;
        for (int i = 0; i < current->tag_count; i++)
        {
            for (int j = 0; j < input_tag_count; j++)
            {
                if (current->tags[i] == input_tags[j])  total_match++;
            }
        }
        if (total_match == input_tag_count) copyBookToResult(current, &filtered_result);
        current = current->next;
    }
    return filtered_result;
}

Book* filterBooks(Book* parameters, Book* head){
    Book* filtered_books = head;

    if (parameters->ISBN) filtered_books = filterByISBN(parameters->ISBN, filtered_books);
    if (parameters->title) filtered_books = filterByTitle(parameters->title, filtered_books);
    if (parameters->total_count > 0) filtered_books = filterByTotalCount(parameters->total_count, filtered_books);
    if (parameters->tag_count > 0)   filtered_books = filterByTags(parameters->tags, parameters->tag_count, filtered_books);
    return filtered_books;
}

BookCopy* createBookCopy(int book_uid, char* ISBN, enum book_status status)
{
    BookCopy* book_copy = malloc(sizeof(BookCopy));
    if (!book_copy)
    {
        printf("Fail to allocate memory for book.\n");
        return NULL;
    }
    book_copy->book_uid = book_uid;
    book_copy->ISBN = ISBN;
    book_copy->status = status;
    book_copy->next = NULL;
    return book_copy;
}

void insertBookCopy(BookCopy* book_copy, BookCopy** head)
{
    if (!*head)
    {
        *head = book_copy;
        return;
    }
    BookCopy* current = *head;
    while (current->next)
        current = current->next;
    current->next = book_copy;
}

BookCopy* getBookCopy(Book* book)
{
    BookCopy* current = inventory;
    while (current)
    {
        if (!strcmp(current->ISBN, book->ISBN) && current->status == active)
            break;
        current = current->next;
    }
    return current;
}

void printBookCopies(BookCopy* head)
{
    BookCopy* copy = head;
    while (copy)
    {
        printf("Book UID: %04i\n", copy->book_uid);
        printf("ISBN: %s\n", copy->ISBN);
        printf("Status: %s\n", formatBookStatus(copy->status));
        printf("--------------------------\n");
        copy = copy->next;
    }
}

int countBooks(Book* head)
{
    int count = 0;
    Book* current = head;
    while (current)
    {
        count++;
        current = current->next;
    }
    return count;
}

int countBookCopies(BookCopy* head)
{
    int count = 0;
    BookCopy* current = head;
    while (current)
    {
        count++;
        current = current->next;
    }
    return count;
}

void freeBookList(Book* head)
{
    while (head)
    {
        Book* temp = head;
        head = head->next;
        free(temp);
    }
}



