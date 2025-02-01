#include "Header Files/Render.h"
#include "Header Files/TextPrint.h"
#ifndef VGA13


const char line_char = '$';

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

void Print(const char* str, uint8_t frontColor, uint8_t unused){
    PrintString(str, frontColor, unused);
}


#endif

#ifdef VGA13
uint16_t CursorPositionRenderer = 0;

void put_pixel(uint16_t x, uint16_t y, uint8_t color){
    uint8_t* memory = (uint8_t*)0xA0000;
    memory += VGA_13_W * x + y; //j'ai inversé x et y je crois ;-;
    *memory = color;
}

void draw_char(uint16_t x_offset, uint16_t y_offset, uint8_t chara, uint8_t frontColor, uint8_t backColor){ //TODO COLOR !!
    //char* ptr = policeA[chara];
    uint64_t charaHex = font[chara];
    uint64_t n = 1;

   for(int x = 7; x>=0; x--){
    for(int y = 7; y>=0; y--){
        uint64_t mask = charaHex & n;
        if(mask){
            put_pixel(x_offset+x, y_offset+y, frontColor);
        }else{
            put_pixel(x_offset+x, y_offset+y, backColor);
        }
        n*=2;
    }
   }
}

uint16_t getCursorPosRenderer(){
    return CursorPositionRenderer;
}

void setCursorPosRenderer(uint16_t newPos){
    CursorPositionRenderer = newPos;
}

uint16_t cursorPositionFromCoo(uint16_t x, uint16_t y){
    return y* SCREEN_MAX_LETTER_W + x;
}

void PrintCharRender(uint16_t cursorPosition, uint8_t chara, uint8_t frontColor, uint8_t backColor){
    if(chara == '\n'){
        CursorPositionRenderer = cursorPositionFromCoo(0, (uint16_t)((cursorPosition) / SCREEN_MAX_LETTER_H) + 1);
        return;
    }
    if(chara == '\r'){
        CursorPositionRenderer = cursorPositionFromCoo(-1, (uint16_t)((cursorPosition) / SCREEN_MAX_LETTER_H));
        return;
    }
    uint16_t y = (cursorPosition) % SCREEN_MAX_LETTER_W;
    uint16_t x  = (uint16_t)((cursorPosition) / SCREEN_MAX_LETTER_H);
    draw_char(x*LETTER_WIDTH, y*LETTER_WIDTH, (uint8_t)chara, frontColor, backColor);
}


void Print(const char* str, uint8_t frontColor, uint8_t backColor){
    while(*str != 0){
        PrintCharRender(CursorPositionRenderer, (uint8_t)(*str), frontColor, backColor);
        str++;
        CursorPositionRenderer++;
    }
}

void PlotImg(char* ptr, uint16_t sizeX, uint16_t sizeY){
    for(uint32_t i = 0; i < sizeX*sizeY; i++){
        uint16_t x = i / sizeY;
        uint16_t y = i % sizeY;
        put_pixel(x, y, (uint8_t)*ptr);
        ptr++; 
    }
}

void drawline(int p[2], int p2[2]){
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
            put_pixel(x, y, 0xF);
        }
        return;
    }

    //cas particulier d'une ligne verticale
    if(dx==0){
        //PrintString("Vertical line\n\r");
        for(y=y1; y<=y2; y++){
            put_pixel(x, y, 0xF);
        }
        return;
    }

    //cas général
    e10[0] = (float)dy/(float)dx; e10[1] = (float)dy/(float)dx;
    e01[0] = -1.0; e01[1] = -1.0;

    for(int x=x1; x<=x2; x++){
        //on dessine le pixel
        put_pixel(x, y, 0xF);
        e[0] += e10[0];
        e[1] += e10[1];
        if(e[0] > 0.5 || e[1] > 0.5){
            y++;
            e[0] += e01[0];
            e[1] += e01[1];
        }
    }
}

#endif