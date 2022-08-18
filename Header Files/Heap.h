#pragma once

#include "Typedefs.h"
#include "TextPrint.h"

struct MemorySegmentHeader{
    uint_64 MemoryLength;
    MemorySegmentHeader* nextSegment;
    MemorySegmentHeader* previousSegment;
    MemorySegmentHeader* nextFreeSegment;
    MemorySegmentHeader* previousFreeSegment;
    bool Free;
};


void* malloc(uint_64 size);
void InitializeHeap(uint_64 heapAdress, uint_64 heapLength);

void free(void* address);