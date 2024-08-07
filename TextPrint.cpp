#include "Header Files/TextPrint.h"
#include "Header Files/Kernel.h"
#include "Header Files/Render.h"


char hexToStringOutput[128];
template<typename T>
const char* HexToString(T value){
    T* valPtr = &value;
    uint_8* ptr;
    uint_8 temp;
    uint_8 size = (sizeof(T))*2 - 1;
    uint_8 i;
    for(i = 0; i < size; i++){
        ptr = (uint_8*)valPtr + i;
        temp = (*ptr & 0xF0) >> 4;
        hexToStringOutput[size - (i*2 +1)] = temp + (temp > 9 ? 55 : 48);
        temp = (*ptr & 0x0F);
        hexToStringOutput[size - (i*2)] = temp + (temp > 9 ? 55 : 48);
    }
    hexToStringOutput[size + 1] = 0;
    return hexToStringOutput;
}

const char* HexToString(uint_8 value) { return HexToString<uint_8>(value); }
const char* HexToString(uint_16 value) { return HexToString<uint_16>(value); }
const char* HexToString(uint_32 value) { return HexToString<uint_32>(value); }
const char* HexToString(uint_64 value) { return HexToString<uint_64>(value); }
const char* HexToString(char value) { return HexToString<char>(value); }
const char* HexToString(short value) { return HexToString<short>(value); }
const char* HexToString(int value) { return HexToString<int>(value); }
const char* HexToString(long long value) { return HexToString<long long>(value); }


char intToStringOutput[128];
template<typename T>
const char* IntToString(T value){
    uint_8 isNegative = 0;
    if(value < 0){
        isNegative = 1;
        value = -value;
        intToStringOutput[0] = '-';
    }

    uint_8 size = 0;
    uint_64 sizeTester = (uint_64)value;
    while(sizeTester / 10 > 0){
        sizeTester /= 10;
        size++;
    }

    uint_8 index = 0;
    uint_64 newValue = (uint_64)value;
    while(newValue / 10 > 0){
        uint_8 reminder = newValue % 10;
        newValue /= 10;
        intToStringOutput[size + isNegative - index] = reminder + 48;
        index++;
    }
    uint_8 reminder = newValue % 10;
    intToStringOutput[size + isNegative- index] = reminder + 48;

    intToStringOutput[size + isNegative+ 1] = 0;
    return intToStringOutput;
}

const char* IntToString(uint_8 value) { return IntToString<uint_8>(value); }
const char* IntToString(uint_16 value) { return IntToString<uint_16>(value); }
const char* IntToString(uint_32 value) { return IntToString<uint_32>(value); }
const char* IntToString(uint_64 value) { return IntToString<uint_64>(value); }
const char* IntToString(char value) { return IntToString<char>(value); }
const char* IntToString(short value) { return IntToString<short>(value); }
const char* IntToString(int value) { return IntToString<int>(value); }
const char* IntToString(long long value) { return IntToString<long long>(value); }

char floatToStringOutput[128];
const char* FloatToString(float value, uint_8 decimalPlaces){

    char* intPtr = (char*)IntToString((int)value);
    char* floatPtr = floatToStringOutput;

    if(value < 0){
        value *= -1;  
    }

    while (*intPtr != 0){
        *floatPtr = *intPtr;
        intPtr++;
        floatPtr++;
    }
    *floatPtr = '.';
    floatPtr++;

    float newValue = value - (int)value;

    for(int i = 0; i < decimalPlaces; i++){
        newValue *= 10;
        *floatPtr = (int)newValue + 48;
        newValue -= (int)newValue;
        floatPtr++;
    }

    *floatPtr = 0;
    return floatToStringOutput;
}

#ifndef VGA13
uint_16 CursorPosition;

void SetCursorPosition(uint_16 position){

    if(position > 2000){
        position = 2000;
    }
    if(position < 0){
        position = 0;
    }

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint_8)(position & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint_8)((position >> 8) & 0xFF));
    CursorPosition = position;
}

uint_16 PosFromCoord(uint_8 x, uint_8 y){
    return y * VGA_WIDTH + x;
}

void PrintString(const char* str, uint_8 color, uint_8 unused){
    char* charPtr = (char*)str;
    uint_16 index = CursorPosition;
    
    while(*charPtr !=0){
        switch (*charPtr)
        {
        case 10: //new line
            index += VGA_WIDTH;
            break;
        case 13: //carriage return
            index -= index%VGA_WIDTH;
            break;
        default:
            *(VGA_MEMORY + index * 2) = *charPtr;
            *(VGA_MEMORY + index *2 + 1) = color;
            index++;
            break;
        } 
        charPtr++;
    }
    SetCursorPosition(index);
}

void jmpLine(){
    SetCursorPosition(CursorPosition + VGA_WIDTH - CursorPosition % VGA_WIDTH);
}

void PrintChar(char chr, uint_8 color ){
    *(VGA_MEMORY + CursorPosition * 2) = chr;
    *(VGA_MEMORY + CursorPosition * 2 +1) = color;
    SetCursorPosition(CursorPosition + 1);
}

void ClearScreen(uint_8 ClearColor){
    for(uint_32 i = 0; i < 80*25; i++){
        *(VGA_MEMORY + i * 2) = 0;
        *(VGA_MEMORY + i *2 + 1) = ClearColor;
    }

}


#endif 

#ifdef VGA13
uint_16 CursorPosition;
void SetCursorPosition(uint_16 position){
    setCursorPosRenderer(position); //bad code mais c'est ma faute ça
}

uint_16 PosFromCoord(uint_8 x, uint_8 y){
    return y * VGA_13_W + x;
}

void PrintString(const char *str, uint_8 foreground, uint_8 background){
    Print(str, foreground, background);
}

void PrintChar(char chr, uint_8 color){

}

void ClearScreen(uint_8 ClearColor){
    uint_8 value = 0;
    //value += ClearColor << 8;
    //value += ClearColor << 24;
    //value += ClearColor << 48;
    //value += ClearColor << 56;
    for (uint_8* i = (uint_8*)VGA_OFFSET; i < (uint_8*)(VGA_OFFSET + 64000); i++){
        *i = value;
    }

}
#endif




