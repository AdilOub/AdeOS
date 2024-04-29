#include "Kernel.h"
#include "Typedefs.h"
#include "ASCITable.h"
#include "Math.h"

#ifdef VGA13

#define VGA_OFFSET 0xA0000
#define VGA_13_W 320
#define VGA_13_H 200

#define LETTER_WIDTH 8

#define SCREEN_MAX_LETTER_W 40
#define SCREEN_MAX_LETTER_H 40

void put_pixel(uint_16 x, uint_16 y, uint_8 color);

void draw_char(uint_16 x_offset, uint_16 y_offset, uint_8 chara, uint_8 frontColor = 0x0F, uint_8 backColor = 0x00); //TODO COLOR !!

void PrintCharRender(uint_16 cursorPosition, uint_8 chara);
void Print(const char* str);

uint_16 cursorPositionFromCoo(uint_16 x, uint_16 y);

uint_16 getCursorPosRenderer();
void setCursorPosRenderer(uint_16 newPos);

void PlotImg(char* ptr, uint_16 sizeX, uint_16 sizeY);

#else
void drawline(int p[2], int p2[2]);

#endif