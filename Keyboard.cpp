#include "Header Files/Keyboard.h"
#include "Header Files/Render.h"

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



void StandarKeyBoardHandler(uint_8 scanCode, uint_8 chr){ //pk printstring ne marche pas ici ?
    //PrintChar('s'); //std keyboard handler loaded

    if(chr != 0){
        //PrintString("char non nul\n\r");
        switch (leftShiftPressed | rightShiftPressed)
        {
        case true: //shift pressed
            PrintChar(chr - 32);
            if(bufferIndex<256){
                cmdsBuffer[bufferIndex] = chr - 32;
                bufferIndex++;
            }

            break;
        case false: //no uppercase

            #ifdef VGA13
            PrintCharRender(getCursorPosRenderer(), chr-32);
            setCursorPosRenderer(getCursorPosRenderer() + 1);
            #else
            PrintChar(chr);
            #endif
            
            if(bufferIndex<256){
                cmdsBuffer[bufferIndex] = chr;
                bufferIndex++;
            }
            break;
        }
    }else{

        switch (scanCode)
        {
        case 0x8E: //backspace
            cmdsBuffer[bufferIndex] = '\0';
            cmdsBuffer[bufferIndex-1] = '\0';
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
        case 0x9C: //enter released
            bufferIndex = 0;
            handleCmds(cmdsBuffer);
            clearCmdBuffer(cmdsBuffer, 256);
            break;
        default:
                //PrintChar('?');
            break;
        }
    }
}

void KeyBoardHandler(uint_8 scanCode){
    //PrintString(IntToString(scanCode));
    //PrintString("\n\r");
    uint_8 chr = 0;
    if(scanCode < 0x3A){
        chr = LookupTable[scanCode];
    }

    //erk pourquoi j'ai fait ça ? enft si c'est logique
    switch (LastScanCode)
    {
    case 0x50: //down key
        if(arrowEnabled){
            SetCursorPosition(CursorPosition + VGA_WIDTH);
        }
        break;
    case 0x48: //up key
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
