#include "Header Files/Compiler.h"

void exectue(uint8_t* asm_code){
    asm volatile ("call %0": : "r"(asm_code));
    return;
}

void print_with_compiler(){
    asm("nop");
}