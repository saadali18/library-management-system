#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "helpers.h"
#include "books.h"

char* formatBookStatus(int status)
{
    char* book_s[] = { "none", "inactive", "active", "deleted", "sold" };
    return book_s[status];
}

char* formatBookTag(int tag)
{
    char* book_t[] = { "none", "biography", "fantasy", "fiction", "mystery", "romance", "scifi", "thriller", "young adult" };
    return book_t[tag];
}

char* formatUserCred(int title)
{
    char* cred[] = { "none", "author", "customer", "librarian", "deleted" };
    return cred[title];
}

char* formatUserStatus(int status)
{
    char* user_s[] = { "none", "inactive", "active", "delinquent" };
    return user_s[status];
}

char* formatTransactionStatus(int status)
{
    char* trans_s[] = {"none", "open", "close", "past due", "purchased" };
    return trans_s[status];
}

char* toLowerCase(const char* string) // Convert string to lower case, ignore number
{
    if (!string)	return NULL;
    int len = strlen(string);
    char* new_string = malloc((len + 1) * sizeof(char));
    if (!new_string)	return NULL;

    for (int i = 0; i < len; i++)
    {
        new_string[i] = tolower(string[i]);
    }
    new_string[len] = '\0';
    return new_string;
}

int isContain(char* data, char* keyword) // Check if a string contain the keyword input from user
{
    if (!data || !keyword)	return 0;

    char* lower_case_data = toLowerCase(data);
    char* lower_case_keyword = toLowerCase(keyword);
    if (!strstr(lower_case_data, lower_case_keyword))
    {
        free(lower_case_data);
        free(lower_case_keyword);
        return 0;
    }
    else
    {
        free(lower_case_data);
        free(lower_case_keyword);
        return 1; // a match
    }
}

int isMatch(char* data, char* input)
{
    if (!data || !input)    return 0; // if either is NULL = not match
    char* lower_case_data = toLowerCase(data);
    char* lower_case_input = toLowerCase(input);
    if (strcmp(lower_case_data, lower_case_input))
    {
        free(lower_case_data);
        free(lower_case_input);
        return 0; // not match
    }
    else
    {
        free(lower_case_data);
        free(lower_case_input);
        return 1; // a match
    }
}

int countDigits(long num)
{
    int count = 0;
    if (num == 0)	return 1;

    if (num < 0)	num = -num;

    while (num > 0)
    {
        count++;
        num = num / 10;
    }

    return count;
}

char* toString(int num)
{
    char* num_string = malloc((countDigits(num) + 1) * sizeof(char));
    sprintf(num_string, "%i", num);
    return num_string;
}

const int rental_days = 90;

Date* addDaysToDates(Date* date, int days)
{
    Date* calculated_date = malloc(sizeof(Date));
    calculated_date->year = date->year;
    calculated_date->month = date->month;
    calculated_date->day = date->day;

    int days_in_month[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    calculated_date->day += days;

    while (calculated_date->day > days_in_month[calculated_date->month])
    {
        calculated_date->day -= days_in_month[calculated_date->month];
        calculated_date->month++;

        if (calculated_date->month > 12)
        {
            calculated_date->month -= 12;
            calculated_date->year++;
        }
    }

    return calculated_date;
}

int compareDate(Date* date1, Date* date2)
{
    if (date1->year == date2->year && date1->month == date2->month && date1->day == date2->day)
        return 1;
    else return 0;
}

Date* getTodaysDate()
{
    time_t current_time;
    time(&current_time);

    struct tm* local_time = localtime(&current_time);

    Date* today = malloc(sizeof(Date));
    today->year = local_time->tm_year + 1900;
    today->month = local_time->tm_mon + 1;
    today->day = local_time->tm_mday;

    return today;
}

int isPastDue(Date* today, Date* due_date)
{
    int is_past = 0;
    if (today->year > due_date->year)   is_past = 1;
    else if (today->year == due_date->year)
    {
        if (today->month > due_date->month || (today->month == due_date->month && today->day < due_date->day))
            is_past = 1;
    }
    return is_past;
}

char* strsep(char** stringp, const char* delim)
{
    char* rv = *stringp;
    if (rv) {
        *stringp += strcspn(*stringp, delim);
        if (**stringp)
            *(*stringp)++ = '\0';
        else
            *stringp = 0;
    }
    return rv;
}

int isUniqueISBN(char* random_ISBN, Book* head)
{
    int is_unique = 1;
    Book* current = head;
    if (!current)   return is_unique; // If no book yet in library, new ISBN is unique

    while (current) {
        if (isMatch(current->ISBN, random_ISBN)) {
            is_unique = 0;
            break;
        }
        current = current->next;
    }
    return is_unique;
}

long long generateRandomNum(long long lower, long long upper)
{

    srand(time(NULL));
    long long num = (rand() % (upper - lower)) + lower;
    return num;
}

char* generateISBN()
{
    int prefix = 987; // Prefix for any ISBN
// Make sure the body has 10 digit after prefix to make up 13 digit ISBN
    long long MIN = 1000000000;
    long long MAX = 9999999999;
    char* ISBN = malloc(14);
    do {
        srand(time(NULL));
        long long num = generateRandomNum(MIN, MAX);
        sprintf(ISBN, "%i%lld", prefix, num);
    } while (!isUniqueISBN(ISBN, library));
    return ISBN;
}


