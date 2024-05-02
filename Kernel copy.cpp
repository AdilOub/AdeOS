#include "Header Files/Kernel.h"

#include "Header Files/TextPrint.h"
#include "Header Files/IDT.h"
#include "Header Files/Keyboard.h"
#include "Header Files/Memory.h"
#include "Header Files/IO.h"
#include "Header Files/Heap.h"
#include "Header Files/Typedefs.h"
#include "Header Files/Timer.h"
//#include "Header Files/FloppyDisk.h"

//to use drawingggg
#include "Header Files/Render.h"

//#include "HeaderFiles/Commands.h"


/*
TODO Important!!!
Il y a actuellement un problème avec le linker:
la section data ne semble pas être chargée correctement
et donc les variables globales non constante ne sont pas initialisées
(ce n'est pas le cas pour les variables globales constante)

*/

#define NULL 0

void PrintAllMemoryMap(MemoryMapEntry** UsableMemeoryMap){
    for(uint_8 i = 0; i < UsableMemeoryRegionCount; i++){
        PrintMemoryMap(UsableMemeoryMap[i], CursorPosition);
    }
}




void PrintString2(const char str[], uint_8 color){
    PrintChar(str[0], color);
}

//affiche la mémoire à partir de ptr jusqu'à size
void memDmp(uint_8* ptr, uint_64 size){
    for(uint_64 i = 0; i < size; i++){
        PrintString(HexToString(ptr[i]));
        PrintChar(' ');
    }
}

const char RODATA[] = {0x6A,0x69,0x69,0x69};
//un array de 1536 octets pour tester le dépassement de mémoire
const char DEPASSE[] = {0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42};
char DATA[] = {0x12,0x12,0x12};

int A = 0xcafe;


//kernel entry, called by extended_program.asm
extern "C" void _start(){
    InitIDT();
    //setLanguage(KBSet1::ScanCodeLookupTableAZERTY);
    MainKeyBoardHandler = KeyBoardHandler;
    MemoryMapEntry** UsableMemeoryMap = getUsableMemoryRegions();
    PrintAllMemoryMap(UsableMemeoryMap);
    InitializeHeap(0x100000, 0x100000);
    SetCursorPosition(PosFromCoord(0, 0));
    ClearScreen(BACKGROUND_BLACK);
    //initCmds();
    //return;

    //const char RODATAINSIDE[] = {0x58,0x58,0x58,0x58,0x58,0x58,0x58,0x58,0x58,0x58,0x58,0x58,0x58,0x58,0x58,0x58};
    const char RODATAINSIDE[] = "ZZZ";
    char data_inside[5] = {'A'};

    char* c1 = (char*)malloc(10);
    char* c2 = (char*)malloc(10);
    char* c3 = (char*)malloc(10);
    char* c4 = (char*)malloc(10);
    char* c5 = (char*)malloc(10);
    char* jmp = (char*)malloc(5);

    //mettre manuellement des valeurs dans les pointeurs
    //"Adresse de DATA"
    c1[0] = 'A';
    c1[1] = 'd';
    c1[2] = 'r';
    c1[3] = 'e';
    c1[4] = 's';
    c1[5] = 's';
    c1[6] = 'e';
    c1[7] = ' ';
    c1[8] = '\0';


    c2[0] = 'V';
    c2[1] = 'a';
    c2[2] = 'l';
    c2[3] = 'e';
    c2[4] = 'u';
    c2[5] = 'r';
    c2[6] = ' ';
    c2[7] = '\0';

    c3[0] = 'D';
    c3[1] = 'A';
    c3[2] = 'T';
    c3[3] = 'A';
    c3[4] = ':';
    c3[5] = ' ';
    c3[6] = '\0';

    c4[0] = 'R';
    c4[1] = 'O';
    c4[2] = 'D';
    c4[3] = 'A';
    c4[4] = 'T';
    c4[5] = 'A';
    c4[6] = ' ';
    c4[7] = '\0';

    c5[0] = 'R';
    c5[1] = 'O';
    c5[2] = 'D';
    c5[3] = 'I';
    c5[4] = 'N';
    c5[5] = 'S';
    c5[6] = 'I';
    c5[7] = 'D';
    c5[8] = ' ';
    c5[9] = '\0';
    
    jmp[0] = '\n';
    jmp[1] = '\r';
    jmp[2] = '\0';
    

    PrintString(c3); //DATA
    PrintString(jmp);
    PrintString(c1); //Adresse
    PrintString(jmp);
    PrintString(HexToString((uint_64)&A)); //0xf000
    PrintString(jmp);
    PrintString(c2); //valleur
    PrintString(IntToString(A)); //18
    PrintString(jmp);
    
    PrintString(c4); //RODATA
    PrintString(jmp);
    PrintString(c1); //Adresse
    PrintString(HexToString((uint_64)(&RODATA[0]))); //0x10080
    PrintString(jmp);
    PrintString(c2); //valleur
    PrintString(IntToString(RODATA[0])); //106
    PrintString(jmp);
    //const char* ptr = (char*)0x10080;
    const char* ptr = RODATA;
    /*if(*ptr == *(RODATA)){ //WHY IT WON'T HAPPEN ?????
        asm("cli");
        asm("hlt");
        while(1){}
    }*/
    PrintString(IntToString(*ptr)); //106? NON WTF ????
    PrintChar(*ptr);
    PrintString(jmp);

    PrintString(c5); //RODATAINSIDE
    PrintString(jmp);
    PrintString(c1); //Adresse
    PrintString(HexToString((uint_64)RODATAINSIDE)); //0x7ba2
    PrintString(jmp);
    PrintString(c2); //valleur
    PrintString(IntToString(RODATAINSIDE[0])); //90
    PrintString(jmp);


    

    /*searchInMemory();
    (*(VGA_MEMORY+20)) = 'F';
    (*(VGA_MEMORY+21)) = FOREGROUND_WHITE | BACKGROUND_BLACK;
    (*(VGA_MEMORY+22)) = 'I';
    (*(VGA_MEMORY+23)) = FOREGROUND_WHITE | BACKGROUND_BLACK;
    (*(VGA_MEMORY+24)) = 'N';
    (*(VGA_MEMORY+25)) = FOREGROUND_WHITE | BACKGROUND_BLACK;
    (*(VGA_MEMORY+26)) = 0;
    while(true){
        //on ne fait rien
    }*/


    SetCursorPosition(PosFromCoord(0, 18));
    PrintString(c3);PrintString(c5);PrintString(jmp);
    const char* slt = "Salut";
    PrintString(HexToString((uint_64)&slt[0])); //0x1065A
    PrintString(jmp);
    PrintChar('>');PrintChar((slt[0]));PrintChar('<');
    PrintString(jmp);
    PrintString(IntToString(slt[0]));
    
    PrintString(jmp);
    PrintString("Salut");

    SetCursorPosition(0);
    //memDmp((uint_8*)0x10070, 0x200); //0x200=512
    /*
    char* aa = (char*)malloc(sizeof(char) * 10);
    aa[0] = 'H';
    aa[1] = 'e';
    aa[2] = 'l';
    aa[3] = 'l';
    aa[4] = 'o';
    aa[5] = '\0';

    char* prout = (char*)malloc(sizeof(char)*256);
    //écrire 0x42 dans prout
    for(int i = 0; i < 256; i++){
        prout[i] = 0x42;
    }
    PrintString(HexToString((uint_64)prout));
    CursorPosition += VGA_WIDTH;
    CursorPosition -= CursorPosition % VGA_WIDTH;
    
    PrintString2(aa, BACKGROUND_RED | FOREGROUND_WHITE);
    PrintString(HexToString((uint_64)aa));

    //on saute une ligne
    CursorPosition += VGA_WIDTH;
    CursorPosition -= CursorPosition % VGA_WIDTH;

    const char* Test2 = "Neige";
    PrintString2(Test2, BACKGROUND_GREEN | FOREGROUND_WHITE);
    PrintString(HexToString((uint_64)Test2));

    free(aa);

    CursorPosition += VGA_WIDTH;
    CursorPosition -= CursorPosition % VGA_WIDTH;
    memDmp((uint_8*)0x8000, 0x200); //0x200=512


    */
    //on a tout check manuellement jusqu'a 0x2000
    while (true)
    {
        /* code */
    }
    




    //initTimer();
    //initFloppy();


    #ifndef VGA13
    //SetCursorPosition(PosFromCoord(0, 0));
    //PrintString(Test);
    /*
    PrintString("Salut\n\r");
    PrintString("Memory>\n\r");
    PrintAllMemoryMap(UsableMemeoryMap);
    PrintString("Fin memory<");
    */
    //endCmd();
    #endif

    //Print("ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZ");
    #ifdef VGA13
    Print("!\"#$%&\'()*+,-./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    //PlotImg((char *)relou, 262, 200);
    #endif

    //ClearScreen();
    //SetCursorPosition(0);
    //uint_8* img = 0;
    //img = (uint_8*)malloc(262*200*sizeof(uint_8));

    //uint_8* img_ptre = (uint_8*)Emi1;

    /*
    for(int i = 0; i<5; i++){
        while (*img_ptre != ',')
        {
            PrintChar(*img_ptre);
            img_ptre++;
        }
        PrintString("\n\r");
        img_ptre++;
    }
    */

    return;
}