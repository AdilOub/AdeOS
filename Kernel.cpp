#include "Header Files/Kernel.h"

 #include "Header Files/TextPrint.h"
 #include "Header Files/IDT.h"
 #include "Header Files/Keyboard.h"
 #include "Header Files/Memory.h"
 #include "Header Files/IO.h"
 #include "Header Files/Heap.h"
 #include "Header Files/Typedefs.h"
#include "Header Files/Timer.h"
#include "Header Files/DiskRead.h"
//#include "Header Files/FloppyDisk.h"


//to use drawingggg
#include "Header Files/Render.h"

#include "Header Files/Commands.h"


/*
TODO Important!!!
Il y a actuellement un problème avec le linker:
la section data ne semble pas être chargée correctement
et donc les variables globales non constante ne sont pas initialisées
(ce n'est pas le cas pour les variables globales constante)

*/


#define NULL 0

//64
int A = 0xcafe;

//kernel entry, called by extended_program.asm

extern "C" void _start(){
    
    InitIDT();
    setLanguage(KBSet1::ScanCodeLookupTableAZERTY);
    MainKeyBoardHandler = KeyBoardHandler;
    InitializeHeap(0x100000, 0x100000);
    SetCursorPosition(PosFromCoord(0, 0));
    initCmds();
    
    
    //on a tout check manuellement jusqu'a 0x2000




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
    ClearScreen();

    Print("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
    Print("!\"#$%&\'()*+,-./0123456789\n\r");
    //Print("lecture1: ");

    //software reset
    outb(0x1f6, 0x04);
    outb(0x1f6, 0x00);
    uint_8 status = inb(0x1f6);
    status = inb(0x1f6);
    status = inb(0x1f6);
    status = inb(0x1f6);
    Print("\n\rInfo:");
    Print(HexToString(status));


    Print("\n\rlecture1...");
    uint_8* buffer3 = (uint_8*)calloc(512*sizeof(uint_8));
    readDataATA(0, 1, buffer3);
    for(int i = 0; i<16; i++){
         Print(HexToString(buffer3[i]));
         Print(" ");
     }

    Print("\n\rlecture1 res: ");
    Print(HexToString(get_status()));

    Print("\n\rEcriture...");
    uint_8* buffer_write = (uint_8*)calloc(sizeof(uint_8)*512);
    for(int i = 0; i<256; i++){
        buffer_write[i] = i;
    }
    for(int i = 255; i<512; i++){
        buffer_write[i] = 0x69;
    }
    buffer_write[511]= 0x12;

    writeDataATA(80, 1, buffer_write);
    Print("\n\rEcriture resultat: ");
    Print(HexToString(get_status()));


    Print("\n\rlecture2...");
    uint_8* buffer4 = (uint_8*)calloc(512*sizeof(uint_8));
    readDataATA(80, 1, buffer4);
    for(int i = 0; i<16; i++){
         Print(HexToString(buffer4[i]));
         Print(" ");
     }
        Print("\n\rlecture 2 res: ");
    Print(HexToString(get_status()));


    //ATTENTION: APPELER LA FONCTION ata_lba_read (asm) CORROMP LE HEAP (on peut plus malloc après)

    // Print("encore: \n\r");

    // ata_lba_read(0, 1, buffer);
    // for(int i = 0; i<256; i++){
    //      Print(HexToString(buffer[i]));
    //      Print(" ");
    //  }


    
    //on va essayer de lire le disque 
    //on va lire le secteur 1 en utilisant le lecteur ATAPI

    
    

    while (true)
    {
        /* code */
    }
    


    return;
    
}





/*
(FAUX ce qui suit est un mensonge):séprarer la lecture du extended program et du code C dans le bios est inutile
donc autant tout lire d'un coup (VRAI?)
actuellement le fichier fait <0x7700 d'après hxD donc on peut tout lire d'un coup
maintenat il ne reste plus qu'a comprendre pourquoi un appel de fonction fait tout crash


ERRATUM:
lire d'un coup fonctionne pour les appels exterieur.
donc on a surement une erreur quand on lit en 2 fois
il faut comprendre cette erreur pour aficher le message de la fonction test :)

cf. disk_read.asm pour les explications.
*/