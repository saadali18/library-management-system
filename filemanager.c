#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helpers.h"
#include "books.h"
#include "users.h"
#include "transactions.h"


typedef struct Record {
    char* data;
    struct Record* next;
}Record;

Record* book_record_head = NULL;
Record* user_record_head = NULL;
Record* book_copy_record_head = NULL;
Record* transaction_record_head = NULL;

const char* book_file = "C:\\Users\\annie\\CLionProjects\\Library-Management-System\\library-management-system\\books.txt";
const char* user_file = "C:\\Users\\annie\\CLionProjects\\Library-Management-System\\library-management-system\\users.txt";
const char* book_copy_file = "C:\\Users\\annie\\CLionProjects\\Library-Management-System\\library-management-system\\bookcopies.txt";
const char* transaction_file = "C:\\Users\\annie\\CLionProjects\\Library-Management-System\\library-management-system\\transactions.txt";

void insertRecord(char* text, Record** head)
{
    Record* new_record = malloc(sizeof(Record));
    if (!new_record) {
        printf("Failed to allocate memory for text record\n");
        return;
    }
    new_record->data = strdup(text);
    new_record->next = NULL;

    if (*head)   {
        Record* current = *head;
        while (current->next) {
            current = current->next;
        }
        current->next = new_record;
    } else {
        *head = new_record;
    }
}

void readRecord(const char* file_path, Record** record_head)
{
    FILE* file = fopen(file_path, "r");
    if (!file) {
        printf("Could not open file.\n");
        return;
    }
    char text[256];
    fgets(text, sizeof(text), file);
    while (fgets(text, sizeof(text), file))
        insertRecord(text, record_head);
    fclose(file);
}

void printRecord(Record* head) // Testing purpose
{
    Record* current = head;
    while (current) {
        printf("%s", current->data);
        current = current->next;
    }
    printf("\n-------------------------\n");
}

void freeRecordList(Record* head)
{
    while (head)
    {
        Book* temp = head;
        head = head->next;
        free(temp);
    }
}

// ************** BOOKS ***************
void insertParsedBookRecord(Record* book_record, Book** book_head)
{
    Book* book = malloc(sizeof(Book));
    if (!book) {
        printf("Failed to allocate memory for book\n");
        return;
    }
    book->ISBN = strdup(strsep(&book_record->data, ","));
    book->title = strdup(strsep(&book_record->data, ","));
    book->author_id = atoi(strsep(&book_record->data, ","));
    book->status = atoi(strsep(&book_record->data, ","));
    // Handle tags field
    book->tag_count = atoi(strsep(&book_record->data, ","));
    char* tags = strsep(&book_record->data, ",");
    if (book->tag_count > 0) {
        book->tags = malloc(book->tag_count * sizeof(enum book_tag));
        for (int i = 0; i < book->tag_count; i++) {
            char *tag = strsep(&tags, "-");
            book->tags[i] = atoi(tag);
        }
    } else  book->tags = NULL;
    book->total_count = atoi(strsep(&book_record->data, ","));
    book->in_stock_count = atoi(strsep(&book_record->data, ","));
    book->likes = atoi(book_record->data);
    book->next = NULL;

    // Add the parsed book record to end of library
    if (*book_head) {
        Book* current = *book_head;
        while (current->next)
            current = current->next;
        current->next = book;
    } else {
        *book_head = book;
    }
}

void loadBooks(Record* record_head, Book** book_head)
{
    int book_count = 0;
    Record* current = record_head;
    while (current) {
        insertParsedBookRecord(current, book_head);
        book_count++;
        current = current->next;
    }
    if (book_count > 0)  printf("Books are successfully inputted!\n");
    freeRecordList(record_head); // free list of plain text after loading into linked list
}

void saveBooks(const char* file_name, Book* book_head)
{
    FILE* file = fopen(file_name, "w");
    if (!file) {
        printf("Could not open file.\n");
        return;
    }
    fprintf(file,"ISBN,Title,AuthorID,Status,TagCount,BookTags,TotalCount,InStockCount,Likes\n");
    Book* current = book_head;
    while (current) {
        fprintf(file, "%s,%s,%04i,%i,", current->ISBN, current->title, current->author_id, current->status);
        // Handle tags
        if (current->tag_count > 0) {
            fprintf(file, "%i,", current->tag_count);
            for (int i = 0; i < current->tag_count; i++) {
                fprintf(file, "%i", current->tags[i]);
                if (i != (current->tag_count - 1))  fprintf(file, "-");
            }
        }
        else fprintf(file, "0,");

        fprintf(file, ",%i,%i,%i\n", current->total_count, current->in_stock_count, current->likes);
        current = current->next;
    }
    fclose(file);
}

// ************** BOOK COPIES ***************
void insertParsedBookCopyRecord(Record* book_copy_record, BookCopy** book_copy_head)
{
    BookCopy* book_copy = malloc(sizeof(BookCopy));
    if (!book_copy) {
        printf("Failed to allocate memory for book copy\n");
        return;
    }
    book_copy->book_uid = atoi(strsep(&book_copy_record->data, ","));
    book_copy->ISBN = strsep(&book_copy_record->data, ",");
    book_copy->status = atoi(book_copy_record->data);
    book_copy->next = NULL;

    if (*book_copy_head) {
        BookCopy* current = *book_copy_head;
        while (current->next)
            current = current->next;
        current->next = book_copy;
    } else {
        *book_copy_head = book_copy;
    }
}

void loadBookCopies(Record* record_head, BookCopy** book_copy_head)
{
    int book_copy_count = 0;
    Record* current = record_head;
    while (current) {
        insertParsedBookCopyRecord(current, book_copy_head);
        book_copy_count++;
        current = current->next;
    }
    if (book_copy_count > 0)  printf("Book copies are successfully inputted!\n");
    freeRecordList(record_head);
}

void saveBookCopies(const char* file_name, BookCopy* book_copy_head)
{
    FILE* file = fopen(file_name, "w");
    if (!file) {
        printf("Could not open file\n");
        return;
    }
    fprintf(file, "BookUID,ISBN,Status\n");
    BookCopy* current = book_copy_head;
    while (current) {
        fprintf(file, "%04i,%s,%i\n", current->book_uid, current->ISBN, current->status);
        current = current->next;
    }
    fclose(file);
}

// ************** USERS ***************
void insertParsedUserRecord(Record* user_record, User** user_head)
{
    User* user = malloc(sizeof(User));
    if (!user) {
        printf("Failed to allocate memory for user\n");
        return;
    }
    user->user_id = atoi(strsep(&user_record->data, ","));
    user->full_name = strsep(&user_record->data, ",");
    user->login_name = strsep(&user_record->data, ",");
    user->password = strsep(&user_record->data, ",");
    user->email = strsep(&user_record->data, ",");
    user->title = atoi(strsep(&user_record->data, ","));
    user->status = atoi(user_record->data);
    user->next = NULL;

    if(*user_head) {
        User* current = *user_head;
        while (current->next)
            current = current->next;
        current->next = user;
    } else {
        *user_head = user;
    }
}

void loadUsers(Record* record_head, User** user_head)
{
    int user_count = 0;
    Record* current = record_head;
    while (current) {
        insertParsedUserRecord(current, user_head);
        user_count++;
        current = current->next;
    }
    if (user_count > 0) printf("Users are successfully inputted!\n");
    freeRecordList(record_head);
}

void saveUsers(const char* file_name, User* user_head)
{
    FILE* file = fopen(file_name, "w");
    if (!file) {
        printf("Could not open file\n");
        return;
    }
    fprintf(file, "UserID,FullName,LogInName,Password,Email,Title,Status\n");
    User* current = user_head;
    while (current) {
        fprintf(file, "%04i,%s,%s,%s,%s,%i,%i\n", current->user_id, current->full_name, current->login_name,
                current->password, current->email, current->title, current->status);
        current = current->next;
    }
    fclose(file);
}

// ************** TRANSACTIONS ***************
Date* parseDateString(char* date_string)
{
    Date* date = malloc(sizeof(Date));
    if (!date) {
        printf("Failed to allocate memory for transaction\n");
        return NULL;
    }
    date->month = atoi(strsep(&date_string, "/"));
    date->day = atoi(strsep(&date_string, "/"));
    date->year = atoi(date_string);
    return date;
}

void insertParsedTransactionRecord(Record* transaction_record, BookTransaction** transaction_head)
{
    BookTransaction* transaction = malloc(sizeof(BookTransaction));
    if (!transaction) {
        printf("Failed to allocate memory for transaction\n");
        return;
    }
    transaction->book_uid = atoi(strsep(&transaction_record->data, ","));
    transaction->check_out_date = parseDateString(strsep(&transaction_record->data, ","));
    transaction->due_date = parseDateString(strsep(&transaction_record->data, ","));
    char* return_date = strsep(&transaction_record->data, ",");
    if (!strcmp(return_date, "\000")) transaction->return_date = NULL;
    else    transaction->return_date = parseDateString(return_date);
    transaction->user_id = atoi(strsep(&transaction_record->data, ","));
    transaction->status = atoi(transaction_record->data);
    transaction->next = NULL;

    if (*transaction_head) {
        BookTransaction* current = *transaction_head;
        while (current->next)
            current = current->next;
        current->next = transaction;
    } else {
        *transaction_head = transaction;
    }
}

void loadTransactions(Record* record_head, BookTransaction** transaction_head)
{
    int transaction_count = 0;
    Record* current = record_head;
    while (current) {
        insertParsedTransactionRecord(current, transaction_head);
        transaction_count++;
        current = current->next;
    }
    if (transaction_count > 0)  printf("Transactions are successfully inputted!\n");
    freeRecordList(record_head);
}

void saveTransactions(const char* file_name, BookTransaction* transaction_head)
{
    FILE* file = fopen(file_name, "w");
    if (!file) {
        printf("Could not open file\n");
        return;
    }
    fprintf(file, "BookUID,CheckOutDate,DueDate,ReturnDate,UserID,Status\n");
    BookTransaction* current = transaction_head;
    while (current) {
        fprintf(file, "%i,", current->book_uid);
        fprintf(file, "%i/%i/%i,", current->check_out_date->month, current->check_out_date->day, current->check_out_date->year);
        fprintf(file, "%i/%i/%i,", current->due_date->month, current->due_date->day, current->due_date->year);
        if (current->return_date)
            fprintf(file, "%i/%i/%i,", current->return_date->month, current->return_date->day, current->return_date->year);
        else    fprintf(file, ",");
        fprintf(file, "%i,%i\n", current->user_id, current->status);
        current = current->next;
    }
    fclose(file);
}