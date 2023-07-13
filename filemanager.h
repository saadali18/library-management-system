#ifndef LIBRARY_MANAGEMENT_SYSTEM_FILEMANAGER_H
#define LIBRARY_MANAGEMENT_SYSTEM_FILEMANAGER_H

typedef struct Record {
    char* text;
    struct Record* next;
}Record;

extern Record* record_head;
extern char* book_file;

void insertRecord(char* text, Record** head);
void readRecord(char* file_path);
void printRecord(Record* head);
void insertParsedBookRecords(Record* book_record, Book** book_head);
void loadBooks();
void saveBooks();

#endif
