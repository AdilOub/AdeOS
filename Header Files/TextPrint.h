#pragma once
#include "IO.h"
#include "Typedefs.h"
#include "TextModeColorCodes.h"
#include "KBScanCodeSet1.h"

#define VGA_MEMORY (uint_8*)0xb8000
#define VGA_WIDTH 80

extern uint_16 CursorPosition;

void SetCursorPosition(uint_16 position);
uint_16 PosFromCoord(uint_8 x, uint_8 y);
void PrintChar(char chr, uint_8 color = BACKGROUND_BLACK | FOREGROUND_WHITE);
void PrintString(const char* str, uint_8 color = BACKGROUND_BLACK | FOREGROUND_WHITE);


void ClearScreen(uint_8 ClearColor = BACKGROUND_BLACK | FOREGROUND_WHITE);


const char* FloatToString(float value, uint_8 decimalPlaces = 3);

const char* HexToString(uint_8 value);
const char* HexToString(uint_16 value);
const char* HexToString(uint_32 value);
const char* HexToString(uint_64 value);
const char* HexToString(char value);
const char* HexToString(short value);
const char* HexToString(int value);
const char* HexToString(long long value);

const char* IntToString(uint_8 value);
const char* IntToString(uint_16 value);
const char* IntToString(uint_32 value);
const char* IntToString(uint_64 value);
const char* IntToString(char value);
const char* IntToString(short value);
const char* IntToString(int value);
const char* IntToString(long long value);