#pragma once
#include "../Kernel.h"
#include "../Commands.h"
#include "../TextPrint.h"
#include "../cpu/idt.h"
#include "../Heap.h"
#include "../Memory.h"
#include "../cpu/io.h"


uint16_t pciConfigReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);

void testPCI();

uint16_t getBar0(uint8_t bus, uint8_t device, uint8_t function);
uint8_t getInterruptLine(uint8_t bus, uint8_t device, uint8_t function);
uint8_t getInterruptPin(uint8_t bus, uint8_t device, uint8_t function);


#define RTL8139_VENDOR_ID 0x10EC
#define RTL8139_DEVICE_ID 0x8139

//pour l'instant on hardcode... todo: faire un truc plus propre
#define _RTL8139_BUS 0
#define _RTL8139_DEVICE 4


uint16_t getCommand(uint8_t bus, uint8_t device, uint8_t function) ;
void setCommand(uint8_t bus, uint8_t device, uint8_t function, uint16_t command) ;