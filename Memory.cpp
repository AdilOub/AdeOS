#include "Header Files/Memory.h"

MemoryMapEntry* UsableMemoryRegion[10];
uint_8 UsableMemeoryRegionCount = 0;


void* memset(void* start, uint_64 value, uint_64 num){ 
	if(num<=8){
		uint_8* valPtr = (uint_8*)&value;
		for(uint_8* ptr = (uint_8*)start; ptr < (uint_8*)((uint_64)start + num); ptr++){
			*ptr= *valPtr;
			valPtr++;
		}
		return start;

	}

	uint_64 proceedingByte = num % 8;
	uint_64 newnum = num - proceedingByte;
	for(uint_64* ptr = (uint_64*)start; ptr < (uint_64*)((uint_64)start + newnum); ptr++){
		*ptr = value;
	}

	uint_8* valPtr = (uint_8*)&value;
	for(uint_8* ptr = (uint_8*)start; ptr < (uint_8*)((uint_64)start + num); ptr++){
		*ptr= *valPtr;
		valPtr++;
	}
	return start;
}

void memcopy(void* destination, void* source, uint_64 num){
	if(num<=8){
		uint_8* valPtr = (uint_8*)source;
		for(uint_8* ptr = (uint_8*)destination; ptr < (uint_8*)((uint_64)destination + num); ptr++){
			*ptr= *valPtr;
			valPtr++;
		}
		return;
	}
	
	uint_64 proceedingByte = num % 8;
	uint_64 newnum = num - proceedingByte;
	uint_64* srcptr = (uint_64*)source;

	for(uint_64* ptr = (uint_64*)destination; ptr < (uint_64*)((uint_64)destination + newnum); ptr++){
		*ptr = *srcptr;
		srcptr++;
	}

	uint_8* valPtr = (uint_8*)((uint_64)source + newnum);
	for(uint_8* ptr =  (uint_8*)((uint_64)source + newnum); ptr <  (uint_8*)((uint_64)source + num); ptr++){
		*ptr= *valPtr;
		valPtr++;
	}
	return;
}

void PrintMemoryMap(MemoryMapEntry* memoryMap, uint_16 position){
	SetCursorPosition(position);

	PrintString("Memory Base: "); PrintString(IntToString(memoryMap->BaseAdress));
	SetCursorPosition(position + VGA_WIDTH);
	PrintString("Region Lenght: "); PrintString(IntToString(memoryMap->RegionLength));
	SetCursorPosition(position + (VGA_WIDTH*2));	
	PrintString("Region Type: "); PrintString(IntToString(memoryMap->RegionType));
	SetCursorPosition(position + (VGA_WIDTH*3));	
	PrintString("Extended Attributes: "); PrintString(IntToString(memoryMap->ExtendedAttributes));
	SetCursorPosition(position + (VGA_WIDTH*5));	
}

bool MemoryRegionsGot = false;

MemoryMapEntry** getUsableMemoryRegions(){
	if(MemoryRegionsGot){
		return UsableMemoryRegion;
	}
	
	MemoryRegionsGot = true;
	
	uint_8 UsableMemIndex = 0;
	for(uint_8 i = 0; i < MemoryRegionCount; i++){
        MemoryMapEntry* memMap = (MemoryMapEntry*)MEMORY_COUNT_LOCATION;
        memMap += i;
		if(memMap->RegionType == 1){
			UsableMemoryRegion[UsableMemIndex]=memMap;
			UsableMemIndex++;
 		}
    }
	UsableMemeoryRegionCount = UsableMemIndex;

	return UsableMemoryRegion;
}