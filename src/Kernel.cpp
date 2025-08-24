#include "includes/Kernel.h"

#include "includes/TextPrint.h"

#include "includes/cpu/idt.h"
#include "includes/cpu/io.h"

#include "includes/Memory.h"
#include "includes/Heap.h"
#include "includes/libc/stdtypes.h"
#include "includes/Timer.h"
#include "includes/DiskRead.h"
#include "includes/FileSystem.h"

//to use drawingggg -(currently useless & unused)
#include "includes/Render.h"

#include "includes/Commands.h"
#include "includes/FileSystem.h"

//compiler
#include "includes/Compiler.h"

//driver
#include "includes/drivers/Keyboard.h"
#include "includes/drivers/pci.h"
#include "includes/drivers/network/rtl8139.h"

#include "includes/usr/shell.h"

#define GET_RIP (uint32_t)({uint32_t rip; asm volatile ("lea (%%rip), %%eax\n\t" "mov %%eax, %0\n\t" : "=r"(rip)); rip;}) 


void var(int n, ...){
    __builtin_va_list args;
    __builtin_va_start(args, n);
    for(int i = 0; i < n; i++){
        int val = __builtin_va_arg(args, int);
        PrintString("Argument ");
        PrintString(IntToString(val));
        PrintString("\n");
    }
    __builtin_va_end(args);
}

//kernel entry, called by extended_program.asm
void afficher_rigolo(uint8_t a){
    asm("nop");
    uint64_t b = 0x69;
    Print(IntToString(a), FOREGROUND_LIGHTRED);
    return;
}



uint16_t bin;

void setup_file_system(){
    
    setup_root(); //attention, doit être utiliser qu'une seule fois !!!

    bin = create_folder_in_parent(0, "bin");
    create_folder_in_parent(0, "usr");

    uint8_t test_syscall_data[] = { 0xB8, 0x00, 0x00, 0x00, 0x00, 0xCD, 0x80, 0xC3, 0x01}; //mov eax, 0; int 0x80; ret trigger le syscall d'example
    uint64_t size = sizeof(test_syscall_data);
    uint16_t f = create_file_in_parent(bin, "testsyscall", (char*)test_syscall_data, size);
    create_file_in_parent(0, "secret.txt", "hello buddy what's up ? Still some work to do....", 50);
}

uint8_t keyboard_input[256];

void hellow(){
    Print("Helloow ^_^\n\r", FOREGROUND_LIGHTMAGENTA);
}


extern "C" void _start(){
    InitIDT();
    setLanguage(KBSet1::ScanCodeLookupTableAZERTY);
    MainKeyBoardHandler = KeyBoardHandler;
    InitializeHeap(0x100000, 0x100000);

    
    
    ClearScreen();
    SetCursorPosition(PosFromCoord(0, 0));
    //PrintString(Test);
    PrintString("Bienvenue sur "); PrintString("AdeOs", FOREGROUND_LIGHTGREEN); PrintString(" !\n\r\n\r");
    
    //Print("Test affichage:\n\r", FOREGROUND_LIGHTCYAN);
    //Print("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
    //Print("!\"#$%&\'()*+,-./0123456789\n\r\n\r");


    if(check_if_root()){
        Print("Root detected\n\r", FOREGROUND_LIGHTGREEN);
    }else{
        //Print("Root not detected\n\r", FOREGROUND_LIGHTRED);
        Print("Warning ! No root detected...\n\r", FOREGROUND_LIGHTRED);
        Print("Press any key to setup the root dir... (this will delete all data !)\n\r", FOREGROUND_LIGHTRED);
        /*while(getc()==0){
        }*/
        setup_file_system();
        
        Print("Press any key to add basic commands...\n\r", FOREGROUND_LIGHTGREEN);
        /*while(getc()==0){
        }*/add_basic_commands();
    
    }
    Print("Press any key to test pci devices.\n\r", FOREGROUND_LIGHTGREEN);
    while(getc()==0){
    }
    rtl8139_setup();
    printMACAddress();


    //Print("Everything is fine...\n\r", FOREGROUND_LIGHTGREEN);
    //const char* s = "aaa";
    startShell();

    




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

--> update en 2076: en gros c'était un probleme lié au option de compilation (mcmodel) et au fait qu'on lisait pas tout
On doit charger en mémoire le code du kernel dans le bios
Le kernel s'occupe ensuite de charger le file system (situé bien plus loin dans le disque) en mémoire, la où il trouve de la place
*/


/*
Idée du pseudo linker:
on rajoute les libs dans le fichier à compiler
Le compilateur s'occupe de charger l'addresse des fonctions dans les libs importante

*/


/*
Idée de chose todo:
-fix le filesystem
-ajouter commande pour lire/ecrire/creer des fichiers
-ajouter commande + interpreteur simple pour avoir du code hihi
-continuer de refactor et spérarer la notion de printf et "l'affichage" (pour changer de mode + facilement)
-passer en mode graphique (la denière fois vga13 était oktier mais moche, on va essayer de faire mieux avec du vbe maybe)
*/