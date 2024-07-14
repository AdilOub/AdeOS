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



//eax: 32 bits
//ax: 16 bits
//ah/al: 8 bits

//numsector: nombre de secteur de 256 octets à lire
void readDataATA(uint_64 lba, uint_8 num_sectors, uint_8* buffer) {
    
    uint_32 masked_lba = lba & 0x0FFFFFFF;

    uint_32 portDrive = 0x01f6;
    uint_32 bit24_27 = (masked_lba >> 24) | 0b11100000;
    uint_8 bit24_27_8 = bit24_27 & 0xFF;
    outb(portDrive, bit24_27_8);

    uint_32 portNbSector = 0x01f2;
    outb(portNbSector, num_sectors);

    uint_32 portBit0_7 = 0x01f3;
    outb(portBit0_7, masked_lba & 0xFF);

    uint_32 portBit8_15 = 0x01f4;
    outb(portBit8_15, (masked_lba >> 8) & 0xFF);

    uint_32 portBit16_23 = 0x01f5;
    outb(portBit16_23, (masked_lba >> 16) & 0xFF);

    uint_32 portCommand = 0x01f7;
    outb(portCommand, 0x20);

    while(!(inb(portCommand) & 0x08));
    

    uint_32 dataPort = 0x1F0;
    uint_64 size = num_sectors * 256;
    uint_16* data = (uint_16*)malloc(2);

    for(uint_64 i = 0; i < size; i+=2){
        insw(dataPort, data); //reversed
        buffer[i+1] = ((*data&0xFF00)>>8);
        buffer[i] = (*data&0x00FF);
    }
    free(data);
    return;
    
}

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
    Print("lecture: ");
    uint_8* buffer = (uint_8*)calloc(512*sizeof(uint_8));
    readDataATA(0, 2, buffer);
    for(int i = 0; i<512; i++){
        Print(HexToString(buffer[i]));
        Print(" ");
    }
    free(buffer);
    
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