#pragma once
#include "TextPrint.h"
#define MEMORY_COUNT_LOCATION 0x5000

extern uint_8 MemoryRegionCount;

void* memset(void* dest, unsigned char val, int count);

struct  MemoryMapEntry
{
    uint_64 BaseAdress;
    uint_64 RegionLength;
    uint_32 RegionType;
    uint_32 ExtendedAttributes;
};

void PrintMemoryMap(MemoryMapEntry* memoryMap, uint_16 position);

extern MemoryMapEntry* UsableMemoryRegion[10];
extern uint_8 UsableMemeoryRegionCount;

MemoryMapEntry** getUsableMemoryRegions();