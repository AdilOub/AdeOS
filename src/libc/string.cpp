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

uint64_t atoi(char* str){
    uint64_t n = 0;
    while(*str != 0) {
        n += *str - '0';
        n *= 10;
        str++;
    }
    return n;
}



uint16_t strargc(char* str, char sep){
    uint16_t n = 0;
    while(*str != 0){
        if(*str == sep && *(str+1) != 0)
            n++;
        str++;
    }
    return n+1;
}

char** splitargv(char* str, char sep){
    uint16_t n_partie = strargc(str, sep);
    char** res = (char**)malloc(sizeof(char*) * n_partie);

    if(n_partie == 1){ //si aucun separateur, on renvoie la chaine entière
        res[0] = (char*)malloc(sizeof(char) * (strlen(str)+1));
        strcpy(res[0], str);
        return res;
    }
 
    uint16_t start = 0;
    for(uint16_t i = 0; i<n_partie; i++){
        uint16_t end = start;
        while(str[end] != sep && str[end] != 0){
            end++;
        }
        
        res[i] = (char*)malloc(sizeof(char) * (end-start+1));
        memcopy(res[i], str+start, end-start);
        res[i][end-start] = 0;
        start = end+1;
    }
    
    return res;
}

void freeargv(char** argv, uint16_t argc){
    for(uint16_t i = 0; i<argc; i++){
        free(argv[i]);
    }
    free(argv);
}

/*
TODO add to makefile et le finir
*/