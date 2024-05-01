#include "Header Files/Render.h"

#ifndef VGA13


//const char line_char = '$';
/*
//on utilise l'algo de Bresenham pour dessiner une ligne
void drawline(int p[2], int p2[2]){
    char line = line_char;
    int x1 = p[0];
    int y1 = p[1];
    int x2 = p2[0];
    int y2 = p2[1];

    //on remet dans le bon sens si x2 < x1
    if(x2 < x1){
        int tmp = x1;
        x1 = x2;
        x2 = tmp;
        tmp = y1;
        y1 = y2;
        y2 = tmp;
    }

    int x,y,dx,dy;
    float e[2] = {0.0,0.0};
    float e10[2] = {1.0,0.0};
    float e01[2] = {0.0,1.0};

    x=x1;
    y = y1;
    dy = y2 - y1;
    dx = x2 - x1;
    //cas particulier d'une ligne horizontale
    if(dy==0){
        //PrintString("Horizontal line\n\r");
        for(x=x1; x<=x2; x++){
            int index = x + y*80;
            *(VGA_MEMORY + index*2) = '$'; //'*'
            *(VGA_MEMORY + index*2 +1) = FOREGROUND_WHITE;

        }
        return;
    }

    //cas particulier d'une ligne verticale
    if(dx==0){
        //PrintString("Vertical line\n\r");
        for(y=y1; y<=y2; y++){
            int index = x + y*80;
            *(VGA_MEMORY + index*2) = '$'; //'*'
            *(VGA_MEMORY + index*2 +1) = FOREGROUND_WHITE;
        }
        return;
    }

    //cas général
    e10[0] = (float)dy/(float)dx; e10[1] = (float)dy/(float)dx;
    e01[0] = -1.0; e01[1] = -1.0;

    for(int x=x1; x<=x2; x++){
        //on dessine le pixel
        int index = x + y*80;
        *(VGA_MEMORY + index*2) = line_char; //'*'
        *(VGA_MEMORY + index*2 +1) = FOREGROUND_WHITE;
        e[0] += e10[0];
        e[1] += e10[1];
        if(e[0] > 0.5 || e[1] > 0.5){
            y++;
            e[0] += e01[0];
            e[1] += e01[1];
        }
    }
}
*/

#endif

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