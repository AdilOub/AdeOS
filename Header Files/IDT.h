#pragma once
#include "Kernel.h"
#include "Typedefs.h"
#include "IO.h"
#include "TextPrint.h"
#include "KBScanCodeSet1.h"
#include "DiskRead.h"

#define PIC 0x20
#define PIC2 0xA0
#define PIC1_CMD PIC1
#define PIC1_DATA (PIC1 + 1)
#define PIC2_CMD PIC2
#define PIC2_DATA (PIC2 + 1)

#define PIC_EOI 0x20

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