#include "includes/Heap.h"

MemorySegmentHeader* FirstFreeMemorySegment;

void InitializeHeap(uint64_t heapAdress, uint64_t heapLength){
    FirstFreeMemorySegment = (MemorySegmentHeader*)heapAdress;
    FirstFreeMemorySegment->MemoryLength = heapLength - sizeof(MemorySegmentHeader);
    FirstFreeMemorySegment->nextSegment = 0;
    FirstFreeMemorySegment->previousSegment = 0;
    FirstFreeMemorySegment->nextFreeSegment = 0;
    FirstFreeMemorySegment->previousFreeSegment = 0;
}

void* malloc(uint64_t size){
    uint64_t remainder = size % 8;
    size -= remainder;
    if(remainder != 0) size += 8;

    MemorySegmentHeader* currentMemSeg = FirstFreeMemorySegment;
    while(true){
        if(currentMemSeg->MemoryLength >= size){
            if(currentMemSeg->MemoryLength > (size + sizeof(MemorySegmentHeader))){
                MemorySegmentHeader* newSegmentHeader = (MemorySegmentHeader*)((uint64_t)currentMemSeg + sizeof(MemorySegmentHeader) + size);
                newSegmentHeader->Free = true;
                newSegmentHeader->MemoryLength = ((uint64_t)currentMemSeg->MemoryLength) - (sizeof(MemorySegmentHeader)+size);
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

void CombineFreeSegments(MemorySegmentHeader* a, MemorySegmentHeader* b){
    if(a==0) return;
    if(b==0) return;
    if(a<b){
        a->MemoryLength += b->MemoryLength + sizeof(MemorySegmentHeader);
        a->nextSegment = b->nextSegment;
        a->nextFreeSegment = b->nextFreeSegment;
        b->nextSegment->previousSegment = a;
        b->nextSegment->previousFreeSegment = a;
        b->nextFreeSegment->previousFreeSegment = a;
    }else{
        b->MemoryLength += a->MemoryLength + sizeof(MemorySegmentHeader);
        b->nextSegment = a->nextSegment;
        b->nextFreeSegment = a->nextFreeSegment;
        a->nextSegment->previousSegment = b;
        a->nextSegment->previousFreeSegment = b;
        a->nextFreeSegment->previousFreeSegment = b;
    }
}

void free(void* address){
    MemorySegmentHeader* currentMemSeg;
    
    AlignedMemorySegmentHeader* AMSH = (AlignedMemorySegmentHeader*)address - 1;
    if(AMSH->IsAligned){
        currentMemSeg = (MemorySegmentHeader*)(uint64_t)AMSH->MemorySegmentHeaderAdress;
    }else{
        currentMemSeg = ((MemorySegmentHeader*)address) - 1;
    }
    currentMemSeg->Free = true;

    if(currentMemSeg<FirstFreeMemorySegment) FirstFreeMemorySegment = currentMemSeg;
    
    if(currentMemSeg->nextFreeSegment != 0){
        if(currentMemSeg->nextFreeSegment->previousFreeSegment < currentMemSeg){
            currentMemSeg->nextFreeSegment->previousFreeSegment = currentMemSeg;
        }
    }

    if(currentMemSeg->previousFreeSegment != 0){
        if(currentMemSeg->previousFreeSegment->nextFreeSegment > currentMemSeg){
            currentMemSeg->previousFreeSegment->nextFreeSegment = currentMemSeg;
        }
    }

    if(currentMemSeg->nextSegment != 0){
        currentMemSeg->nextSegment->previousSegment = currentMemSeg;
        if(currentMemSeg->nextSegment->Free) CombineFreeSegments(currentMemSeg, currentMemSeg->nextSegment);
    }

    if(currentMemSeg->previousSegment !=0){
        currentMemSeg->previousSegment->nextSegment = currentMemSeg;
        if(currentMemSeg->previousSegment->Free) CombineFreeSegments(currentMemSeg, currentMemSeg->previousSegment);

    }

}


void* calloc(uint64_t size){
    void* mallocVal = malloc(size);
    memset(mallocVal, 0, size);
    return mallocVal;
}

void* realloc(void* address, uint64_t newSize){
    MemorySegmentHeader* oldSegment;
    
    AlignedMemorySegmentHeader* AMSH = (AlignedMemorySegmentHeader*)address - 1;
    if(AMSH->IsAligned){
        oldSegment = (MemorySegmentHeader*)(uint64_t)AMSH->MemorySegmentHeaderAdress;
    }else{
        oldSegment = ((MemorySegmentHeader*)address) - 1;
    }
    uint64_t smallerSize = newSize;
    if(oldSegment->MemoryLength < newSize) smallerSize = oldSegment->MemoryLength;
    void* newMem = malloc(newSize);
    memcopy(newMem, address, smallerSize);
    free(address);
    return newMem;
}

void* aligned_alloc(uint64_t alignment, uint64_t size){
    uint64_t alignmentReminder = alignment%8;
    alignment -= alignmentReminder;
    if(alignmentReminder != 0) alignment += 8;

    uint64_t sizeR = size%8;
    size -= sizeR;
    if(sizeR != 0) size += 8;

    uint64_t fullSize  = size + alignment;
    void* mallocVal = malloc(fullSize);

    uint64_t address = (uint64_t)mallocVal;

    uint64_t reminder = address%alignment;
    address -= reminder;

    if(reminder!=0){
        address += alignment;

        AlignedMemorySegmentHeader* AMSH = (AlignedMemorySegmentHeader*)address - 1;
        AMSH->IsAligned=true;
        AMSH->MemorySegmentHeaderAdress = (uint64_t)mallocVal - sizeof(MemorySegmentHeader);
    }

    return (void*)address;
}