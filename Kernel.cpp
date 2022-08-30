#include "Header Files/Kernel.h"

#include "Header Files/TextPrint.h"
#include "Header Files/IDT.h"
#include "Header Files/Keyboard.h"
#include "Header Files/Memory.h"
#include "Header Files/IO.h"
#include "Header Files/Heap.h"
#include "Header Files/Typedefs.h"

//to use drawingggg
#include "Header Files/Render.h"

//#include "HeaderFiles/Commands.h"


void PrintAllMemoryMap(MemoryMapEntry** UsableMemeoryMap){
    for(uint_8 i = 0; i < UsableMemeoryRegionCount; i++){
        PrintMemoryMap(UsableMemeoryMap[i], CursorPosition);
    }
}

extern "C" void _start(){
    InitIDT();
    setLanguage(KBSet1::ScanCodeLookupTableAZERTY);
    MainKeyBoardHandler = KeyBoardHandler;
    ClearScreen(BACKGROUND_BLACK);

    MemoryMapEntry** UsableMemeoryMap = getUsableMemoryRegions();

    InitializeHeap(0x100000, 0x100000);

    
    #ifndef VGA13
    SetCursorPosition(PosFromCoord(0, 0));
    PrintString(Test);
    PrintString("\n\r");
    PrintString("Memory:\n\r");
    PrintAllMemoryMap(UsableMemeoryMap);
    endCmd();
    #endif

    //Print("ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZ");
    #ifdef VGA13
    Print("!\"#$%&\'()*+,-./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    //PlotImg((char *)relou, 262, 200);
    #endif

    ClearScreen();
    SetCursorPosition(0);
    uint_8* img = 0;
    img = (uint_8*)malloc(262*200*sizeof(uint_8));

    uint_8* Emi1Pointer = (uint_8*)Emi1;

    for(int i = 0; i<5; i++){
        while (*Emi1Pointer != ',')
        {
            PrintChar(*Emi1Pointer);
            Emi1Pointer++;
        }
        PrintString("\n\r");
        Emi1Pointer++;
    }


    return;
}