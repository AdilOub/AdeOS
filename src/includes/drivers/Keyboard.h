#pragma once
#include "Keyboard.h"

#include "../TextPrint.h"
#include "../cpu/idt.h"
#include "../Commands.h"
#include "../KBScanCodeSet1.h"
#include "../libc/stdtypes.h"

extern bool leftShiftPressed;
extern bool rightShiftPressed;
extern uint8_t LastScanCode;


extern bool arrowEnabled;


extern char LookupTable[58];


extern void setLanguage(const char* table);
extern void enableArrow(bool enable);
extern bool getArrowState();
extern void StandarKeyBoardHandler(uint8_t scanCode, uint8_t chr);
extern void KeyBoardHandler(uint8_t scanCode);


char getc();