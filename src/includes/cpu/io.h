#pragma once
#include "../libc/stdtypes.h"

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

#define PIC_EOI		0x20		/* End-of-interrupt command code */


void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);

void insw (uint16_t __port, void *__addr);
void outsw(uint16_t __port, void* __addr);



void RemapPic(uint16_t offset1, uint16_t offset2);
void PIC_sendEOI(uint8_t irq);