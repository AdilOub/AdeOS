#include "Header Files/Render.h"


#ifdef VGA13
uint_16 CursorPositionRenderer = 0;

void put_pixel(uint_16 x, uint_16 y, uint_8 color){
    uint_8* memory = (uint_8*)0xA0000;
    memory += VGA_13_W * x + y;
    *memory = color;
}

void draw_char(uint_16 x_offset, uint_16 y_offset, uint_8 chara, uint_8 frontColor, uint_8 backColor){ //TODO COLOR !!
    //char* ptr = policeA[chara];
    uint_64 charaHex = font[chara];
    int n = 0;
    for(int x = 0; x < 8; x++){
        for(int y = 0; y<8; y++){
            if((charaHex&(pow(2, 32-n))) != 0){
                put_pixel(x_offset+x, y_offset+y, frontColor);
            }else{
                put_pixel(x_offset+x, y_offset+y, backColor);
            }
            n++;
        }
    }
}

uint_16 getCursorPosRenderer(){
    return CursorPositionRenderer;
}

void setCursorPosRenderer(uint_16 newPos){
    CursorPositionRenderer = newPos;
}

uint_16 cursorPositionFromCoo(uint_16 x, uint_16 y){
    return y* SCREEN_MAX_LETTER_W + x;
}

void PrintCharRender(uint_16 cursorPosition, uint_8 chara){
    uint_16 y = (cursorPosition) % SCREEN_MAX_LETTER_W;
    uint_16 x  = (uint_16)((cursorPosition) / SCREEN_MAX_LETTER_H);
    draw_char(x*LETTER_WIDTH, y*LETTER_WIDTH, (uint_8)chara);
}


void Print(const char* str){
    while(*str != 0){
        PrintCharRender(CursorPositionRenderer, (uint_8)(*str));
        str++;
        CursorPositionRenderer++;
    }
}

void PlotImg(char* ptr, uint_16 sizeX, uint_16 sizeY){
    for(uint_32 i = 0; i < sizeX*sizeY; i++){
        uint_16 x = i / sizeY;
        uint_16 y = i % sizeY;
        put_pixel(x, y, (uint_8)*ptr);
        ptr++; 
    }
}

#endif