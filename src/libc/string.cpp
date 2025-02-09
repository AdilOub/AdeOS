#include "../includes/libc/string.h"

bool strcmp(const char* str1, const char* str2) {
    if(str1 == NULL || str2 == NULL) {
        return false;
    }

    while(*str1 && *str2) {
        if(*str1 != *str2) {
            return false;
        }
        str1++;
        str2++;
    }
    return *str1 == *str2;
}

uint64_t strlen(const char* str) {
    if(str == NULL) {
        return 0;
    }

    uint64_t len = 0;
    while(*str) {
        len++;
        str++;
    }
    return len;
}

uint64_t cntchar(char* str, char c) {
    if(str == NULL) {
        return 0;
    }
    uint64_t cnt = 0;
    while(*str) {
        if(*str == c) {
            cnt++;
        }
        str++;
    }
    return cnt;
}

char** strsplit(char* str, char separator){
    return NULL;

}


/*
TODO add to makefile et le finir
*/