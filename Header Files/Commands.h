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



struct cmd_item
{
    const char* key;
    void (*value)(char*);
};
typedef struct cmd_item cmd_item;

struct cmd_table
{
    cmd_item* items;
    int size;
    int count;
};
typedef struct cmd_table cmd_table;


cmd_table* create_table();
void set(cmd_table* t, const char* key, void (*value)(char*));
void (*get(cmd_table* t, const char* key))(char*);

void initCmds();