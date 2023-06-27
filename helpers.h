//
// Created by Saad Ali on 24/06/2023.
//

#ifndef LMS_PRACTICE_ANNIE_CODE_HELPERS_H
#define LMS_PRACTICE_ANNIE_CODE_HELPERS_H


char* formatBookStatus(int status);
char* formatBookTag(int tag);
char* formatUserCred(int title);
char* formatUserStatus(int status);

char* toLowerCase(const char* string);
int isContain(char* data, char* keyword);
int isMatch(char* data, char* input);
int countDigits(long num);
char* toString(long num);


#endif //LMS_PRACTICE_ANNIE_CODE_HELPERS_H
