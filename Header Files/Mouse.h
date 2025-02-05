#pragma once
#include "Kernel.h"
#include "Commands.h"
#include "TextPrint.h"
#include "Keyboard.h"
#include "IDT.h"
#include "Heap.h"
#include "Memory.h"
#include "IO.h"

#define COMMAND_PORT 0x64
#define DATA_PORT 0x60



uint8_t readControllerConfig();


void initPS2Mouse();