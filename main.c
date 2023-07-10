#include <stdio.h>
#include <stdlib.h>
#include "users.h"
#include "transactions.h"
#include "books.h"
#include "helpers.h"
#include "auth.h"

int main()
{
    printf("Welcome to the Library Management System!\n");
    enum book_tag tags[] = { fantasy, fiction };
    enum book_tag tags1[] = { fantasy};
    enum book_tag tags2[] = {fantasy, fiction, scifi};
    Book* new_book1 = createBook("1234", "Annie Bananie", 1, active, tags, 2, 10, 10, 0);
    Book* new_book2 = createBook("567", "Annie's world", 1, active, tags1, 1, 2, 2, 0);
    Book* new_book3 = createBook("8910", "Annie's Adventure", 1, active, tags2, 3, 5, 5, 0);
    insertBook(new_book1, &library);
    insertBook(new_book2, &library);
    insertBook(new_book3, &library);
    printf("***LIBRARY***\n");
    printBooks(library);
    Book* searched_books = searchBooks("Fantasy", library);
    printf("*****Searched Book Results*****\n");
    printBooks(searched_books);
    enum book_tag filter_tags[] = { fantasy, fiction};
    Book* filterBy = createBook(NULL, NULL, 0, no_book_status,filter_tags, 2, 0, 0, 0);
    Book* filtered_books = filterBooks(filterBy, library);
    printf("*****Filtered Book Results*****\n");
    printBooks(filtered_books);

    freeBookList(searched_books);
    freeBookList(filtered_books);

    User* new_user1 = createUser(1111, "Luke Skywalker", "luke.s", "luke", "luke.s@gmail.com", author, active_user);
    User* new_user2 = createUser(2222, "Obiwan Kenobi", "obiwan.k", "obiwan", "obiwan.k@gmail.com", customer, active_user);
    User* new_user3 = createUser(3333, "Darth Vader", "darth.v", "darth", "darth.v@gmail.com", librarian, active_user);
    insertUser(new_user1, &user_list_head);
    insertUser(new_user2, &user_list_head);
    insertUser(new_user3, &user_list_head);
    printf("***USERS***\n");
    printUser(user_list_head);
    User* searched_users = searchUsers("VADER", user_list_head);
    printf("*****Searched User Results*****\n");
    printUser(searched_users);
    User* user_parameters = createUser(-1, NULL, NULL, NULL, NULL, customer, active_user);
    User* filtered_users = filterUsers(user_parameters, user_list_head);
    printf("*****Filtered User Results*****\n");
    printUser(filtered_users);

    freeUserList(searched_users);
    freeUserList(filtered_users);

    BookCopy* copy1 = createBookCopy(01, "1234", active);
    BookCopy* copy2 = createBookCopy(02, "1234", active);
    BookCopy* copy3 = createBookCopy(03, "1234", active);
    BookCopy* copy4 = createBookCopy(04, "567", active);
    BookCopy* copy5 = createBookCopy(05, "567", active);
    BookCopy* copy6 = createBookCopy(06, "8910", active);
    insertBookCopy(copy1, &inventory);
    insertBookCopy(copy2, &inventory);
    insertBookCopy(copy3, &inventory);
    insertBookCopy(copy4, &inventory);
    insertBookCopy(copy5, &inventory);
    insertBookCopy(copy6, &inventory);
    printf("***INVENTORY***\n");
    printBookCopies(inventory);

    // Rent book cases:
    rentBook(new_book1, new_user1);
    rentBook(new_book2, new_user1);
    rentBook(new_book2, new_user2);
    rentBook(new_book2, new_user3);

    printf("***TRANSACTIONS***\n");
    printTransactions(transaction_list);

    printf("***Updated Library and Inventory after Renting***\n");
    printBooks(library);
    printBookCopies(inventory);

    returnBook(04);
    printf("***Updated Library and Inventory after Returning***\n");
    printBooks(library);
    printBookCopies(inventory);

    printf("***TRANSACTIONS***\n");
    printTransactions(transaction_list);

    BookTransaction* searched_trans = searchTransaction("Luke", transaction_list, user_list_head); // use name to find user id linked w trans
    printf("*****Searched Transaction Results*****\n");
    printTransactions(searched_trans);
    BookTransaction* trans_parameters = createTransaction(0, getTodaysDate(), 0, no_trans_status);
    BookTransaction* filtered_trans = filterTransactions(trans_parameters, transaction_list);
    printf("*****Filtered Transaction Results*****\n");
    printTransactions(filtered_trans);

    freeTransactionList(searched_trans);
    freeTransactionList(filtered_trans);

    char* login_name = getLoginName();
    char* password = getPassword();
    login(login_name, password);
    // Do something in between
    free(login_name);
    free(password);
    logout();

    return 0;
}