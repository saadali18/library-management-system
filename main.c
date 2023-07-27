#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "users.h"
#include "transactions.h"
#include "books.h"
#include "helpers.h"
#include "auth.h"
#include "filemanager.h"

int main()
{
    // FILE MANAGEMENT
    readRecord(book_file, &book_record_head);
    loadBooks(book_record_head, &library);

    readRecord(book_copy_file, &book_copy_record_head);
    loadBookCopies(book_copy_record_head, &inventory);

    readRecord(user_file, &user_record_head);
    loadUsers(user_record_head, &user_list_head);

    readRecord(transaction_file, &transaction_record_head);
    loadTransactions(transaction_record_head, &transaction_list);

    printf("Welcome to the Library Management System!\n");
    // AUTHENTICATION
    char* log_in_name;
    char* password;
    int attempt = 0;
    User* current_user;
    do {
        log_in_name = getLoginName();
        password = getPassword();
        current_user = login(log_in_name, password);
        attempt++;
    } while (!current_user && attempt < 3);

    if (attempt == 3) {
        printf("Multiple failed login attempts detected. Please contact admin\n");
        exit(0);
    }
    // LIBRARIAN
    if (current_user->title == librarian) {
        int category;
        printf("\033[1m" "***** LIBRARIAN PORTAL *****\n" "\033[0m"); // Bold title
        printf("1. BOOKS\t2. USERS\t3. TRANSACTIONS\t 4. LOG OUT\n");
        scanf("%i", &category);
        switch(category) {
            case 1: // BOOKS
                printf("1. View all books\t2. Search books by keywords\n"
                       "3. Search books by filters\t4. Delete book\n");
                int book_choice;
                scanf("%i", &book_choice);
                switch(book_choice) {
                    case 1:
                        printf("\033[1m" "LIBRARY\n" "\033[0m");
                        printBooks(library);
                        break;
                    case 2:
                        printf("Enter search keyword: ");
                        char* keyword = malloc(100);
                        scanf(" %[^\n]", keyword);
                        Book* searched_books = searchBooks(keyword, library);
                        if (searched_books) {
                            printf("%i matching book(s) found!\n-----------\n", countBooks(searched_books));
                            printBooks(searched_books);
                        } else  printf("No matching book found\n");
                        freeBookList(searched_books);
                        free(keyword);
                        break;
                    case 3:
                        printf("0. No\t1. Yes\n");
                        char *ISBN = malloc(20);
                        char *title = malloc(100);
                        char *author_name = malloc(50);
                        enum book_status status = 0;
                        int author_id = 0, tag_count = 0, total_count = 0, in_stock_count = 0, likes = 0;
                        enum book_tag *tags = NULL;
                        int selection;
                        printf("Filtered by ISBN? ");
                        scanf("%i", &selection);
                        if (selection == 1) {
                            printf("Input ISBN: ");
                            scanf(" %[^\n]", ISBN);
                            printf("You entered: %s\n\n", ISBN);
                        } else  ISBN = NULL;
                        printf("Filtered by Title? ");
                        scanf("%i", &selection);
                        if (selection == 1) {
                            printf("Input Title: ");
                            scanf(" %[^\n]", title);
                            printf("You entered: %s\n\n", title);
                        } else  title = NULL;
                        printf("Filtered by Author's name? ");
                        scanf("%i", &selection);
                        if (selection == 1) {
                            printf("Input Author's name: ");
                            scanf(" %[^\n]", author_name);
                            printf("You entered: %s\n\n", author_name);
                            User *filtered_author = filterByFullName(author_name, user_list_head);
                            author_id = filtered_author->user_id;
                            free(author_name);
                        } else author_id = 0;
                        printf("Filtered by Book Status? ");
                        scanf("%i", &selection);
                        if (selection == 1) {
                            printf("1. Inactive\t2. Active\t3. Deleted\n");
                            scanf("%i", &status);
                        }
                        printf("Filtered by Book Tags? ");
                        scanf("%i", &selection);
                        if (selection == 1) {
                            printf("How many tags: ");
                            scanf("%i", &tag_count);
                            tags = malloc(tag_count * sizeof(enum book_tag));
                            for (int i = 0; i < tag_count; i++) {
                                printf("1. biography\t2. fantasy\t3. fiction\t4. mystery\n"
                                       "5. romance\t6. scifi\t7. thriller\t8. young_adult\n");
                                printf("Tag #%i: ", i + 1);
                                scanf("%i", &tags[i]);
                            }
                        }
                        printf("Filtered by Total Count? ");
                        scanf("%i", &selection);
                        if (selection == 1) {
                            printf("Input count: ");
                            scanf("%i", &total_count);
                        }
                        printf("Filtered by In-Stock Count? ");
                        scanf("%i", &selection);
                        if (selection == 1) {
                            printf("Input count: ");
                            scanf("%i", &in_stock_count);
                        }
                        printf("Filtered by Likes? ");
                        scanf("%i", &selection);
                        if (selection == 1) {
                            printf("Input likes: ");
                            scanf("%i", &likes);
                        }
                        Book *filterBy = createBook(ISBN, title, author_id, status, tags, tag_count, total_count, in_stock_count, likes);
                        Book *filtered_books = filterBooks(filterBy, library);
                        if (filtered_books)
                        {
                            printf("%i matching book(s) found!\n-----------\n", countBooks(filtered_books));
                            printBooks(filtered_books);
                        }
                        else    printf("No matching book found\n");
                        freeBookList(filtered_books);
                        break;
                    case 4:
                        printf("Enter ISBN of book to be deleted: ");
                        char* ISBN_to_delete = malloc(20);
                        scanf("%s", ISBN_to_delete);
                        // Delete source book
                        int deleted_book = deleteBook(ISBN_to_delete);
                        if (deleted_book > 0)  printf("%i book deleted\n", deleted_book);
                        else    printf("No book found!\n");
                        // Delete all copies of source book
                        int deleted_book_copies = deleteBookCopies(ISBN_to_delete);
                        if (deleted_book_copies > 0)    printf("%i book copies deleted\n", deleted_book_copies);
                        else    printf("No book copies found!\n");
                        free(ISBN_to_delete);
                        break;
                    default:
                        break;
                }
                break; // END OF BOOKS
            case 2: // USERS
                printf("1. View all authors\t2. View all customers\n"
                       "3. Search users by keyword\t4. Search users by filters\n"
                       "5. Create new user\t6. Delete user\n");
                int user_choice;
                scanf("%i", &user_choice);
                switch(user_choice) {
                    case 1:
                        printf("\033[1m" "AUTHOR LIST\n" "\033[0m");
                        User* authors = filterByUserTitle(author, user_list_head);
                        if (authors) {
                            printf("%i authors found\n-----------\n", countUsers(authors));
                            printUser(authors);
                        }
                        else    printf("No author found!\n");
                        break;
                    case 2:
                        printf("\033[1m" "CUSTOMER LIST\n" "\033[0m");
                        User* customers = filterByUserTitle(customer, user_list_head);
                        if (customers) {
                            printf("%i customers found\n-----------\n", countUsers(customers));
                            printUser(customers);
                        }
                        else    printf("No customer found!\n");
                        break;
                    case 3:
                        printf("Enter search keyword: ");
                        char* keyword = malloc(100);
                        scanf(" %[^\n]", keyword);
                        printf("You have entered: %s\n", keyword);
                        User* searched_users = searchUsers(keyword, user_list_head);
                        if (searched_users) {
                            printf("%i user(s) found\n", countUsers(searched_users));
                            printUser(searched_users);
                        }
                        else    printf("No user found!\n");
                        free(keyword);
                        break;
                    case 4:
                        printf("0. No\t1. Yes\n");
                        int user_id = 0;
                        char* full_name = malloc(100);
                        char* login_name = malloc(50);
                        char* email = malloc(100);
                        enum credential title = 0;
                        enum user_status status = 0;
                        int selection;
                        printf("Filter by user ID? ");
                        scanf("%i", &selection);
                        if (selection == 1) {
                            printf("Input user ID: ");
                            scanf("%i", &user_id);
                            printf("You have entered: %i\n", user_id);
                        }
                        printf("Filter by full name? ");
                        scanf("%i", &selection);
                        if (selection == 1) {
                            printf("Input user's full name: ");
                            scanf(" %[^\n]", full_name);
                            printf("You have entered: %s\n", full_name);
                        } else  full_name = NULL;
                        printf("Filter by login name? ");
                        scanf("%i", &selection);
                        if (selection == 1) {
                            printf("Input user's login name: ");
                            scanf(" %[^\n]", login_name);
                            printf("You have entered: %s\n", login_name);
                        } else  login_name = NULL;
                        printf("Filter by email? ");
                        scanf("%i", &selection);
                        if (selection == 1) {
                            printf("Input user's email: ");
                            scanf(" %[^\n]", email);
                            printf("You have entered: %s\n", email);
                        } else  email = NULL;
                        printf("Filter by title? ");
                        scanf("%i", &selection);
                        if (selection == 1) {
                            printf("1. Author\t2. Customer\t3. Librarian\n");
                            printf("Input user's title: ");
                            scanf("%i", &title);
                            printf("You have entered: %i\n", title);
                        }
                        printf("Filter by status? ");
                        scanf("%i", &selection);
                        if (selection == 1) {
                            printf("1. Inactive\t2. Active\t3. Delinquent\t4. Deleted\n");
                            printf("Input user's title: ");
                            scanf("%i", &status);
                            printf("You have entered: %i\n", status);
                        }
                        User* filterBy = createUser(user_id, full_name, login_name, NULL, email, title, status);
                        printUser(filterBy);
                        User* filtered_users = filterUsers(filterBy, user_list_head);
                        if (filtered_users) {
                            printf("%i user(s) found\n", countUsers(filtered_users));
                            printUser(filtered_users);
                        }
                        else    printf("No user found!\n");
                        freeUserList(filtered_users);
                        break;
                    case 5:
                        printf("Enter new user's information\n");
                        int new_user_id = 0;
                        char* new_full_name = malloc(100);
                        char* new_login_name = malloc(50);
                        char* new_email = malloc(100);
                        char* new_password = malloc(50);
                        enum credential new_title = 0;
                        printf("Enter user ID: ");
                        scanf("%i", &new_user_id);
                        printf("Enter user's full name: ");
                        scanf(" %[^\n]", new_full_name);
                        printf("Enter user's login name: ");
                        scanf(" %[^\n]", new_login_name);
                        printf("Enter user's email: ");
                        scanf(" %[^\n]", new_email);
                        printf("Enter user's password: ");
                        scanf(" %[^\n]", new_password);
                        printf("1. Author\t2. Customer\t3. Librarian\n"
                               "Enter user's title: ");
                        scanf("%i", &new_title);
                        User* new_user = createUser(new_user_id, new_full_name, new_login_name, new_password, new_email, new_title, active_user);
                        insertUser(new_user, &user_list_head);
                        printf("New user added!\n");
                        printUser(new_user);
                        break;
                    case 6:
                        printf("Enter user ID to be deleted: ");
                        int id = 0;
                        scanf("%i", &id);
                        int deleted_user = deleteUser(id);
                        if (deleted_user > 0)  printf("%i user deleted!\n", deleted_user);
                        else    printf("No user found!\n");
                        break;
                }
                break; // END OF USERS
            case 3: // TRANSACTIONS
                printf("1. View all transactions\t2. Search transactions by keyword\n"
                       "3. Search transactions by filters\n");
                int transaction_choice;
                scanf("%i", &transaction_choice);
                switch (transaction_choice) {
                    case 1:
                        printf("\033[1m" "TRANSACTION LIST\n" "\033[0m");
                        printTransactions(transaction_list);
                        break;
                    case 2:
                        printf("Enter search keyword: ");
                        char* keyword = malloc(100);
                        scanf(" %[^\n]", keyword);
                        BookTransaction* searched_transactions = searchTransaction(keyword, transaction_list, user_list_head);
                        if (searched_transactions) {
                            printf("%i transaction(s) found!\n-----------------\n", countTransactions(searched_transactions));
                            printTransactions(searched_transactions);
                        } else  printf("No transaction found!\n");
                        free(keyword);
                        freeTransactionList(searched_transactions);
                        break;
                    case 3:
                        printf("0. No\t1. Yes\n");
                        int book_uid = 0;
                        Date* check_out_date = malloc(sizeof(Date));
                        Date* due_date = malloc(sizeof(Date));
                        Date* return_date = malloc(sizeof(Date));
                        int user_id = 0;
                        enum transaction_status status = 0;
                        int selection;
                        printf("Filter by book unique ID? ");
                        scanf("%i", &selection);
                        if (selection == 1) {
                            printf("Enter book unique ID: ");
                            scanf("%i", &book_uid);
                            printf("You enter: %i\n", book_uid);
                        }
                        printf("Filter by transaction status? ");
                        scanf("%i", &selection);
                        printf("Filter by Check Out Date? ");
                        scanf("%i", &selection);
                        if (selection == 1) {
                            printf(" Enter month: ");
                            scanf("%i", &check_out_date->month);
                            printf(" Enter day: ");
                            scanf("%i", &check_out_date->day);
                            printf(" Enter year: ");
                            scanf("%i", &check_out_date->year);
                            printf("You enter: %i/%i/%i\n", check_out_date->month, check_out_date->day, check_out_date->year);
                        } else  check_out_date = NULL;
                        printf("Filter by Due Date? ");
                        scanf("%i", &selection);
                        if (selection == 1) {
                            printf(" Enter month: ");
                            scanf("%i", &due_date->month);
                            printf(" Enter day: ");
                            scanf("%i", &due_date->day);
                            printf(" Enter year: ");
                            scanf("%i", &due_date->year);
                            printf("You enter: %i/%i/%i\n", due_date->month, due_date->day, due_date->year);
                        } else  due_date = NULL;
                        printf("Filter by Return Date? ");
                        scanf("%i", &selection);
                        if (selection == 1) {
                            printf(" Enter month: ");
                            scanf("%i", &return_date->month);
                            printf(" Enter day: ");
                            scanf("%i", &return_date->day);
                            printf(" Enter year: ");
                            scanf("%i", &return_date->year);
                            printf("You enter: %i/%i/%i\n", return_date->month, return_date->day, return_date->year);
                        } else  return_date = NULL;
                        printf("Filter by user ID? ");
                        scanf("%i", &selection);
                        if (selection == 1) {
                            printf("Enter user ID: ");
                            scanf("%i", &user_id);
                            printf("You enter: %i\n", user_id);
                        }
                        if (selection == 1) {
                            printf("1. Open\t2. Close\t3. Past due\nEnter transaction status: ");
                            scanf("%i", &status);
                            printf("You enter: %i\n", status);
                        }
                        BookTransaction* filterBy = createTransaction(book_uid, check_out_date, due_date, return_date, user_id, status);
                        printTransactions(filterBy);
                        BookTransaction* filtered_transactions = filterTransactions(filterBy, transaction_list);
                        if (filtered_transactions) {
                            printf("%i transaction(s) found\n---------------------\n", countTransactions(filtered_transactions));
                            printTransactions(filtered_transactions);
                        }
                        else    printf("No transaction found!\n");
                        freeTransactionList(filtered_transactions);
                        break;
                }
                break; // END OF TRANSACTIONS
            case 4: // LOG OUT
                free(log_in_name);
                free(password);
                logout();
            default:
                break;
        } // END OF SWITCH CATEGORY
    } // END OF LIBRARIAN

    // SAVE TO FILE BEFORE EXIT
    saveBooks(book_file, library);
    saveBookCopies(book_copy_file, inventory);
    saveUsers(user_file, user_list_head);
    saveTransactions(transaction_file, transaction_list);

    return 0;
}