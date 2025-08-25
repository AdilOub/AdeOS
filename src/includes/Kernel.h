#pragma once
#include "libc/stdtypes.h"

//#define VGA13


//Dans le futur, peut être utiliser des fonctions plutot que des variables pour eviter les erreurs de ma part (il ne faut pas écrire dans ces variables !!!)


//va contenir pour chaque scancode (pressed) 0 si relaché, 1 si pressé
//la 256ème case contient 1 si any key pressed (sauf échap)
extern uint8_t keyboard_input[256];


extern const char Test[];
extern const uint8_t Emi1[];
