#pragma once
#include "Typedefs.h"

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

#define PIC_EOI		0x20		/* End-of-interrupt command code */


void outb(uint_16 port, uint_8 val);
uint_8 inb(uint_16 port);

void insw (unsigned short int __port, void *__addr);

void RemapPic(uint_16 offset1, uint_16 offset2);