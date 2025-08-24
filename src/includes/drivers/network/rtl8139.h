#pragma once

#include "../../TextPrint.h"
#include "../../cpu/idt.h"
#include "../../libc/stdtypes.h"
#include "../pci.h"


char* rtl8139_setup();
void printMACAddress();