#pragma once
#include "Kernel.h"
#include "Commands.h"
#include "TextPrint.h"
#include "Keyboard.h"
#include "KBScanCodeSet1.h"


void clearCmdBuffer(char* buffer, int bufferSize);
bool strcmp(char* a, char* b);
void endCmd();
void errorCmd(char* cmd);
void handleCmds(char* cmd);
