#pragma once

#include "libc/stdtypes.h"
#include "ASCITable.h"
#include "Math.h"




void Print(const char* str, uint8_t frontColor = 0x0F, uint8_t backColor = 0x00);


#ifdef VGA13

#define VGA_OFFSET 0xA0000
#define VGA_13_W 320
#define VGA_13_H 200

#define LETTER_WIDTH 8

#define SCREEN_MAX_LETTER_W 40
#define SCREEN_MAX_LETTER_H 40

void put_pixel(uint16_t x, uint16_t y, uint8_t color);

void draw_char(uint16_t x_offset, uint16_t y_offset, uint8_t chara, uint8_t frontColor = 0x0F, uint8_t backColor = 0x00); //TODO COLOR !!

void PrintCharRender(uint16_t cursorPosition, uint8_t chara, uint8_t frontColor, uint8_t backColor);

uint16_t cursorPositionFromCoo(uint16_t x, uint16_t y);

uint16_t getCursorPosRenderer();
void setCursorPosRenderer(uint16_t newPos);

void PlotImg(char* ptr, uint16_t sizeX, uint16_t sizeY);

void drawline(int p[2], int p2[2]);

#else
void drawline(int p[2], int p2[2]);

#endif