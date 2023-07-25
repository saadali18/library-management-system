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

/*    char* keyword = malloc(50 * sizeof(char));
    printf("Enter keyword: ");
    fgets(keyword, 50, stdin);
    size_t len = strlen(keyword);
    if (len > 0 && keyword[len - 1] == '\n')  keyword[len - 1] = '\0';

    Book* searched_result = searchBooks(keyword, library);
    if (!searched_result)   printf("No result found\n");
    else {
        printf("%i book(s) found\n---------\n", countBooks(searched_result));
        printBooks(searched_result);
    }

    Book* filterBy = createBook(NULL, NULL, 1234, 0, NULL, 0, 0, 0, 0);
    Book* filtered_result = filterBooks(filterBy, library);
    if (!filtered_result)   printf("No result found\n");
    else {
        printf("%i book(s) found\n---------\n", countBooks(filtered_result));
        printBooks(filtered_result);
    }*/



    char* login_name;
    char* password;
    int attempt = 0;
    User* current_user;
    do {
        login_name = getLoginName();
        password = getPassword();
        current_user = login(login_name, password);
        attempt++;
    } while (!current_user && attempt < 3);

    if (attempt == 3) {
        printf("Multiple failed login attempts detected. Please contact admin\n");
        exit(0);
    }

    if (current_user->title == librarian) {
        int category;
        printf("***** Librarian Portal *****\n");
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
                        printf("\033[1m" "LIBRARY\n" "\033[0m"); // Bold title
                        printBooks(library);
                        break;
                    case 2:
                        printf("Input your keyword: ");
                        char* keyword = malloc(100);
                        scanf("%s", keyword);
                        Book* searched_books = searchBooks(keyword, library);
                        printBooks(searched_books);
                        freeBookList(searched_books);
                        free(keyword);
                        break;
                    case 3: {
                        char *ISBN = malloc(20);
                        char *title = malloc(100);
                        char *author_name = malloc(50);
                        enum book_status status = 0;
                        int author_id = 0, tag_count = 0, total_count = 0, in_stock_count = 0, likes = 0;
                        enum book_tag *tags = NULL;
                        int selection;
                        printf("Select 0 for No\t Select 1 for Yes\n");
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
                            printf("Select 1 for inactive\tSelect 2 for active\n");
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
                    } // END OF CHOICE 3
                    case 4:
                        printf("Enter ISBN of book to be deleted: ");
                        char* ISBN = malloc(20);
                        scanf("%s", ISBN);
                        // Delete source book
                        int deleted_book = deleteBook(ISBN);
                        if (deleted_book > 0)  printf("%i book deleted\n", deleted_book);
                        else    printf("No book found!\n");
                        // Delete all copies of source book
                        int deleted_book_copies = deleteBookCopies(ISBN);
                        if (deleted_book_copies > 0)    printf("%i book copies deleted\n", deleted_book_copies);
                        else    printf("No book copies found!\n");
                        free(ISBN);
                        break;
                    default:
                        break;
                }
                break; // END OF BOOKS
            case 2: // USERS
                break; // END OF USERS
            case 3: // TRANSACTIONS
                break; // END OF TRANSACTIONS
            case 4: // LOG OUT
                free(login_name);
                free(password);
                logout();
            default:
                break;
        } // END OF SWITCH CATEGORY
    } // END OF LIBRARIAN

    /*// SAVE TO FILE BEFORE EXIT
    saveBooks(book_file, library);
    saveBookCopies(book_copy_file, inventory);
    saveUsers(user_file, user_list_head);
    saveTransactions(transaction_file, transaction_list);*/

    /*printBooks(library);
    printBookCopies(inventory);*/
    return 0;
}