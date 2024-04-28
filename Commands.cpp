#include "Header Files/Commands.h"
#include "Header Files/Heap.h"
/*
const char* colorString[] = {"blue", "green", "red", "purple", "white", "black"};
const int colorForeground[] = {};
const int colorBackground[] = {};
*/


/*
-------------------------------------
Creation de la hashtable des commandes
-------------------------------------
*/



#pragma region cmds
void helpCmd(char* args){
    PrintString(" Cmds: help; clear; arrow; lang-fr; lang-en; slt; fatal; reboot");
    endCmd();
}

void clearCmd(char* args){
    ClearScreen();
    SetCursorPosition(PosFromCoord(0,0));
    endCmd();
}

void arrowCmd(char* args){
    if(getArrowState()){
        enableArrow(false);
        PrintString(" Arrow disabled");
    }else{
        enableArrow(true);
        PrintString(" Arrow enabled");
    }
    endCmd();
}

void langFrCmd(char* args){
    setLanguage(KBSet1::ScanCodeLookupTableAZERTY);
    PrintString(" Keyboard set to azerty");
    endCmd();
}

void langEnCmd(char* args){
    setLanguage(KBSet1::ScanCodeLookupTableQWERTY);
    PrintString(" Keyboard set to qwerty");
    endCmd();
}

void sltCmd(char* args){
    ClearScreen(BACKGROUND_BLACK);
    SetCursorPosition(0);
    PrintString(Test);
    endCmd();
}

void fatalCmd(char* args){
    PrintString("Fatal error: ", FOREGROUND_RED);
    endCmd();
    int a = 1/0;
}

void rebootCmd(char* args){
    asm("jmp 0xFFFF");
}

#pragma endregion cmds


//une permutation de [0;255]
#define SIZE 256
unsigned int pearson_table[SIZE] = {131,73,96,115,237,223,74,236,41,166,186,192,242,34,56,122,173,164,84,138,152,167,231,43,8,184,63,136,195,159,146,144,102,226,10,52,38,40,163,25,88,199,83,36,37,233,94,212,11,178,64,85,215,180,240,194,210,130,134,82,109,214,72,135,112,68,220,20,45,225,252,181,53,104,126,157,234,213,98,200,127,227,92,255,91,78,65,13,9,175,31,7,238,118,156,217,105,3,254,29,162,250,170,116,100,93,209,230,222,39,16,185,132,216,190,125,137,249,95,26,168,169,70,123,177,114,241,196,108,239,15,103,59,67,193,229,202,171,203,208,66,0,87,189,154,183,201,120,6,149,142,197,139,46,50,80,219,151,57,161,86,117,22,49,30,153,205,77,32,5,228,12,211,124,89,243,1,33,14,48,140,76,232,24,119,148,188,90,113,75,207,245,58,69,97,111,179,145,19,2,28,55,110,221,23,147,107,21,27,44,4,129,158,62,61,155,251,54,206,187,101,248,42,128,99,71,244,141,246,17,176,79,235,133,204,121,174,81,35,224,191,51,143,160,60,18,172,165,150,106,182,198,253,247,218,47};
cmd_table* cmdTable;

const char* cmds[] = {"help", "clear", "arrow", "lang-fr", "lang-en", "slt", "fatal", "reboot"};

unsigned int pearson(const char* str){
    unsigned int hash = 0;
    for (int i = 0; str[i] != '\0'; i++)
    {
        hash = pearson_table[hash ^ str[i]];
    }
    return hash;
}


unsigned int hashfunction(char *key){
    int hash = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        hash += key[i];
    }
    return hash % SIZE; //le modulo est inutile
}


unsigned int djb2(char* str){
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)){
        hash = ((hash << 5) + hash) + c; // 33 parceque ca marche bien 
    }
    return hash;
}


cmd_table* create_table(){
    cmd_table* t = (cmd_table*)malloc(sizeof(cmd_table));
    t->items = (cmd_item*)malloc(sizeof(cmd_item) * SIZE);
    for(int i = 0; i<SIZE; i++){
        t->items[i].key = NULL;
        t->items[i].value = NULL;
    }
    t->size = SIZE;
    t->count = 0;
    return t;
}

void set(cmd_table* t, const char* key, void (*value)(char*)){
    int index = pearson(key);
    if(t->items[index].key == NULL){
        t->items[index].key = key;
        t->items[index].value = value;
        t->count++;
    }
}

void (*get(cmd_table* t, const char* key))(char*){
    int index = pearson(key);

    if(t->items[index].key == NULL){
        //printf("No value found for key: %s\n", key);
        //PrintString("Pas de valeur pour la clé: ", FOREGROUND_RED);
        //PrintString(key, FOREGROUND_RED);
        return NULL;
    } else {
        return t->items[index].value;
    }
}

void clearCmdBuffer(char* buffer, int bufferSize){
    for(int i = 0; i<bufferSize; i++){
        buffer[i] = '\0';
    }
}


void endCmd(){
    PrintString("\n\r>");
}

void errorCmd(char* cmd){
    PrintString("--Unknown cmd: ", FOREGROUND_RED);
    PrintString(cmd, FOREGROUND_RED);
    endCmd();
}

bool strcmp(char* a, const char* b){
    int n = 0;
    while(a[n] != '\0'){
        if(a[n] != b[n]){
            return false;
        } 
        n++;
    }

    return a[n] == b[n];
}



void initCmds(){
    PrintString("Loading cmds...\n\r", FOREGROUND_GREEN);
    cmdTable = create_table();
    //peut etre utiliser un fichier texte pour faire ça automatiquement
    set(cmdTable, cmds[0], &helpCmd);
    set(cmdTable, cmds[1], &clearCmd);
    set(cmdTable, cmds[2], &arrowCmd);
    set(cmdTable, cmds[3], &langFrCmd);
    set(cmdTable, cmds[4], &langEnCmd);
    set(cmdTable, cmds[5], &sltCmd);
    set(cmdTable, cmds[6], &fatalCmd);
    set(cmdTable, cmds[7], &rebootCmd);
    PrintString("Cmds loaded!\n\r", FOREGROUND_GREEN);
}




void handleCmds(char* cmd){
    //todo separer la cmd et les arguments
    //PrintString("getting cmd...\n\r", FOREGROUND_GREEN);
    void (*cmdFunc)(char*) = get(cmdTable, cmd);
    //PrintString("ok!\n\r", FOREGROUND_GREEN);
    if(cmdFunc == NULL){
        errorCmd(cmd);
        return;
    }
    (*cmdFunc)(NULL);
    endCmd();
    return;
}

/*
//j'avais pas encore connaisance des strucures de donnée
//todo 1: utiliser une hashmap
//todo 2: utiliser un systeme de fichier ou les commandes sont des exectuables dans un dossier (comme pour lionux)
void handleCmds(char* cmd){
    if(strcmp(cmd, "help")){
        //PrintChar('n');
        PrintString(" Cmds: help; clear; arrow; lang-fr; lang-en");
        endCmd();
    }
    else if(strcmp(cmd, "clear")){
        ClearScreen();
        SetCursorPosition(PosFromCoord(0,0));
        endCmd();
    }else if(strcmp(cmd, "arrow")){
        if(getArrowState()){
            enableArrow(false);
            PrintString(" Arrow disabled");
        }else{
            enableArrow(true);
            PrintString(" Arrow enabled");
        }
        endCmd();
    }else if(strcmp(cmd, "lang-fr")){
        setLanguage(KBSet1::ScanCodeLookupTableAZERTY);
        PrintString(" Keyboard set to azerty");
        endCmd();
    }else if(strcmp(cmd, "lang-en")){
        setLanguage(KBSet1::ScanCodeLookupTableQWERTY);
        PrintString(" Keyboard set to qwerty");
        endCmd();
    }else if(strcmp(cmd, "slt")){
        ClearScreen(BACKGROUND_BLACK);
        SetCursorPosition(0);
        PrintString(Test);
        endCmd();
    }else if(strcmp(cmd, "fatal")){
        PrintString("Fatal error: ", FOREGROUND_RED);
        endCmd();
        int a = 1/0;
    }else if(strcmp(cmd, "reboot")){
        asm("jmp 0xFFFF");
    }
    else{
        errorCmd(cmd);
    }
}
*/

