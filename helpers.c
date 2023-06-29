#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

char* formatBookStatus(int status)
{
    char* book_s[] = { "none", "inactive", "active" };
    return book_s[status];
}

char* formatBookTag(int tag)
{
    char* book_t[] = { "none", "biography", "fantasy", "fiction", "mystery", "romance", "scifi", "thriller", "young adult" };
    return book_t[tag];
}

char* formatUserCred(int title)
{
    char* cred[] = { "none", "author", "customer", "librarian" };
    return cred[title];
}

char* formatUserStatus(int status)
{
    char* user_s[] = { "none", "inactive", "active", "delinquent" };
    return user_s[status];
}

char* formatTransactionStatus(int status)
{
    char* trans_s[] = {"none", "open", "close"};
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
    char* num_string = malloc(countDigits(num + 1) * sizeof(char));
    sprintf(num_string, "%i", num);
    return num_string;
}


