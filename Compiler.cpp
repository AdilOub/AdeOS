#include "Header Files/Compiler.h"

void exectue(uint_8* asm_code){
    asm volatile ("call %0": : "r"(asm_code));
    return;
}

