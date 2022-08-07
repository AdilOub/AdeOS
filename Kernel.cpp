#include "TextPrint.cpp"

extern "C" void _start(){
    SetCursorPosition(PosFromCoord(0, 0));
    PrintString("Hello My bois\n\rEMT!\n\r", BACKGROUND_LIGHTCYAN | FOREGROUND_LIGHTRED);
    PrintString("\n\r");
    PrintString("Test du rigodrole", BACKGROUND_CYAN | FOREGROUND_GREEN);
    return;
}