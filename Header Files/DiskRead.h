#pragma once

#include "Typedefs.h"
#include "TextPrint.h"
#include "IDT.h"
#include "Heap.h"

extern "C" void ata_lba_read(uint_64 lba, uint_8 num_sectors, uint_8* buffer);

void readDataATASector(uint_64 lba, uint_8 num_sectors, uint_8* buffer);
void writeDataATASector(uint_64 lba, uint_8 num_sectors, uint_8* buffer);

// TODO void resetDisk();

uint_8 get_status();

void readDataATA(uint_64 lba, uint_64 size, uint_8* buffer);
void writeDataATA(uint_64 lba, uint_64 size, uint_8* buffer);