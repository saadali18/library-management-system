#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helpers.h"
#include "books.h"


typedef struct Record {
    char* data;
    struct Record* next;
}Record;

Record* record_head = NULL;

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

char* book_file = "C:\\Users\\annie\\CLionProjects\\Library-Management-System\\library-management-system\\books.txt";

void readRecord(char* file_path)
{
    FILE* file = fopen(file_path, "r");
    if (!file) {
        printf("Could not open file.\n");
        return;
    }
    char text[256];
    while (fgets(text, sizeof(text), file))
        insertRecord(text, &record_head);
    fclose(file);
}

void printRecord(Record* head)
{
    Record* current = head;
    while (current) {
        printf("%s", current->data);
        current = current->next;
    }
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

void insertParsedBookRecords(Record* book_record, Book** book_head)
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
    char* tags = strsep(&book_record->data, ",");
    book->tag_count = atoi(strsep(&book_record->data, ","));
    if (book->tag_count > 0) {
        book->tags = malloc(book->tag_count * sizeof(enum book_tag));
        for (int i = 0; i < book->tag_count; i++) {
            char *tag = strsep(&tags, "-");
            book->tags[i] = atoi(tag);
        }
    }
    book->total_count = atoi(strsep(&book_record->data, ","));
    book->in_stock_count = atoi(strsep(&book_record->data, ","));
    book->likes = atoi(strsep(&book_record->data, ","));
    book->next = NULL;

    // Add the parsed book record to end of library
    if (*book_head) {
        Book* current = *book_head;
        while (current->next)
            current = current->next;
        current->next = book;
    }
    else {
        *book_head = book;
    }
}

void loadBooks()
{
    Record* current = record_head;
    while (current) {
        insertParsedBookRecords(current, &library);
        current = current->next;
    }
    printf("Books are successfully inputted!\n");
    freeRecordList(record_head); // free list of plain text after loading into linked list
}

void saveBooks()
{
    FILE* file = fopen(book_file, "w");
    if (!file) {
        printf("Could not open file.\n");
        return;
    }
    Book* current = library;
    while (current) {
        fprintf(file, "%s,%s,%i,%i,", current->ISBN, current->title, current->author_id, current->status);
        // Handle tags
        if (current->tag_count > 0) {
            for (int i = 0; i < current->tag_count; i++) {
                fprintf(file, "%i", current->tags[i]);
                if (i != (current->tag_count - 1))  fprintf(file, "-");
            }
            fprintf(file, ",%i", current->tag_count);
        }
        else fprintf(file, ",0");

        fprintf(file, ",%i,%i,%i\n", current->total_count, current->in_stock_count, current->likes);
        current = current->next;
    }
    fclose(file);
}


