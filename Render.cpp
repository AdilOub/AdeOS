#include "Header Files/Render.h"

uint_8 CursorPositionRenderer = 0;

void put_pixel(uint_16 x, uint_16 y, uint_8 color){
    uint_8* memory = (uint_8*)0xA0000;
    memory += VGA_13_W * x + y;
    *memory = color;
}

void draw_char(uint_16 x_offset, uint_16 y_offset, uint_8 chara){ //TODO COLOR !!
    char* ptr = policeA[chara];
    for(int x = 0; x < 8; x++){
        for(int y = 0; y<8; y++){
            put_pixel(x_offset+x, y_offset+y, 0X0F+(1-*ptr));
            ptr++;
        }
    }
}

uint_8 cursorPositionFromCoo(uint_8 x, uint_8 y){
    return y* SCREEN_MAX_LETTER_W + x;
}

void PrintCharRender(uint_8 cursorPosition, uint_8 chara){
    uint_16 y = (cursorPosition) % SCREEN_MAX_LETTER_W;
    uint_16 x  = (cursorPosition) / SCREEN_MAX_LETTER_H;
    draw_char(x*LETTER_WIDTH, y*LETTER_WIDTH, (uint_8)chara);
}


void Print(const char* str){
    while(*str != 0){
        PrintCharRender(CursorPositionRenderer, (uint_8)(*str-65)); //TODO CHANGE THAT
        str++;
        CursorPositionRenderer++;
    }
}