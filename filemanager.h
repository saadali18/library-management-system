#ifndef LIBRARY_MANAGEMENT_SYSTEM_FILEMANAGER_H
#define LIBRARY_MANAGEMENT_SYSTEM_FILEMANAGER_H

typedef struct Record {
    char* text;
    struct Record* next;
}Record;

extern Record* book_record_head;
extern Record* user_record_head;
extern Record* book_copy_record_head;
extern Record* transaction_record_head;

extern const char* book_file;
extern const char* user_file;
extern const char* book_copy_file;
extern const char* transaction_file;

void insertRecord(char* text, Record** head);
void readRecord(const char* file_path, Record** record_head);
void printRecord(Record* head);

void loadBooks(Record* record_head, Book** book_head);
void saveBooks(const char* file_name, Book* book_head);

void loadBookCopies(Record* record_head, BookCopy** book_copy_head);
void saveBookCopies(const char* file_name, BookCopy* book_copy_head);

void loadUsers(Record* record_head, User** user_head);
void saveUsers(const char* file_name, User* user_head);

void loadTransactions(Record* record_head, BookTransaction** transaction_head);
void saveTransactions(const char* file_name, BookTransaction* transaction_head);

#endif
