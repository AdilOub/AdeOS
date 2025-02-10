#pragma once


#include "libc/stdtypes.h"
#include "cpu/io.h"
#include "TextPrint.h"

#define PIT_COMMAND_PORT    0x43
#define PIT_CHANNEL0_PORT   0x40
#define PIT_CHANNEL2_PORT   0x42
#define PIT_MODE            0x36  // Mode 3 (square wave generator) pour le canal 0 du PIT

#define PIT_FREQUENCY       1193182  // Fréquence du PIT en Hz (typiquement 1.193182 MHz)


void initTimer();
