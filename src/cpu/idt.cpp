#include "../includes/cpu/IDT.h"

/*du nettoyage à faire ici
1- refaire la structure
2- gerer différement les Interrupt, Trap et Task
*/


extern IDT64 _idt[256];

//error handler 0 à 31
extern uint64_t isr0;
// //extern uint64_t isr1;
// extern uint64_t isr2;
// //extern uint64_t isr3;
// extern uint64_t isr4;
// extern uint64_t isr5;
extern uint64_t isr6;
// extern uint64_t isr7;
extern uint64_t isr8;
// extern uint64_t isr9;
// extern uint64_t isr10;
// extern uint64_t isr11;
// extern uint64_t isr12;
// extern uint64_t isr13;
extern uint64_t isr14;
// extern uint64_t isr16;
// extern uint64_t isr17;
// extern uint64_t isr18;
// extern uint64_t isr19;
// extern uint64_t isr20;
// extern uint64_t isr30;
// extern uint64_t isr31;





extern uint64_t isr32; //timer
extern uint64_t isr33; //keyboard

extern uint64_t isr44; //mouse ps2

extern uint64_t isr128; //software interupt, 128=0x80 arbritraire 


uint8_t tick = 0;

uint8_t flpydisk_int = 0;


extern "C" void LoadIDT();

void(*MainKeyBoardHandler)(uint8_t scanCode);


//uint64_t external_error_handler[32] = {isr0, 0, isr2, 0, isr4, isr5, isr6, isr7, isr8, isr9, isr10, isr11, isr12, isr13, isr14, isr15, isr16, isr17, isr18, isr19, isr20, isr21, isr22, isr23, isr24, isr25, isr26, isr27, isr28, isr29, isr30, isr31};

//uint64_t external_error_handler[32];

//array de fonction pour les erreurs

void InitIDT(){

    
    //div error 
    _idt[0].zero = 0;
	_idt[0].offset_low = (uint16_t)(((uint64_t)&isr0 & 0x000000000000ffff));
	_idt[0].offset_mid = (uint16_t)(((uint64_t)&isr0 & 0x00000000ffff0000) >> 16);
	_idt[0].offset_high = (uint32_t)(((uint64_t)&isr0 & 0xffffffff00000000) >> 32);
	_idt[0].ist = 0;
	_idt[0].selector = 0x08; 
	_idt[0].types_attr = 0x8f;

    //invalid op code
    _idt[6].zero = 0;
    _idt[6].offset_low = (uint16_t)(((uint64_t)&isr6 & 0x000000000000ffff));
    _idt[6].offset_mid = (uint16_t)(((uint64_t)&isr6 & 0x00000000ffff0000) >> 16);
    _idt[6].offset_high = (uint32_t)(((uint64_t)&isr6 & 0xffffffff00000000) >> 32);
    _idt[6].ist = 0;
    _idt[6].selector = 0x08;
    _idt[6].types_attr = 0x8f;


    //double faute, sera appelé par exemple si une interupt n'est pas géré
    _idt[8].zero = 0;
    _idt[8].offset_low = (uint16_t)(((uint64_t)&isr8 & 0x000000000000ffff));
    _idt[8].offset_mid = (uint16_t)(((uint64_t)&isr8 & 0x00000000ffff0000) >> 16);
    _idt[8].offset_high = (uint32_t)(((uint64_t)&isr8 & 0xffffffff00000000) >> 32);
    _idt[8].ist = 0;
    _idt[8].selector = 0x08;
    _idt[8].types_attr = 0x8f;

    //page fault
    _idt[14].zero = 0;
    _idt[14].offset_low = (uint16_t)(((uint64_t)&isr14 & 0x000000000000ffff));
    _idt[14].offset_mid = (uint16_t)(((uint64_t)&isr14 & 0x00000000ffff0000) >> 16);
    _idt[14].offset_high = (uint32_t)(((uint64_t)&isr14 & 0xffffffff00000000) >> 32);
    _idt[14].ist = 0;
    _idt[14].selector = 0x08;
    _idt[14].types_attr = 0x8f;

    


//    for(uint8_t i = 0; i < 32; i++){
//         if(i != 0 || i!= 8){
//             continue;
//         }
//        _idt[i].zero = 0;
//        _idt[i].offset_low = (uint16_t)(((uint64_t)external_error_handler[i] & 0x000000000000ffff));
//        _idt[i].offset_mid = (uint16_t)(((uint64_t)external_error_handler[i] & 0x00000000ffff0000) >> 16);
//        _idt[i].offset_high = (uint32_t)(((uint64_t)external_error_handler[i] & 0xffffffff00000000) >> 32);
//        _idt[i].ist = 0;
//        _idt[i].selector = 0x08;
//        _idt[i].types_attr = 0x8f;
//    }       


    //timer
    _idt[32].zero = 0;
	_idt[32].offset_low = (uint16_t)(((uint64_t)&isr32 & 0x000000000000ffff));
	_idt[32].offset_mid = (uint16_t)(((uint64_t)&isr32 & 0x00000000ffff0000) >> 16);
	_idt[32].offset_high = (uint32_t)(((uint64_t)&isr32 & 0xffffffff00000000) >> 32);
	_idt[32].ist = 0;
	_idt[32].selector = 0x08; 
	_idt[32].types_attr = 0x8e;


    //keyboard
	_idt[33].zero = 0;
	_idt[33].offset_low = (uint16_t)(((uint64_t)&isr33 & 0x000000000000ffff));
	_idt[33].offset_mid = (uint16_t)(((uint64_t)&isr33 & 0x00000000ffff0000) >> 16);
	_idt[33].offset_high = (uint32_t)(((uint64_t)&isr33 & 0xffffffff00000000) >> 32);
	_idt[33].ist = 0;
	_idt[33].selector = 0x08;
	_idt[33].types_attr = 0x8e;
    

    //mouse
    _idt[44].zero = 0;
    _idt[44].offset_low = (uint16_t)(((uint64_t)&isr44 & 0x000000000000ffff));
    _idt[44].offset_mid = (uint16_t)(((uint64_t)&isr44 & 0x00000000ffff0000) >> 16);
    _idt[44].offset_high = (uint32_t)(((uint64_t)&isr44 & 0xffffffff00000000) >> 32);
    _idt[44].ist = 0;
    _idt[44].selector = 0x08;
    _idt[44].types_attr = 0x8e;



    //software interupt
    _idt[128].zero = 0;
    _idt[128].offset_low = (uint16_t)(((uint64_t)&isr128 & 0x000000000000ffff));
    _idt[128].offset_mid = (uint16_t)(((uint64_t)&isr128 & 0x00000000ffff0000) >> 16);
    _idt[128].offset_high = (uint32_t)(((uint64_t)&isr128 & 0xffffffff00000000) >> 32);
    _idt[128].ist = 0;
    _idt[128].selector = 0x08;
    _idt[128].types_attr = 0x8e;
    


    //on remap les interupts pour laisser la place aux interupts du cpu
    //irq 0 à 7 -> 0x20 à 0x27
    //irq 8 à 15 -> 0x28 à 0x2f
    RemapPic(0x20, 0x28); //le probleme c'est les interupts 0 à 7 sont déjà utilisé par les erreurs du cpu dont on va les remapper


    //TODO reactivé les interupts
    outb(0x21, 0xf9); //11111001 on n'autorise que ~~l'irq0~~ et l'irq1 (~~timer~~ et clavier) + irq2 (cascade, pour utiliser le slave)
    outb(0xa1, 0xef); //et la souris sur le slave

    //outb(0x28, 0xf7); //11110111 on n'autorise que l'irq11 (mouse)


    //on active les interupts
    asm("sti");

    LoadIDT();
}


/*

HARDWARE INTERUPTS

*/



//timer remap !
extern "C" void isr32_handler(){    
    asm("cli");
    outb(0x20, 0x20); //on envoie un eoi

    asm("sti");


    //PrintString("isr32 called\n\r", BACKGROUND_BLUE);
    
    //on va faire clignoter un point en haut à droite
    uint16_t pos = PosFromCoord(79, 0);
    if(tick%8 == 0){
        ((uint16_t*)0xb8000)[pos] = 0x0f00 | '.';
    }else if (tick%8 == 4){
        ((uint16_t*)0xb8000)[pos] = 0x0f00 | ' ';
    }


    tick++;
    if(tick == 255){
        tick = 0;
    }

}




//clavier, irq1 donc 0x21 ie 33 
extern "C" void isr33_handler(){
    asm("cli");

    uint8_t scanCode = inb(0x60);

    if(scanCode == 00 || scanCode == 0xff){
        outb(0x20, 0x20);
        asm("sti");
        return;
    }
    
    if(scanCode < 0x80){
        keyboard_input[scanCode] = 1;
        keyboard_input[256] = 1;
    }else{
        keyboard_input[scanCode - 0x80] = 0;
        keyboard_input[256] = 0;
    }
    
    if(MainKeyBoardHandler != 0){
        MainKeyBoardHandler(scanCode);
    }else{
        PrintString("No Keyboard Handler\n\r", BACKGROUND_RED);
    }    
    outb(0x20, 0x20);
    asm("sti");

}

//irq6, floppy disk
//on utilise le mode DMA, donc l'interupt est raise que à l'init, et au début de read & write
extern "C" void isr38_handler(){
    outb(0x20, 0x20);
    outb(0xa0, 0x20);
}

//mouse
int16_t mouse_dx = 0;
int16_t mouse_dy = 0;
char ptr = 158;

uint16_t inv_sensibility = 1;

int32_t mouseX = 0;
int32_t mouseY = 0;

int32_t clamp(int32_t val, int32_t min, int32_t max){
    if(val < min){
        return min;
    }else if(val > max){
        return max;
    }else{
        return val;
    }
}

int16_t linearConversion(int16_t val, int16_t min, int16_t max, int16_t new_min, int16_t new_max){
    float v = (float)((val - min) * (new_max - new_min)) / (float)(max - min) + (float)new_min;
    return (int16_t)v;
}

int32_t min(int32_t a, int32_t b){
    return a < b ? a : b;
}

extern "C" void isr44_handler(){
    asm("cli");
    uint16_t old_pos = PosFromCoord(mouseX, mouseY);

    //PrintString("isr44 called\n\rScanCode: ", BACKGROUND_BLUE);
    uint8_t pack1 = inb(0x60);
    uint8_t pack2 = inb(0x60);
    uint8_t pack3 = inb(0x60);

    if(pack1 & 0b11000000){ //overflow, on ignore le packet
        outb(0x20, 0x20);
        outb(0xa0, 0x20);
        asm("sti");
        return;
    }

    if(pack1 & 0b10){ //right clicked

        SetCursorPosition(0);
        ClearScreen();
    }

    if(pack1 & 0b01){ //left click
        mouseX = 40;
        mouseY = 12;
    }


    // PrintString(HexToString(pack1), BACKGROUND_MAGENTA);
    // PrintString(" ", BACKGROUND_BLUE);
    // PrintString(HexToString(pack2), BACKGROUND_MAGENTA);
    // PrintString(" ", BACKGROUND_BLUE);    
    // PrintString(HexToString(pack3), BACKGROUND_MAGENTA);
    // PrintString("\n\r", BACKGROUND_BLUE);

    int8_t signX = pack1 & 0b00010000 ? -1 : 1;
    int8_t signY = pack1 & 0b00100000 ? -1 : 1;


    mouse_dx = pack2 & 0xFF; 
    mouse_dy = pack3 & 0xFF;
    if(signX == -1){
        mouse_dx = pack2 | 0xFF00;
    }
    if(signY == -1){
        mouse_dy = pack3 | 0xFF00;
    }

    mouse_dx=clamp(mouse_dx, -254, 254);
    mouse_dy=clamp(mouse_dy, -254, 254);

    // mouse_dx=((int16_t)pack2)*signX;
    // mouse_dy= (pack3)*signY;
    
    //mouseX = clamp(mouseX+mouse_dx, -10000, 10000);
    //mouseY = clamp(mouseY-mouse_dy, -10000, 10000);



    //putting char on screen


    
        //PrintString(HexToString(pack1));    
        PrintString("\n\rMouse mov x: ");PrintString(IntToString(mouse_dx));PrintString(" y: ");PrintString(IntToString(mouse_dy));PrintString("   ");
        //PrintString("\n\rMouse pos x: ");PrintString(IntToString(mouseX));PrintString(" y: ");PrintString(IntToString(mouseY));PrintString("            ");
        
        int16_t x_in_range = linearConversion(mouse_dx, -254, 254, -40, 40);
        int16_t y_in_range = linearConversion(mouse_dy, -254, 254, -12, 12);
        PrintString(" Mouse inr x: ");PrintString(IntToString(x_in_range));PrintString(" y: ");PrintString(IntToString(y_in_range));PrintString("            ");
        
        mouseX = clamp(mouseX+x_in_range, 0, 79);
        mouseY = clamp(mouseY-y_in_range, 0, 24);

        uint16_t pos = PosFromCoord(mouseX, mouseY);
        ((uint16_t*)0xb8000)[old_pos] = 0x0f00 | ' ';
        ((uint16_t*)0xb8000)[pos] = 0x0f00 | ptr;
    

    

    outb(0x20, 0x20);
    outb(0xa0, 0x20);
    asm("sti");
}

/*
td;dr: on initialise les interupts avec InitIDT (qui va remapper les inturupts materiels avec le bon offset, merci ibm)
 et activer les interupts avec le bon mask (0xfc ici pour n'autoriser que l'irq0 et l'irq1)
*/

uint8_t get_flpydisk_int(){
    return flpydisk_int;
}
void set_flpydisk_int(uint8_t val){
    flpydisk_int = val;
}


#pragma region error_handler



//software interupt
uint64_t handle_syscall(uint64_t syscall);

extern "C" void isr128_handler(){
    asm("cli");
    uint64_t syscall = 0;
    asm("mov %%rax, %0" : "=r"(syscall) : : "rax");
    if(syscall == 0){
        PrintString("syscall 0\n\r", FOREGROUND_LIGHTMAGENTA);
    }else{
        handle_syscall(syscall);
    }
    outb(0x20, 0x20);
    outb(0xa0, 0x20);
    asm("sti");

}

uint64_t handle_syscall(uint64_t syscall){
    uint64_t arg1 = 0;
    switch (syscall)
    {
        case 1: //print, address is in rbx
            asm("mov %%rbx, %0" : "=r"(arg1) : : "rbx");
            PrintString("syscall 1\n\r", BACKGROUND_BLUE);
            PrintString(HexToString(arg1));
            PrintString("\n\r", BACKGROUND_BLUE);
            PrintString((char*)arg1);
            PrintString("\n\ri am done\n\r", BACKGROUND_BLUE | FOREGROUND_RED);

            break;
        default:
            PrintString("syscall default\n\r", BACKGROUND_RED);
            break;
    }

    return 0;
}






/*

CPU ERROR HANDLER

*/



// //code généré en python

//div error
extern "C" void isr0_handler(){
    asm("cli");
    uint64_t this_isr = (uint64_t)((void*)&isr0);
    PrintString("\n\n\r isr0 funadd: ");
    PrintString(HexToString(this_isr));
    SetCursorPosition(0);
    PrintString("Kernel panic: div error\n\r", BACKGROUND_RED | FOREGROUND_WHITE);
    //c'est surement grave
    //PIC_sendEOI(0);
    asm("hlt");
}

// /*
// extern "C" void isr1_handler(){
//     SetCursorPosition(0);asm("cli");
//     PrintString("Kernel panic: debug exception\n\r", BACKGROUND_RED | FOREGROUND_WHITE);
//     //c'est surement grave
//     asm("hlt");
// }
// */

// extern "C" void isr2_handler(){
//     SetCursorPosition(0);asm("cli");
//     PrintString("Kernel panic: non maskable interrupt\n\r", BACKGROUND_RED | FOREGROUND_WHITE);
//     //c'est surement grave
//     asm("hlt");
// }

// /*
// extern "C" void isr3_handler(){
//     SetCursorPosition(0);asm("cli");
//     PrintString("Kernel panic: breakpoint\n\r", BACKGROUND_RED | FOREGROUND_WHITE);
//     //c'est surement grave
//     asm("hlt");
// }
// */

// extern "C" void isr4_handler(){
//     SetCursorPosition(0);asm("cli");
//     PrintString("Kernel panic: overflow\n\r", BACKGROUND_RED | FOREGROUND_WHITE);
//     //c'est surement grave
//     asm("hlt");
// }

// extern "C" void isr5_handler(){
//     SetCursorPosition(0);asm("cli");
//     PrintString("Kernel panic: bound range exceeded\n\r", BACKGROUND_RED | FOREGROUND_WHITE);
//     //c'est surement grave
//     asm("hlt");
// }

extern "C" void isr6_handler(){
    asm("cli");
    SetCursorPosition(0);
    PrintString("Kernel panic: invalid opcode\n\r", BACKGROUND_RED | FOREGROUND_WHITE);
    //c'est surement grave
    asm("hlt");
}

// extern "C" void isr7_handler(){
//     SetCursorPosition(0);asm("cli");
//     PrintString("Kernel panic: device not available\n\r", BACKGROUND_RED | FOREGROUND_WHITE);
//     //c'est surement grave
//     asm("hlt");
// }

//double faute (nottament les interupts non géré)
//todo faire un dump
extern "C" void isr8_handler(){
    asm("cli");
    SetCursorPosition(0);
    PrintString("Kernel panic: double fault :c\n\r", BACKGROUND_RED | FOREGROUND_WHITE);
    //c'est très grave
    asm("hlt");
}

// extern "C" void isr9_handler(){
//     SetCursorPosition(0);asm("cli");
//     PrintString("Kernel panic: coprocessor segment overrun\n\r", BACKGROUND_RED | FOREGROUND_WHITE);
//     //c'est surement grave
//     asm("hlt");
// }

// extern "C" void isr10_handler(){
//     SetCursorPosition(0);asm("cli");
//     PrintString("Kernel panic: invalid TSS\n\r", BACKGROUND_RED | FOREGROUND_WHITE);
//     //c'est surement grave
//     asm("hlt");
// }

// extern "C" void isr11_handler(){
//     SetCursorPosition(0);asm("cli");
//     PrintString("Kernel panic: segment not present\n\r", BACKGROUND_RED | FOREGROUND_WHITE);
//     //c'est surement grave
//     asm("hlt");
// }

// extern "C" void isr12_handler(){
//     SetCursorPosition(0);asm("cli");
//     PrintString("Kernel panic: stack-segment fault\n\r", BACKGROUND_RED | FOREGROUND_WHITE);
//     //c'est surement grave
//     asm("hlt");
// }

// extern "C" void isr13_handler(){
//     SetCursorPosition(0);asm("cli");
//     PrintString("Kernel panic: general protection fault\n\r", BACKGROUND_RED | FOREGROUND_WHITE);
//     //c'est surement grave
//     asm("hlt");
// }

extern "C" void isr14_handler(){
    asm("cli");
    SetCursorPosition(0);
    PrintString("Kernel panic: page fault\n\r", BACKGROUND_RED | FOREGROUND_WHITE);
    //c'est surement grave
    asm("hlt");
}

// i think we need to enable this one for backward compatibility (cf. le wiki comme d'hab)
// extern "C" void isr16_handler(){
//     SetCursorPosition(0);asm("cli");
//     PrintString("Kernel panic: x87 FPU floating-point error\n\r", BACKGROUND_RED | FOREGROUND_WHITE);
//     //c'est surement grave
//     asm("hlt");
// }

// extern "C" void isr17_handler(){
//     SetCursorPosition(0);asm("cli");
//     PrintString("Kernel panic: alignment check\n\r", BACKGROUND_RED | FOREGROUND_WHITE);
//     //c'est surement grave
//     asm("hlt");
// }

// extern "C" void isr18_handler(){
//     SetCursorPosition(0);asm("cli");
//     PrintString("Kernel panic: machine check\n\r", BACKGROUND_RED | FOREGROUND_WHITE);
//     //c'est surement grave
//     asm("hlt");
// }

// extern "C" void isr19_handler(){
//     SetCursorPosition(0);asm("cli");
//     PrintString("Kernel panic: SIMD floating-point exception\n\r", BACKGROUND_RED | FOREGROUND_WHITE);
//     //c'est surement grave
//     asm("hlt");
// }

// extern "C" void isr20_handler(){
//     SetCursorPosition(0);asm("cli");
//     PrintString("Kernel panic: virtualization exception\n\r", BACKGROUND_RED | FOREGROUND_WHITE);
//     //c'est surement grave
//     asm("hlt");
// }

// extern "C" void isr21_handler(){
//     SetCursorPosition(0);asm("cli");
//     PrintString("Kernel panic: control protection exception\n\r", BACKGROUND_RED | FOREGROUND_WHITE);
//     //c'est surement grave
//     asm("hlt");
// }

// extern "C" void isr30_handler(){
//     SetCursorPosition(0);asm("cli");
//     PrintString("Kernel panic: security exception\n\r", BACKGROUND_RED | FOREGROUND_WHITE);
//     //c'est surement grave
//     asm("hlt");
// }

#pragma endregion error_handler