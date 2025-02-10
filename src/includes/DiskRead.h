#pragma once

#include "libc/stdtypes.h"
#include "TextPrint.h"
#include "cpu/idt.h"
#include "Heap.h"

extern "C" void ata_lba_read(uint64_t lba, uint8_t num_sectors, uint8_t* buffer);

void readDataATASector(uint64_t lba, uint8_t num_sectors, uint8_t* buffer);
void writeDataATASector(uint64_t lba, uint8_t num_sectors, uint8_t* buffer);

// TODO void resetDisk();

uint8_t get_status();

void readDataATA(uint64_t lba, uint64_t size, uint8_t* buffer);
void writeDataATA(uint64_t lba, uint64_t size, uint8_t* buffer);

void readDATA(uint64_t address, uint64_t size, uint8_t* buffer);
void writeDATA(uint64_t address, uint64_t size, uint8_t* buffer);