#pragma once
#include "io.h"

#include "../libc/stdtypes.h"
#include "../TextPrint.h"
#include "../drivers/network/netstack.h"

#ifndef PCI_H
#define PCI_H

#endif


struct IDT64{
    uint16_t offset_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t types_attr;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t zero;
};


extern void(*MainKeyBoardHandler)(uint8_t scanCode);
void InitIDT();

uint8_t get_flpydisk_int();
void set_flpydisk_int(uint8_t val);