#pragma once

#include "libc/stdtypes.h"
#include "TextPrint.h"
#include "Memory.h"

#define NULL 0 

struct MemorySegmentHeader{
    uint64_t MemoryLength;
    MemorySegmentHeader* nextSegment;
    MemorySegmentHeader* previousSegment;
    MemorySegmentHeader* nextFreeSegment;
    MemorySegmentHeader* previousFreeSegment;
    bool Free;
};

struct AlignedMemorySegmentHeader{
    uint64_t MemorySegmentHeaderAdress : 63;
    bool IsAligned : 1;
};


void* malloc(uint64_t size);
void InitializeHeap(uint64_t heapAdress, uint64_t heapLength);

void* aligned_alloc(uint64_t alignment, uint64_t size);

void free(void* address);
void* calloc(uint64_t size);
void* realloc(void* adress, uint64_t newSize);