#include "Header Files/TextPrint.h"
#include "Header Files/Kernel.h"
#include "Header Files/Render.h"


char hexToStringOutput[128];
template<typename T>
const char* HexToString(T value){
    T* valPtr = &value;
    uint8_t* ptr;
    uint8_t temp;
    uint8_t size = (sizeof(T))*2 - 1;
    uint8_t i;
    for(i = 0; i < size; i++){
        ptr = (uint8_t*)valPtr + i;
        temp = (*ptr & 0xF0) >> 4;
        hexToStringOutput[size - (i*2 +1)] = temp + (temp > 9 ? 55 : 48);
        temp = (*ptr & 0x0F);
        hexToStringOutput[size - (i*2)] = temp + (temp > 9 ? 55 : 48);
    }
    hexToStringOutput[size + 1] = 0;
    return hexToStringOutput;
}

const char* HexToString(uint8_t value) { return HexToString<uint8_t>(value); }
const char* HexToString(uint16_t value) { return HexToString<uint16_t>(value); }
const char* HexToString(uint32_t value) { return HexToString<uint32_t>(value); }
const char* HexToString(uint64_t value) { return HexToString<uint64_t>(value); }
const char* HexToString(char value) { return HexToString<char>(value); }
const char* HexToString(short value) { return HexToString<short>(value); }
const char* HexToString(int value) { return HexToString<int>(value); }
const char* HexToString(long long value) { return HexToString<long long>(value); }


char intToStringOutput[128];
template<typename T>
const char* IntToString(T value){
    uint8_t isNegative = 0;
    if(value < 0){
        isNegative = 1;
        value = -value;
        intToStringOutput[0] = '-';
    }

    uint8_t size = 0;
    uint64_t sizeTester = (uint64_t)value;
    while(sizeTester / 10 > 0){
        sizeTester /= 10;
        size++;
    }

    uint8_t index = 0;
    uint64_t newValue = (uint64_t)value;
    while(newValue / 10 > 0){
        uint8_t reminder = newValue % 10;
        newValue /= 10;
        intToStringOutput[size + isNegative - index] = reminder + 48;
        index++;
    }
    uint8_t reminder = newValue % 10;
    intToStringOutput[size + isNegative- index] = reminder + 48;

    intToStringOutput[size + isNegative+ 1] = 0;
    return intToStringOutput;
}

const char* IntToString(uint8_t value) { return IntToString<uint8_t>(value); }
const char* IntToString(uint16_t value) { return IntToString<uint16_t>(value); }
const char* IntToString(uint32_t value) { return IntToString<uint32_t>(value); }
const char* IntToString(uint64_t value) { return IntToString<uint64_t>(value); }
const char* IntToString(char value) { return IntToString<char>(value); }
const char* IntToString(short value) { return IntToString<short>(value); }
const char* IntToString(int value) { return IntToString<int>(value); }
const char* IntToString(long long value) { return IntToString<long long>(value); }

char floatToStringOutput[128];
const char* FloatToString(float value, uint8_t decimalPlaces){

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
uint16_t CursorPosition;

void SetCursorPosition(uint16_t position){

    if(position > 2000){
        position = 2000;
    }
    if(position < 0){
        position = 0;
    }

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(position & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((position >> 8) & 0xFF));
    CursorPosition = position;
}

uint16_t PosFromCoord(uint8_t x, uint8_t y){
    return y * VGA_WIDTH + x;
}

void PrintString(const char* str, uint8_t color, uint8_t unused){
    char* charPtr = (char*)str;
    uint16_t index = CursorPosition;
    
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

void PrintChar(char chr, uint8_t color ){
    *(VGA_MEMORY + CursorPosition * 2) = chr;
    *(VGA_MEMORY + CursorPosition * 2 +1) = color;
    SetCursorPosition(CursorPosition + 1);
}

void ClearScreen(uint8_t ClearColor){
    for(uint32_t i = 0; i < 80*25; i++){
        *(VGA_MEMORY + i * 2) = 0;
        *(VGA_MEMORY + i *2 + 1) = ClearColor;
    }

}


#endif 

#ifdef VGA13
uint16_t CursorPosition;
void SetCursorPosition(uint16_t position){
    setCursorPosRenderer(position); //bad code mais c'est ma faute ça
}

uint16_t PosFromCoord(uint8_t x, uint8_t y){
    return y * VGA_13_W + x;
}

void PrintString(const char *str, uint8_t foreground, uint8_t background){
    Print(str, foreground, background);
}

void PrintChar(char chr, uint8_t color){

}

void ClearScreen(uint8_t ClearColor){
    uint8_t value = 0;
    //value += ClearColor << 8;
    //value += ClearColor << 24;
    //value += ClearColor << 48;
    //value += ClearColor << 56;
    for (uint8_t* i = (uint8_t*)VGA_OFFSET; i < (uint8_t*)(VGA_OFFSET + 64000); i++){
        *i = value;
    }

}
#endif




