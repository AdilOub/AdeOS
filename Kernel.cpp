#include "Header Files/TextPrint.h"
#include "Header Files/IDT.h"
#include "Header Files/Keyboard.h"
#include "Header Files/Memory.h"
#include "Header Files/IO.h"
#include "Header Files/Heap.h"
#include "Header Files/Typedefs.h"

//#include "HeaderFiles/Commands.h"

extern const char Test[];

void PrintAllMemoryMap(MemoryMapEntry** UsableMemeoryMap){
    for(uint_8 i = 0; i < UsableMemeoryRegionCount; i++){
        PrintMemoryMap(UsableMemeoryMap[i], CursorPosition);
    }
}


extern "C" void _start(){
    InitIDT();
    setLanguage(KBSet1::ScanCodeLookupTableAZERTY);
    SetCursorPosition(PosFromCoord(0, 0));
    MainKeyBoardHandler = KeyBoardHandler;
    ClearScreen(BACKGROUND_BLACK);

    PrintString(Test);
    PrintString("\n\r\n\r");


    MemoryMapEntry** UsableMemeoryMap = getUsableMemoryRegions();
    PrintAllMemoryMap(UsableMemeoryMap);

    InitializeHeap(0x100000, 0x100000);
    void* Test = malloc(0x10);
    void* Test2 = malloc(0x10);
    void* Test3 = malloc(0x10);

    PrintString(HexToString((uint_64)Test));    PrintString("\n\r");

    PrintString(HexToString((uint_64)Test2));    PrintString("\n\r");

    PrintString(HexToString((uint_64)Test3));    PrintString("\n\r");


    return;
}