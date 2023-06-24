//
// Created by Saad Ali on 24/06/2023.
//

#ifndef LMS_PRACTICE_ANNIE_CODE_BOOKS_H
#define LMS_PRACTICE_ANNIE_CODE_BOOKS_H

enum book_status { inactive, active };

enum book_tag { biography, fantasy, fiction, mystery, romance, scifi, thriller, young_adult };

typedef struct Book
{
    char* ISBN;
    char* title;
    int author_id;
    enum book_status status;
    enum book_tag* tags;
    int tag_count;
    int total_count;
    int in_stock_count;
    int likes;
    struct Book* next;
} Book;

typedef struct BookCopy
{
    int book_uid; // Unique ID of each copy of same ISBN (original book)
    char* ISBN;
    enum book_status status;
    struct BookCopy* next;
} BookCopy;

extern Book* library;

Book* createBook(char* ISBN, char* title, int author_id, enum book_tag* tags, int tag_count, int total_count);
void insertBook(Book* book, Book** head);
void printBooks(Book* book);
Book* searchBooks(char* keyword, Book* head);
Book* filterBooks(Book* parameters, Book* head);

#endif //LMS_PRACTICE_ANNIE_CODE_BOOKS_H
