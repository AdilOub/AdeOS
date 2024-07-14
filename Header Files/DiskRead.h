#pragma once

#include "Typedefs.h"
#include "TextPrint.h"
#include "IDT.h"
#include "Heap.h"

extern "C" void ata_lba_read(uint_64 lba, uint_8 num_sectors, uint_8* buffer);

void readDataATA(uint_64 lba, uint_8 num_sectors, uint_8* buffer);
void writeDataATA(uint_64 lba, uint_8 num_sectors, uint_8* buffer);
void resetDisk();
uint_8 get_status();