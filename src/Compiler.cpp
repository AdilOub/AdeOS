#include "includes/Compiler.h"

void exectue(uint8_t* asm_code){
    asm volatile ("call %0": : "r"(asm_code)); //TODO rajouter '*' devant %0
    return;
}

void print_with_compiler(){
    asm("nop");
}