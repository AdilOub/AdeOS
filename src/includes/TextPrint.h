#pragma once
#include "cpu/io.h"
#include "libc/stdtypes.h"
#include "TextModeColorCodes.h"
#include "KBScanCodeSet1.h"
#include "libc/string.h"

#define VGA_MEMORY (uint8_t*)0xb8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

#define MAX_PRINTF_BUFFER 256


extern uint16_t CursorPosition;

void SetCursorPosition(uint16_t position);
uint16_t GetCursorPosition();
uint16_t PosFromCoord(uint8_t x, uint8_t y);
void PrintChar(char chr, uint8_t color = BACKGROUND_BLACK | FOREGROUND_WHITE);
void PrintString(const char* str, uint8_t color = BACKGROUND_BLACK | FOREGROUND_WHITE, uint8_t unused = 0);

void jmpLine();
void ClearScreen(uint8_t ClearColor = BACKGROUND_BLACK | FOREGROUND_WHITE);


const char* FloatToString(float value, uint8_t decimalPlaces = 3);

const char* HexToString(uint8_t value);
const char* HexToString(uint16_t value);
const char* HexToString(uint32_t value);
const char* HexToString(uint64_t value);
const char* HexToString(char value);
const char* HexToString(short value);
const char* HexToString(int value);
const char* HexToString(long long value);

const char* IntToString(uint8_t value);
const char* IntToString(uint16_t value);
const char* IntToString(uint32_t value);
const char* IntToString(uint64_t value);
const char* IntToString(char value);
const char* IntToString(short value);
const char* IntToString(int value);
const char* IntToString(long long value);


void testPrint();
void printf(char* format, ...);