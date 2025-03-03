#pragma once
#include "TextPrint.h"
#define MEMORY_COUNT_LOCATION 0xC000 //set dans DetectMemory.asm

extern uint8_t MemoryRegionCount;

void* memset(void* start, uint8_t value, uint64_t num);
void memcopy(void* destination, void* source, uint64_t num);


struct  MemoryMapEntry
{
    uint64_t BaseAdress;
    uint64_t RegionLength;
    uint32_t RegionType;
    uint32_t ExtendedAttributes;
};

void PrintMemoryMap(MemoryMapEntry* memoryMap, uint16_t position);

extern MemoryMapEntry* UsableMemoryRegion[10];
extern uint8_t UsableMemeoryRegionCount;

MemoryMapEntry** getUsableMemoryRegions();

