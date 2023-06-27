#include <stdio.h>
#include <string.h>
#include "users.h"
#include "transactions.h"
#include "books.h"
#include "helpers.h"

int main()
{
    printf("Welcome to the Library Management System!\n");
    enum book_tag tags[] = { fantasy, fiction };
    enum book_tag tags1[] = { fantasy};
    Book* new_book1 = createBook("1234", "Annie Bananie", 1, tags, 2, 10, 10, 0);
    Book* new_book2 = createBook("567", "Annie's world", 1, tags1, 1, 5, 5, 0); // change tag count from 3 to 1
    Book* new_book3 = createBook("8919", "Annie's Adventure", 1, NULL, 0, 2, 2, 0);
    insertBook(new_book1, &library);
    insertBook(new_book2, &library);
    insertBook(new_book3, &library);
    printBooks(library);
    Book* searched_books = searchBooks("ADVENTURE", library); // should be able to handle upper/lower cases or mixed
    printf("*****Searched Results*****\n");
    printBooks(searched_books);
    Book* filterBy = createBook(NULL, "Annie's WORLD", -1, NULL, -1, 5, -1, -1);
    Book* filtered_books = filterBooks(filterBy, library);
    printf("*****Filtered Results*****\n");
    printBooks(filtered_books);

    return 0;
}