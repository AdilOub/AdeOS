#include "../includes/usr/shell.h"

bool shellActive = false;

char buffer[256];
uint8_t bufferIndex = 0;

void clearBuffer(){
    uint64_t* b = (uint64_t*)buffer;
    for(uint8_t i = 0; i<32; i++){
        *b = 0;
        b++;
    }
}

void shell() {
    while (shellActive) {
        clearBuffer();
        printf("AdeOs> ");
        char c = '\0';
        while((c = getc()) != '\n'){
            buffer[bufferIndex] = c;
            bufferIndex++;
            printf("%c", c);
        }
        buffer[bufferIndex] = '\0';
        bufferIndex = 0;
        handleCmds(buffer);
    }
    return;
}

void startShell() {
    PrintString("Welcome to the shell!\n\r");
    clearBuffer();
    shellActive = true;
    shell();
    return;
}

void stopShell(){
    shellActive = false;
    return;
}