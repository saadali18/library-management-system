#include <stdio.h>
#include <stdlib.h>
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
    char* login_name;
    char* password;
    User* current_user;
    do {
        login_name = getLoginName();
        password = getPassword();
        current_user = login(login_name, password);
    } while (!current_user);

    if (current_user->title == librarian) {
        int category;
        printf("***** Librarian Portal *****\n");
        printf("1. BOOKS\t2. USERS\t3. TRANSACTIONS\t 4. LOG OUT\n");
        scanf("%i", &category);
        switch(category) {
            case 1: // BOOKS
                printf("1. View all books\t2. Search books by keywords\n"
                       "3. Search books by filters\t4. Delete book\n");
                int choice;
                scanf("%i", &choice);
                switch(choice) {
                    case 1:
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
                            scanf("%s", ISBN);
                        }
                        printf("Filtered by Title? ");
                        scanf("%i", &selection);
                        if (selection == 1) {
                            printf("Input Title: ");
                            scanf("%s", title);
                        }
                        printf("Filtered by Author's name? ");
                        scanf("%i", &selection);
                        if (selection == 1) {
                            printf("Input Author's name: ");
                            scanf("%s", author_name);
                            User *filtered_author = filterByFullName(author_name, user_list_head);
                            author_id = filtered_author->user_id;
                            free(author_name);
                        }
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
                        Book *filterBy = createBook(ISBN, title, author_id, status, tags, tag_count, total_count,
                                                    in_stock_count, likes);
                        Book *filtered_books = filterBooks(filterBy, library);
                        printBooks(filtered_books);
                        freeBookList(filtered_books);
                        break;
                    }
                    case 4:
                        printf("Enter ISBN of book to be deleted: ");
                        char* ISBN = malloc(20);
                        scanf("%s", ISBN);
                        // Delete source book
                        int deleted_book = deleteBook(ISBN);
                        if (deleted_book > 0)  printf("%i book deleted\n");
                        else    printf("No book found!\n");
                        // Delete all copies of source book
                        int deleted_book_copies = deleteBookCopies(ISBN);
                        if (deleted_book_copies > 0)    printf("%i book copies deleted\n");
                        else    printf("No book copies found!\n");
                        free(ISBN);
                        break;

                    default:
                        break;
                }
            case 2: // USERS
                break;
            case 3: // TRANSACTIONS
                break;
            case 4: // LOG OUT
                free(login_name);
                free(password);
                logout();
            default:
                break;
        }
    } // END OF LIBRARIAN

    /*// SAVE TO FILE BEFORE EXIT
    saveBooks(book_file, library);
    saveBookCopies(book_copy_file, inventory);
    saveUsers(user_file, user_list_head);
    saveTransactions(transaction_file, transaction_list);*/

    printBooks(library);
    printBookCopies(inventory);
    return 0;
}