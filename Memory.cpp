#include "Header Files/Memory.h"

MemoryMapEntry* UsableMemoryRegion[10];
uint_8 UsableMemeoryRegionCount = 0;


void* memset(void* dest, unsigned char val, int count){ 
	unsigned char* destC = (unsigned char*)dest;
	int i;
	for (i = 0; i < count; i++)
		destC[i] = val;
	return dest;
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