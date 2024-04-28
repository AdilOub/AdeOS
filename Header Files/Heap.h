#pragma once

#include "Typedefs.h"
#include "TextPrint.h"
#include "Memory.h"

#define NULL 0 

struct MemorySegmentHeader{
    uint_64 MemoryLength;
    MemorySegmentHeader* nextSegment;
    MemorySegmentHeader* previousSegment;
    MemorySegmentHeader* nextFreeSegment;
    MemorySegmentHeader* previousFreeSegment;
    bool Free;
};

struct AlignedMemorySegmentHeader{
    uint_64 MemorySegmentHeaderAdress : 63;
    bool IsAligned : 1;
};


void* malloc(uint_64 size);
void InitializeHeap(uint_64 heapAdress, uint_64 heapLength);

void* aligned_alloc(uint_64 alignment, uint_64 size);

void free(void* address);
void* calloc(uint_64 size);
void* realloc(void* adress, uint_64 newSize);