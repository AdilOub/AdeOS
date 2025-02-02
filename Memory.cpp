#include "Header Files/Memory.h"

MemoryMapEntry* UsableMemoryRegion[10];
uint8_t UsableMemeoryRegionCount = 0;


//pourquoi :C
/*
void* memset(void* start, uint64_t value, uint64_t num){ 
	if(num<=8){
		uint8_t* valPtr = (uint8_t*)&value;
		for(uint8_t* ptr = (uint8_t*)start; ptr < (uint8_t*)((uint64_t)start + num); ptr++){
			*ptr= *valPtr;
		}
		return start;

	}

	uint64_t proceedingByte = num % 8;
	uint64_t newnum = num - proceedingByte;
	for(uint64_t* ptr = (uint64_t*)start; ptr < (uint64_t*)((uint64_t)start + newnum); ptr++){
		*ptr = value;
	}

	uint8_t* valPtr = (uint8_t*)&value;
	for(uint8_t* ptr = (uint8_t*)start + newnum; ptr < (uint8_t*)((uint64_t)start + num); ptr++){
		*ptr= *valPtr;
	}
	return start;
}
*/


/*

TODO OPTIMISER AVEC uint64_t et alignement

*/
//moins rapide mais au moins c'est fiable...
void* memset(void* start, uint8_t val, uint64_t size){
	uint8_t* ptr = (uint8_t*)start;
	for(uint64_t i = 0; i < size; i++){
		*ptr = val;
		ptr++;
	}
	return start;
}

//fix de memcopy

void memcopy(void* destination, void* source, uint64_t num){
	uint8_t* valPtr = (uint8_t*)source;
	uint8_t* ptr = (uint8_t*)destination;
	for(uint64_t i = 0; i < num; i++){
		*ptr = *valPtr;
		ptr++;
		valPtr++;
	}
}

/* 
//mdr c'est ça de recopier du code en 2022(???) omg ça date, mais merci quand même poncho <3
void memcopy(void* destination, void* source, uint64_t num){
	if(num<=8){
		uint8_t* valPtr = (uint8_t*)source;
		for(uint8_t* ptr = (uint8_t*)destination; ptr < (uint8_t*)((uint64_t)destination + num); ptr++){
			*ptr= *valPtr;
			valPtr++;
		}
		return;
	}
	
	uint64_t proceedingByte = num % 8;
	uint64_t newnum = num - proceedingByte;
	uint64_t* srcptr = (uint64_t*)source;

	for(uint64_t* ptr = (uint64_t*)destination; ptr < (uint64_t*)((uint64_t)destination + newnum); ptr++){
		*ptr = *srcptr;
		srcptr++;
	}

	uint8_t* valPtr = (uint8_t*)((uint64_t)source + newnum);
	for(uint8_t* ptr =  (uint8_t*)((uint64_t)source + newnum); ptr <  (uint8_t*)((uint64_t)source + num); ptr++){
		*ptr= *valPtr;
		valPtr++;
	}
	return;
}*/

void PrintMemoryMap(MemoryMapEntry* memoryMap, uint16_t position){
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
	
	uint8_t UsableMemIndex = 0;
	for(uint8_t i = 0; i < MemoryRegionCount; i++){
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