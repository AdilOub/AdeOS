#include "../includes/drivers/Keyboard.h"
#include "../includes/Render.h"

bool leftShiftPressed = false;
bool rightShiftPressed = false;
uint8_t LastScanCode;


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



void StandarKeyBoardHandler(uint8_t scanCode, uint8_t chr){ //pk printstring ne marche pas ici ? //edit 2076: si je savais l'enfer que j'ai du vivre pour fix ça mdr
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
            PrintCharRender(getCursorPosRenderer(), chr, 0x0F, 0x00);
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
        int i = 0;
        int n = 0;
        uint64_t current_add = 0;
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

            //PrintString("\n\n\n\rCommand add: ");
            //current_add = (uint64_t)((void*)StandarKeyBoardHandler);
            //PrintString(HexToString(current_add));
            //PrintString("\n\r");

            //asm("mov %%eax, %0" : "=b"(i));
            //PrintString("Buffer: ");
            //PrintString(IntToString(i));
            //PrintString("\n\r");

            //asm("mov $0x69, %eax");
            //asm("mov %%eax, %0" : "=b"(i));
            //PrintString("Buffer: ");
            //PrintString(IntToString(i));
            //PrintString("\n\r");

            //asm("int $0x80");
            bufferIndex = 0;
            handleCmds(cmdsBuffer);
            clearCmdBuffer(cmdsBuffer, 256);
            //asm("int $0x2c");
            break;

        default:
                //PrintChar('?');
            break;
        }
    }
}



int i = 0;

void memDmp2(uint8_t* ptr, uint64_t size){
    for(uint64_t i = 0; i < size; i++){
        PrintString(HexToString(ptr[i]));
        PrintChar(' ');
    }
}

#ifdef DEBUG_MEMORY_PRINT
void KeyBoardHandler(uint8_t scanCode){
    //PrintString(IntToString(scanCode));
    //PrintString("\n\r");
    uint8_t chr = 0;
    if(scanCode < 0x3A){
        chr = LookupTable[scanCode];
        ClearScreen();
        SetCursorPosition(0);
        memDmp2((uint8_t*)0x8000 +0x7000 + 512*i, 512);
        //memDmp2((uint8_t*)0xFFFF, 512);
        //on affiche l'adresse de la page
        SetCursorPosition(PosFromCoord(0, 24));
        PrintString(HexToString(0x8000 + 0x7000 + 512*i), BACKGROUND_RED | FOREGROUND_WHITE);
        if(chr == 'b'){
            i--;
            ClearScreen();
            SetCursorPosition(0);
            memDmp2((uint8_t*)0x8000 +0x7000 + 512*i, 512);
            //memDmp2((uint8_t*)0xFFFF, 512);
            //on affiche l'adresse de la page
            SetCursorPosition(PosFromCoord(0, 24));
            PrintString(HexToString(0x8000 + 0x7000 + 512*i), BACKGROUND_RED | FOREGROUND_WHITE);
        }else{
            i++;
            PrintChar(chr, BACKGROUND_RED | FOREGROUND_GREEN);
        }
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

#else

void KeyBoardHandler(uint8_t scanCode){
    //PrintString(IntToString(scanCode));
    //PrintString("\n\r");
    uint8_t chr = 0;
    if(scanCode < 0x3A){
        chr = LookupTable[scanCode];
        StandarKeyBoardHandler(scanCode, chr);
        return;
    }
    
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

#endif