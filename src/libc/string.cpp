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


const char* strcat(char* dest, const char* src) {
    uint64_t dest_len = strlen(dest);
    uint64_t src_len = strlen(src);
    char* new_str = (char*)realloc(dest, dest_len + src_len + 1);
    for(uint64_t i = 0; i < src_len; i++) {
        new_str[dest_len + i] = src[i];
    }
    new_str[dest_len + src_len] = '\0';
    return new_str;
}

void strcatover(char** dest, const char* src) {
    *dest = (char*)realloc(dest, strlen(*dest) + strlen(src) + 1);
    strcat(*dest, src);
    return;
}

void strcpy(char* dest, const char* src) {
    while(*src) {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
}


/*
TODO add to makefile et le finir
*/