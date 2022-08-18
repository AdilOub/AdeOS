#include "Header Files/Keyboard.h"

bool leftShiftPressed = false;
bool rightShiftPressed = false;
uint_8 LastScanCode;


bool arrowEnabled = false;

char cmdsBuffer[256];
int bufferIndex = 0;

char LookupTable[58];


void setLanguage(const char* table){
    for(int i = 0; i<58; i++){
        LookupTable[i] = table[i];
    }
}

void enableArrow(bool enable){
    arrowEnabled = enable;
}
bool getArrowState(){
    return arrowEnabled;
}



void StandarKeyBoardHandler(uint_8 scanCode, uint_8 chr){
    if(chr != 0){
        switch (leftShiftPressed | rightShiftPressed)
        {
        case true:
            PrintChar(chr - 32);
            cmdsBuffer[bufferIndex] = chr - 32;
            bufferIndex++;
            break;
        case false:
            PrintChar(chr);
            cmdsBuffer[bufferIndex] = chr;
            bufferIndex++;
            break;
        }
    }else{
        switch (scanCode)
        {
        case 0x8E: //backspace
            cmdsBuffer[bufferIndex] = '\0';
            bufferIndex -= 1;

            SetCursorPosition(CursorPosition-1);
            PrintChar(' ');
            SetCursorPosition(CursorPosition-1);
            break;
        case 0x2A: //leftshiftPress
            leftShiftPressed = true;
            break;
        case 0xAA: //leftshiftRealsed
            leftShiftPressed = false;
            break;
        case 0x36: //rightshiftPress
            rightShiftPressed = true;
            break;
        case 0xB6: //rightshiftRealsed
            rightShiftPressed = false;
            break;
        case 0x9C: //enter
            bufferIndex = 0;
            handleCmds(cmdsBuffer);
            clearCmdBuffer(cmdsBuffer, 256);
            break;
        default:
            break;
        }
    }
}

void KeyBoardHandler(uint_8 scanCode){
    //PrintString(IntToString(scanCode));
    uint_8 chr = 0;
    if(scanCode < 0x3A)
        chr = LookupTable[scanCode];

    switch (LastScanCode)
    {
    case 0x50:
        if(arrowEnabled){
            SetCursorPosition(CursorPosition + VGA_WIDTH);
        }
        break;
    case 0x48:
        if(arrowEnabled){
            SetCursorPosition(CursorPosition - VGA_WIDTH);
        }
        break;

    default:
        StandarKeyBoardHandler(scanCode, chr);
        break;
    }

    LastScanCode = scanCode;
}
