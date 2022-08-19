#include "Header Files/Kernel.h"

#include "Header Files/TextPrint.h"
#include "Header Files/IDT.h"
#include "Header Files/Keyboard.h"
#include "Header Files/Memory.h"
#include "Header Files/IO.h"
#include "Header Files/Heap.h"
#include "Header Files/Typedefs.h"

//#include "HeaderFiles/Commands.h"


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
    PrintString("\n\r");


    MemoryMapEntry** UsableMemeoryMap = getUsableMemoryRegions();
    PrintAllMemoryMap(UsableMemeoryMap);

    InitializeHeap(0x100000, 0x100000);

    uint_64* TAest = (uint_64*)aligned_alloc(0x4000, 0x08);
    free(TAest);
    uint_64* Test = (uint_64*)malloc(0x08);
    PrintString(HexToString((uint_64)Test));

    return;
}