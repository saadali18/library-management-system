#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helpers.h"
#include "books.h"

Book* library = NULL;

// Add more fields in createBook() so we can use it later in copyBook() as well
Book* createBook(char* ISBN, char* title, int author_id, enum book_tag* tags, int tag_count, int total_count, int in_stock_count, int likes)
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
        for (int i = 0; i < tag_count; i++)	book->tags[i] = tags[i];
    }
    else    book->tags = NULL;
    // Move total_count & in_stock_count assignment out of if (tag_count >0) condition
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

void copyBooktoResult(Book* book, Book** result_list)
{
    Book* result = createBook(book->ISBN,book->title, book->author_id, book->tags, book->tag_count, \
    book->total_count, book->in_stock_count, book->likes);
    insertBook(result, result_list);
}

void printBooks(Book* head)
{
    Book* book = head;
    while (book){
        printf("ISBN number: %s\n", book->ISBN);
        printf("Title: %s\n", book->title);
        printf("Author ID: %i\n", book->author_id);
        printf("Book Status: %s\n", formatBookStatus(book->status));
        printf("Book Tags: ");
        // Add if/else condition to handle both cases
        if (book->tags)
        {
            for (int i = 0; i < book->tag_count; i++)
            {
                printf("%s ", formatBookTag(book->tags[i]));
            }
        }
        else    printf("none");

        printf("\n");
        printf("Total count: %i\nIn-stock count: %i\nLikes: %i\n", book->total_count, book->in_stock_count, book->likes);
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
    { // Use isContain instead of strstr() because isContain convert both to lowercase using toLowerCase before comparing
        if (isContain(current->ISBN, keyword) || isContain(current->title, keyword) || \
			isContain(toString(current->author_id), keyword) || \
			isContain(formatBookStatus(current->status), keyword))
        {
            copyBooktoResult(current, &search_result);
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
        if (isMatch(current->ISBN, input_ISBN))
            copyBooktoResult(current, &filtered_result);
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
        if (isMatch(current->title, input_title))
            copyBooktoResult(current, &filtered_result);
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
        if (current->total_count >= input_count) // change tag_count to total_count, change == to >=
            copyBooktoResult(current, &filtered_result);
        current = current->next;
    }
    return filtered_result;
}

// TODO: filterbyAuthorID, filterbyTags, filterbyinStockCount and filterbyLikes

Book* filterBooks(Book* parameters, Book* head){
    Book* filtered_books = head;

    if (parameters->ISBN) filtered_books = filterByISBN(parameters->ISBN, filtered_books);
    if (parameters->title) filtered_books = filterByTitle(parameters->title, filtered_books);
    if (parameters->total_count > 0) filtered_books = filterByTotalCount(parameters->total_count, filtered_books);
    return filtered_books;
}
//    NOTE: for fields like tag_count, author_id, instead of parameters->author_id check, we need parameters->author > 0

/* NOTE: There is an issue in filtering logic using insertBook(). When adding current to filtered_result, we carry over
the entire library list after matching a result. This results in an infinite loop in insertBook()
because we are updating the `next` of the last pointer to an item that exists in the list.*/