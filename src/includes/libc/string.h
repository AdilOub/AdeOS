#pragma once

#include "stdtypes.h"
#include "../Heap.h"


//alloue une nouvelle chaine de caractère et copie src après dest
const char* strcat(char* dest, const char* src) ;
//realloc (ou alloue) dest pour y ajouter src après dest
void strcatover(char** dest, const char* src) ;

//ecrit src dans dest (overwritting). dest doit être assez grand !
void strcpy(char* dest, const char* src) ;

uint64_t strlen(const char* str) ;