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
    enum book_tag tags2[] = {fantasy, fiction, scifi};
    Book* new_book1 = createBook("1234", "Annie Bananie", 1, active, tags, 2, 10, 10, 0);
    Book* new_book2 = createBook("567", "Annie's world", 1, active, tags1, 1, 5, 5, 0);
    Book* new_book3 = createBook("8919", "Annie's Adventure", 1, active, tags2, 3, 2, 2, 0);
    insertBook(new_book1, &library);
    insertBook(new_book2, &library);
    insertBook(new_book3, &library);
    printf("***LIBRARY***\n");
    printBooks(library);
    Book* searched_books = searchBooks("Fantasy", library);
    printf("*****Searched Results*****\n");
    printBooks(searched_books);
    enum book_tag filter_tags[] = { fantasy, fiction};
    Book* filterBy = createBook(NULL, NULL, 0, no_book_status,filter_tags, 2, 0, 0, 0);
    Book* filtered_books = filterBooks(filterBy, library);
    printf("*****Filtered Results*****\n");
    printBooks(filtered_books);

    User* new_user1 = createUser(1111, "Luke Skywalker", "luke.s", "luke", "luke.s@gmail.com", author, active_user);
    User* new_user2 = createUser(2222, "Obiwan Kenobi", "obiwan.k", "obiwan", "obiwan.k@gmail.com", customer, active_user);
    User* new_user3 = createUser(3333, "Darth Vader", "darth.v", "darth", "darth.v@gmail.com", librarian, active_user);
    insertUser(new_user1, &user_list_head);
    insertUser(new_user2, &user_list_head);
    insertUser(new_user3, &user_list_head);
    printf("***USERS***\n");
    printUser(user_list_head);
    User* searched_users = searchUsers("VADER", user_list_head);
    printf("*****Searched Results*****\n");
    printUser(searched_users);
    User* user_parameters = createUser(-1, NULL, NULL, NULL, NULL, customer, active_user);
    User* filtered_users = filterUsers(user_parameters, user_list_head);
    printf("*****Filtered Results*****\n");
    printUser(filtered_users);

    BookCopy* copy1 = createBookCopy(01, "1234", active);
    BookCopy* copy2 = createBookCopy(02, "1234", active);
    BookCopy* copy3 = createBookCopy(03, "1234", active);
    insertBookCopy(copy1, &inventory);
    insertBookCopy(copy2, &inventory);
    insertBookCopy(copy3, &inventory);
    printf("***INVENTORY***\n");
    printBookCopies(inventory);

    Date date1 = {2023, 6, 29};
    Date date2 = {2023, 2, 06};
    Date date3 = {2023, 8, 11};
    BookTransaction* rental1 = createTransaction(01, &date1, 1111, open);
    BookTransaction* rental2 = createTransaction(02, &date2, 2222, open);
    BookTransaction* rental3 = createTransaction(03, &date3, 3333, open);
    insertTransaction(rental1, &transaction_list);
    insertTransaction(rental2, &transaction_list);
    insertTransaction(rental3, &transaction_list);
    printf("***TRANSACTIONS***\n");
    printTransactions(transaction_list);
    BookTransaction* searched_trans = searchTransaction("Luke", transaction_list, user_list_head); // use name to find user id linked w trans
    printf("*****Searched Results*****\n");
    printTransactions(searched_trans);
    BookTransaction* trans_parameters = createTransaction(0, &date1, 0, no_trans_status);
    BookTransaction* filtered_trans = filterTransactions(trans_parameters, transaction_list);
    printf("*****Filtered Results*****\n");
    printTransactions(filtered_trans);

    rentBook(rental1, library, inventory);
    printBooks(library);
    printBookCopies(inventory);
    return 0;
}