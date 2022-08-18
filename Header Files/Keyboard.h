#pragma once
#include "Keyboard.h"
#include "TextPrint.h"
#include "IDT.h"
#include "Commands.h"
#include "KBScanCodeSet1.h"
#include "Typedefs.h"

extern bool leftShiftPressed;
extern bool rightShiftPressed;
extern uint_8 LastScanCode;


extern bool arrowEnabled;

extern char cmdsBuffer[256];
extern int bufferIndex;

extern char LookupTable[58];


extern void setLanguage(const char* table);
extern void enableArrow(bool enable);
extern bool getArrowState();
extern void StandarKeyBoardHandler(uint_8 scanCode, uint_8 chr);
extern void KeyBoardHandler(uint_8 scanCode);