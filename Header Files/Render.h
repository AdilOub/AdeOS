#include "Typedefs.h"
#include "ASCITable.h"

#define VGA_OFFSET 0xA0000
#define VGA_13_W 320
#define VGA_13_H 200
#define LETTER_WIDTH 8

#define SCREEN_MAX_LETTER_W 40
#define SCREEN_MAX_LETTER_H 25

void put_pixel(uint_16 x, uint_16 y, uint_8 color);

void draw_char(uint_16 x, uint_16 y, uint_8 chara);

void PrintCharRender(uint_8 cursorPosition, uint_8 chara);
void Print(const char* str);

uint_8 cursorPositionFromCoo(uint_8 x, uint_8 y);