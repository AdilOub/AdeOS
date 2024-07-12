#pragma once
#include "TextPrint.h"
#define MEMORY_COUNT_LOCATION 0x5000 //set dans DetectMemory.asm

extern uint_8 MemoryRegionCount;

void* memset(void* start, uint_64 value, uint_64 num);
void memcopy(void* destination, void* source, uint_64 num);


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

