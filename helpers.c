#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

char* formatBookStatus(int status)
{
    char* book_s[] = { "inactive", "active" };
    return book_s[status];
}

char* formatBookTag(int tag)
{
    char* book_t[] = { "biography", "fantasy", "fiction", "mystery", "romance", "sci fi", "thriller", "young adult" };
    return book_t[tag];
}

char* formatUserCred(int title)
{
    char* cred[] = { "author", "customer", "librarian" };
    return cred[title];
}

char* formatUserStatus(int status)
{
    char* user_s[] = { "inactive", "active", "delinquent" };
    return user_s[status];
}

char* toLowerCase(const char* string) // Convert string to lower case, ignore number
{
    if (!string)	return NULL;
    int len = strlen(string);
    char* new_string = malloc((len + 1) * sizeof(char));
    if (!new_string)	return NULL;

    for (int i = 0; i < len; i++)
    {
        new_string[i] = tolower(string[i]); // exception thrown - access violation here
    }
    new_string[len] = '\0';
    return new_string;
}

int isContain(char* data, char* keyword) // Check if a string contain the keyword input from user
{
    if (!data || !keyword)	return 0;

    if (!strstr(toLowerCase(data), toLowerCase(keyword)))	return 0;
    else	return 1;
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

char* toString(long num)
{
    char* num_string = malloc(countDigits(num + 1) * sizeof(char));
    sprintf(num_string, "%ld", num);
    return num_string;
}
