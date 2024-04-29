#include "Header Files/IDT.h"



extern IDT64 _idt[256];
extern uint_64 isr0;
extern uint_64 isr8;

extern uint_64 isr33;
extern uint_64 isr32;

extern uint_64 isr38;

uint_8 tick = 0;

uint_8 flpydisk_int = 0;


extern "C" void LoadIDT();

void(*MainKeyBoardHandler)(uint_8 scanCode);

void InitIDT(){

    //div error 
    _idt[0].zero = 0;
	_idt[0].offset_low = (uint_16)(((uint_64)&isr0 & 0x000000000000ffff));
	_idt[0].offset_mid = (uint_16)(((uint_64)&isr0 & 0x00000000ffff0000) >> 16);
	_idt[0].offset_high = (uint_32)(((uint_64)&isr0 & 0xffffffff00000000) >> 32);
	_idt[0].ist = 0;
	_idt[0].selector = 0x08; 
	_idt[0].types_attr = 0x8f; 

    //double faute, sera appelé par exemple si une interupt n'est pas géré
    _idt[8].zero = 0;
    _idt[8].offset_low = (uint_16)(((uint_64)&isr8 & 0x000000000000ffff));
    _idt[8].offset_mid = (uint_16)(((uint_64)&isr8 & 0x00000000ffff0000) >> 16);
    _idt[8].offset_high = (uint_32)(((uint_64)&isr8 & 0xffffffff00000000) >> 32);
    _idt[8].ist = 0;
    _idt[8].selector = 0x08;
    _idt[8].types_attr = 0x8f;



    //timer
    _idt[32].zero = 0;
	_idt[32].offset_low = (uint_16)(((uint_64)&isr32 & 0x000000000000ffff));
	_idt[32].offset_mid = (uint_16)(((uint_64)&isr32 & 0x00000000ffff0000) >> 16);
	_idt[32].offset_high = (uint_32)(((uint_64)&isr32 & 0xffffffff00000000) >> 32);
	_idt[32].ist = 0;
	_idt[32].selector = 0x08; 
	_idt[32].types_attr = 0x8e;


    //keyboard
	_idt[33].zero = 0;
	_idt[33].offset_low = (uint_16)(((uint_64)&isr33 & 0x000000000000ffff));
	_idt[33].offset_mid = (uint_16)(((uint_64)&isr33 & 0x00000000ffff0000) >> 16);
	_idt[33].offset_high = (uint_32)(((uint_64)&isr33 & 0xffffffff00000000) >> 32);
	_idt[33].ist = 0;
	_idt[33].selector = 0x08;
	_idt[33].types_attr = 0x8e;
    
    //floppy disk irq6 donc 0x26 ie 38
    _idt[38].zero = 0;
    _idt[38].offset_low = (uint_16)(((uint_64)&isr38 & 0x000000000000ffff));
    _idt[38].offset_mid = (uint_16)(((uint_64)&isr38 & 0x00000000ffff0000) >> 16);
    _idt[38].offset_high = (uint_32)(((uint_64)&isr38 & 0xffffffff00000000) >> 32);
    _idt[38].ist = 0;
    _idt[38].selector = 0x08;
    _idt[38].types_attr = 0x8e;

    


    //on remap les interupts
    //irq 0 à 7 -> 0x20 à 0x27
    //irq 8 à 15 -> 0x28 à 0x2f
    RemapPic(0x20, 0x28); //le probleme c'est les interupts 0 à 7 sont déjà utilisé par les erreurs du cpu dont on va les remapper


    //outb(0x21, 0xfd); //11111101 on n'autorise que l'irq1
    //outb(0x21, 0xfc); //11111100 on n'autorise que l'irq0 et l'irq1
    outb(0x21, 0xbc); //10111100 irq0, irq1 et irq6
    outb(0xa1, 0xff); //slave pic

    //on active les interupts
    asm("sti");

    LoadIDT();
}


//div error
extern "C" void isr0_handler(){
    PrintString("Kernel panic: div error\n\r", BACKGROUND_RED | FOREGROUND_WHITE);
    //c'est surement grave
    asm("hlt");
}

//double faute (nottament les interupts non géré)
//todo faire un dump
extern "C" void isr8_handler(){
    PrintString("Kernel panic: double fault :c\n\r", BACKGROUND_RED | FOREGROUND_WHITE);
    //c'est très grave
    asm("hlt");
}


//timer
extern "C" void isr32_handler(){
    //on va faire clignoter un point en haut à droite
    uint_16 pos = PosFromCoord(79, 0);
    if(tick%8 == 0){
        ((uint_16*)0xb8000)[pos] = 0x0f00 | '.';
    }else if (tick%8 == 4){
        ((uint_16*)0xb8000)[pos] = 0x0f00 | ' ';
    }

    tick++;
    if(tick == 255){
        tick = 0;
    }

    outb(0x20, 0x20); //on envoie un eoi
    outb(0xa0, 0x20);
}


//clavier, irq1 donc 0x21 ie 33
extern "C" void isr33_handler(){
    asm("cli");
    //PrintString("isr1", BACKGROUND_BLUE);
    uint_8 scanCode = inb(0x60);
    //uint_8 chr = 0;
    //if(scanCode < 0x3A){
    //    chr = KBSet1::ScanCodeLookupTableQWERTY[scanCode];
    //}
    
    if(MainKeyBoardHandler != 0){
        MainKeyBoardHandler(scanCode);
    }else{
        PrintString("No Keyboard Handler\n\r", BACKGROUND_RED);
    }
    asm("sti");
    outb(0x20, 0x20);
    outb(0xa0, 0x20);

}

//irq6, floppy disk
//on utilise le mode DMA, donc l'interupt est raise que à l'init, et au début de read & write
extern "C" void isr38_handler(){
    asm("cli");
    PrintString("isr38 called\n\r", BACKGROUND_BLUE);
    flpydisk_int = 1;
    asm("sti");
    outb(0x20, 0x20);
    outb(0xa0, 0x20);
}


/*
td;dr: on initialise les interupts avec InitIDT (qui va remapper les inturupts materiels avec le bon offset, merci ibm)
 et activer les interupts avec le bon mask (0xfc ici pour n'autoriser que l'irq0 et l'irq1)
*/

uint_8 get_flpydisk_int(){
    return flpydisk_int;
}
void set_flpydisk_int(uint_8 val){
    flpydisk_int = val;
}