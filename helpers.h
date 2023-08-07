#ifndef HELPERS_H
#define HELPERS_H

typedef struct Date
{
    int year;
    int month;
    int day;
} Date;

extern const int rental_days;

char* formatBookStatus(int status);
char* formatBookTag(int tag);
char* formatUserCred(int title);
char* formatUserStatus(int status);
char* formatTransactionStatus(int status);

char* toLowerCase(const char* string);
int isContain(char* data, char* keyword);
int isMatch(char* data, char* input);
int countDigits(long num);
char* toString(int num);

Date* addDaysToDates(Date* date, int days);
int compareDate(Date* date1, Date* date2);
Date* getTodaysDate();
int isPastDue(Date* today, Date* due_date);
char* strsep(char** stringp, const char* delim);

char* generateISBN();

#endif
