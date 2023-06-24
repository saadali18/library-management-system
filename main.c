#include <stdio.h>
#include <string.h>
#include "users.h"
#include "transactions.h"
#include "books.h"

int main()
{
    printf("Welcome to the Library Management System!\n");
    enum book_tag tags[] = { fantasy, fiction };
    enum book_tag tags1[] = { fantasy};
    Book* new_book1 = createBook("1234", "Annie Bananie", 1, tags, 2, 10);
    Book* new_book2 = createBook("567", "Annie's world", 1, tags1, 3, 10);
    insertBook(new_book1, &library);
    insertBook(new_book2, &library);
    printBooks(library);
    Book* searched_books = searchBooks("world", library);  // Assuming only 2nd book will be filtered out
    printf("*****Searched Results*****\n");
    printBooks(searched_books);
    Book* filterBy = createBook(NULL, NULL, -1, NULL, -1, 10);
    Book* filtered_books = filterBooks(filterBy, library);  // Expecting two records
    printf("*****Filtered Results*****\n");
    printBooks(filtered_books);


    return 0;
}