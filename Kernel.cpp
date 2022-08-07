#include "TextPrint.cpp"

extern const char Test[];

extern "C" void _start(){
    SetCursorPosition(PosFromCoord(0, 0));
    PrintString("Hello My bois\n\rEMT!\n\r", BACKGROUND_LIGHTCYAN | FOREGROUND_LIGHTRED);
    PrintString("\n\r");
    ClearScreen();
    SetCursorPosition(PosFromCoord(0, 0));
    PrintString(Test);
    return;
}