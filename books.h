#ifndef BOOKS_H
#define BOOKS_H

enum book_status { no_book_status, inactive, active, deleted, sold }; // add deleted status for soft delete

enum book_tag { no_tag, biography, fantasy, fiction, mystery, romance, scifi, thriller, young_adult };

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
    int book_uid;
    char* ISBN;
    enum book_status status;
    struct BookCopy* next;
} BookCopy;

extern Book* library;
extern BookCopy* inventory;

Book* createBook(char* ISBN, char* title, int author_id, enum book_status status, enum book_tag* tags, int tag_count, int total_count, int in_stock_count, int likes);
void insertBook(Book* book, Book** head);
void printBooks(Book* head);
Book* searchBooks(char* keyword, Book* head);
Book* filterByISBN(char* input_ISBN, Book* head);
Book* filterByBookStatus(int input_book_status, Book* head);
Book* filterByAuthorID(int input_author_id, Book* head);
Book* filterBooks(Book* parameters, Book* head);
int countBooks(Book* head);
int deleteBook(char* input_ISBN);
void freeBookList(Book* head);

BookCopy* createBookCopy(int book_uid, char* ISBN, enum book_status status);
void insertBookCopy(BookCopy* book_copy, BookCopy** head);
void printBookCopies(BookCopy* head);
BookCopy* getBookCopy(Book* book);
BookCopy* filterSoldCopyByISBN(char* input_ISBN, BookCopy* head);
int countBookCopies(BookCopy* head);
int deleteBookCopies(char* input_ISBN);
void freeBookCopyList(BookCopy* head);


#endif
