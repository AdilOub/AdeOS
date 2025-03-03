#include "../includes/usr/shell.h"

bool shellActive = false;

char buffer[256];
uint8_t bufferIndex = 0;

uint16_t currentInode = 0;

void clearBuffer(){
    uint64_t* b = (uint64_t*)buffer;
    for(uint8_t i = 0; i<32; i++){
        *b = 0;
        b++;
    }
}

void shell() {
    char c;
    while (shellActive) {
        clearBuffer();
        printf("AdeOs> ");
        while((c = getc()) != '\n'){
            if(c== '\b'){
                if(bufferIndex == 0){
                    continue;
                }
                backspace(BACKGROUND_BLACK);
                buffer[--bufferIndex] = '\0';
                continue;
            }
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
    printf("Welcome to the shell! address of buffer: %x\n", (uint64_t)buffer);
    clearBuffer();
    bufferIndex = 0;
    shellActive = true;
    shell();
    return;
}

void stopShell(){
    shellActive = false;
    return;
}