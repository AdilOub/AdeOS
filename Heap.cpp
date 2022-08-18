#include "Header Files/Heap.h"

MemorySegmentHeader* FirstFreeMemorySegment;

void InitializeHeap(uint_64 heapAdress, uint_64 heapLength){
    FirstFreeMemorySegment = (MemorySegmentHeader*)heapAdress;
    FirstFreeMemorySegment->MemoryLength = heapLength - sizeof(MemorySegmentHeader);
    FirstFreeMemorySegment->nextSegment = 0;
    FirstFreeMemorySegment->previousSegment = 0;
    FirstFreeMemorySegment->nextFreeSegment = 0;
    FirstFreeMemorySegment->previousFreeSegment = 0;
}

void* malloc(uint_64 size){
    uint_64 remainder = size % 8;
    size -= remainder;
    if(remainder != 0) size += 8;

    MemorySegmentHeader* currentMemSeg = FirstFreeMemorySegment;
    while(true){
        if(currentMemSeg->MemoryLength >= size){
            if(currentMemSeg->MemoryLength > (size + sizeof(MemorySegmentHeader))){
                MemorySegmentHeader* newSegmentHeader = (MemorySegmentHeader*)((uint_64)currentMemSeg + sizeof(MemorySegmentHeader) + size);
                newSegmentHeader->Free = true;
                newSegmentHeader->MemoryLength = ((uint_64)currentMemSeg->MemoryLength) - (sizeof(MemorySegmentHeader)+size);
                newSegmentHeader->nextFreeSegment = currentMemSeg->nextFreeSegment;
                newSegmentHeader->nextSegment = currentMemSeg->nextSegment;
                newSegmentHeader->previousSegment = currentMemSeg;
                newSegmentHeader->previousFreeSegment = currentMemSeg->previousFreeSegment;

                currentMemSeg->nextFreeSegment = newSegmentHeader;
                currentMemSeg->nextSegment = newSegmentHeader;

                currentMemSeg->MemoryLength = size;
            }
            if(currentMemSeg == FirstFreeMemorySegment){
                FirstFreeMemorySegment = currentMemSeg->nextFreeSegment;
            }
            currentMemSeg->Free = false;

            if(currentMemSeg->previousFreeSegment != 0) currentMemSeg->previousFreeSegment->nextFreeSegment = currentMemSeg->nextFreeSegment;
            if(currentMemSeg->nextFreeSegment != 0) currentMemSeg->nextFreeSegment->previousFreeSegment = currentMemSeg->previousFreeSegment;

            if(currentMemSeg->previousSegment != 0) currentMemSeg->previousSegment->nextFreeSegment = currentMemSeg->nextFreeSegment;
            if(currentMemSeg->nextSegment != 0) currentMemSeg->nextSegment->previousFreeSegment = currentMemSeg->previousFreeSegment;


            return currentMemSeg + 1;
        }
        if(currentMemSeg->nextFreeSegment == 0){
            //no more memory
            return 0;
        }
        currentMemSeg = currentMemSeg->nextFreeSegment;
    }
    return 0;
}
