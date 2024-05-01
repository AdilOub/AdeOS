#include "Header Files/Commands.h"
#include "Header Files/Heap.h"
#include "Header Files/Timer.h"
#include "Header Files/Render.h"
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


/*
#pragma region cmds
void helpCmd(char* args){
    PrintString(" Cmds: help; clear; arrow; lang-fr; lang-en; slt; fatal; reboot; cube");
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
    PrintString(" Generating fatal error...", FOREGROUND_RED);
    endCmd();
    int a = 1/0;
}
*/

/*
static const char * drive_types[8] = {
    "none",
    "360kB 5.25\"",
    "1.2MB 5.25\"",
    "720kB 3.5\"",

    "1.44MB 3.5\"",
    "2.88MB 3.5\"",
    "unknown type",
    "unknown type"
};

void readDiskCmd(char *args){

    outb(0x70, 0x10);
    unsigned drives = inb(0x71);
    PrintString("Floppy drives 0: "); PrintString(drive_types[drives >> 4]); PrintString("\n\r");
    PrintString("Floppy drives 1: "); PrintString(drive_types[drives & 0x0F]); PrintString("\n\r");
    
    PrintString("\n\r");

    uint_8 floppy_info = inb(FLPYDSK_MSR);
    PrintString("Floppy disk status: ");
    PrintString(IntToString(floppy_info));
    PrintString("\n\r");

    uint_8 *ptr = (uint_8*)(0x1000 + 0x8000); 
    PrintString("Preread:");
    PrintString("\n\r");
    PrintString("0x");
    PrintString(HexToString(*ptr));
    PrintString(" 0x");
    PrintString(HexToString(*(ptr+1)));

    flpydsk_read_sector(0);
    PrintString("Postread:");
    PrintString("\n\r");
    PrintString("0x");
    PrintString(HexToString(*ptr));
    PrintString(" 0x");
    PrintString(HexToString(*(ptr+1)));


    floppy_info = inb(FLPYDSK_MSR);
    PrintString("Floppy disk status: ");
    PrintString(IntToString(floppy_info));
    PrintString("\n\r");

    *ptr = 0x41;
    *(ptr+1) = 0x42;

}*/

//geré
void raise_interupt32(char* args){
    asm("int $32");
}

//pas geré, va faire planter le systeme
void raise_interupt49(char* args){
    asm("int $49");
}
void timer(char* args){
    initTimer();
}
void rebootCmd(char* args){
    asm("jmp 0xFFFF");
}

/*
void cube(char* args){
    //pour render un cube on doit garder en mémoire les sommets et les arrêtes, et projeté 
    //les sommets sur l'écran en fonction de la position de la caméra
    //on utilisera ensuite une matrice de rotation et le timer (qu'on vient d'imlementer) pour faire tourner le cube

    //sommets et arretes
    //coordonnées des sommets
    //on est en 80*25, on va prendre un cube de 20*20*20 centré en 40, 12, tourné de 45°
    
    int edges[8][3] = {
        {35, 7, 7},
        {35, 17, 7},
        {45, 7, 7},
        {45, 17, 7},
        {35, 7, 17},
        {35, 17, 17},
        {45, 7, 17},
        {45, 17, 17}
    };
    


    //pas la manière la plus effiace, mais c'est un exemple;
    //ce code sera amélioré quand on changera de mode VGA (si on le fait)
    //bool si x et y sont reliés
    bool vertices[8][8] = {
        {0, 1, 1, 0, 1, 0, 0, 0},
        {1, 0, 0, 1, 0, 1, 0, 0},
        {1, 0, 0, 1, 0, 0, 1, 0},
        {0, 1, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 1, 1, 0},
        {0, 1, 0, 0, 1, 0, 0, 1},
        {0, 0, 1, 0, 1, 0, 0, 1},
        {0, 0, 0, 1, 0, 1, 1, 0}
    };

    //on projete les sommets sur l'écran
    //on prendra un point de vue de 0, 0, 0
    //un ecran de 80x25
    //on prendra un angle de pi/4 et une distance de 1
    
    //quel est la distance optimal pour que le cube soit bien visible?
    float dist = 40;
    int screen[8][2];

    //on calcul les coordonnées projetées (donc en 2d)
    for(int i = 0; i < 8; i++){
        screen[i][0] = (int)(edges[i][0] * dist) / (edges[i][2] + dist);
        screen[i][1] = (int)(edges[i][1] * dist) / (edges[i][2] + dist);
    }

    //on dessine les arrêtes
    for(int i = 0; i<8; i++){
        for(int j=0; j<8; j++){
            if(vertices[i][j]){
                //on dessine l'arrête directement dans la mémoire VGA, l'écran fait 80x25
                drawline(screen[i], screen[j]);
            }
        }
    }

    //on dessine un point rouge au projeté de chaque sommet
    for(int i = 0; i<8; i++){
        int x = (int)(edges[i][0] * dist) / (edges[i][2] + dist);
        int y = (int)(edges[i][1] * dist) / (edges[i][2] + dist);
        int index = x + y*80;
        *(VGA_MEMORY + index*2) = 0x0F; //'*'
        *(VGA_MEMORY + index*2 +1) = FOREGROUND_RED;
    }
    

}*/
#pragma endregion cmds



//TODO: trouver une permutation pour avoir une fonction de hachage plus efficace voir changer de fonction de hachage

//une permutation de [0;255]
#define CMDS_TABLE_SIZE 256
unsigned int pearson_table[CMDS_TABLE_SIZE] = {131,73,96,115,237,223,74,236,41,166,186,192,242,34,56,122,173,164,84,138,152,167,231,43,8,184,63,136,195,159,146,144,102,226,10,52,38,40,163,25,88,199,83,36,37,233,94,212,11,178,64,85,215,180,240,194,210,130,134,82,109,214,72,135,112,68,220,20,45,225,252,181,53,104,126,157,234,213,98,200,127,227,92,255,91,78,65,13,9,175,31,7,238,118,156,217,105,3,254,29,162,250,170,116,100,93,209,230,222,39,16,185,132,216,190,125,137,249,95,26,168,169,70,123,177,114,241,196,108,239,15,103,59,67,193,229,202,171,203,208,66,0,87,189,154,183,201,120,6,149,142,197,139,46,50,80,219,151,57,161,86,117,22,49,30,153,205,77,32,5,228,12,211,124,89,243,1,33,14,48,140,76,232,24,119,148,188,90,113,75,207,245,58,69,97,111,179,145,19,2,28,55,110,221,23,147,107,21,27,44,4,129,158,62,61,155,251,54,206,187,101,248,42,128,99,71,244,141,246,17,176,79,235,133,204,121,174,81,35,224,191,51,143,160,60,18,172,165,150,106,182,198,253,247,218,47};
cmd_table* cmdTable;

//collision entre int49 et timer


//const void cmdFuncs[]  = {&helpCmd, &clearCmd, &arrowCmd, &langFrCmd, &langEnCmd, &sltCmd, &fatalCmd, &rebootCmd, &interupt49};

/*
unsigned int pearson(const char* str){
    PrintString("Hashing: ", FOREGROUND_LIGHTGRAY);
    PrintString(str, FOREGROUND_LIGHTGRAY);
    PrintString("\n\r", FOREGROUND_LIGHTGRAY);
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
    return hash % CMDS_TABLE_SIZE; //le modulo est inutile
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
    t->items = (cmd_item*)malloc(sizeof(cmd_item) * CMDS_TABLE_SIZE);
    for(int i = 0; i<CMDS_TABLE_SIZE; i++){
        t->items[i].key = NULL;
        t->items[i].value = NULL;
    }
    t->size = CMDS_TABLE_SIZE;
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



void endCmd(){
    PrintString("\n\r>");
}

void errorCmd(char* cmd){
    PrintString("--Unknown cmd: ", FOREGROUND_RED);
    PrintString(cmd, FOREGROUND_RED);
    endCmd();
}
*/

void clearCmdBuffer(char* buffer, int bufferSize){
    for(int i = 0; i<bufferSize; i++){
        buffer[i] = '\0';
    }
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


//const char* cmds[] = {"help", "clear", "arrow", "langfr", "langen", "slt", "fatal", "reboot", "int32", "int49", "ttimer", "cube", "read"};
/*
void initCmds(){
    PrintString("Loading cmds...\n\r", FOREGROUND_GREEN);
    PrintString(IntToString(pearson_table[0]));

    const char* cmds[] = {"help", "clear", "arrow", "langfr", "langen", "slt", "fatal", "reboot", "int32", "int49", "ttimer", "cube", "read"};

    cmdTable = create_table();
    //peut etre utiliser un fichier texte pour faire ça automatiquement
    set(cmdTable, cmds[0], &helpCmd);
    set(cmdTable, cmds[1], &clearCmd);

    //on va afficher les addresses des char* 
    PrintString("Address of help: ");
    PrintString(HexToString((uint_64)cmds[0]));
    PrintString("\n\r");

    PrintString("Address of clear: ");
    PrintString(HexToString((uint_64)cmds[1]));
    PrintString("\n\r");

    int index = pearson(cmds[0]);
    PrintString("Index of help: ");
    PrintString(HexToString(index));
    PrintString("\n\r");

    index = pearson(cmds[1]);
    PrintString("Index of clear: ");
    PrintString(HexToString(index));
    PrintString("\n\r");

    set(cmdTable, "clear", &clearCmd);
    set(cmdTable, "arrow", &arrowCmd);
    set(cmdTable, "langfr", &langFrCmd);
    set(cmdTable, "langen", &langEnCmd);
    set(cmdTable, "slt", &sltCmd);
    set(cmdTable, "fatal", &fatalCmd);
    set(cmdTable, "reboot", &rebootCmd);
    set(cmdTable, "int32", &raise_interupt32);
    set(cmdTable, "int49", &raise_interupt49);
    set(cmdTable, "ttimer", &timer);
    set(cmdTable, "cube", &cube);
    set(cmdTable, "read", &readDiskCmd);
    PrintString("Cmds loaded!\n\r", FOREGROUND_GREEN);
}

*/


void handleCmds(char* cmd){
    //todo separer la cmd et les arguments
    //PrintString("getting cmd...\n\r", FOREGROUND_GREEN);
    //void (*cmdFunc)(char*) = get(cmdTable, cmd);
    //PrintString("ok!\n\r", FOREGROUND_GREEN);
    /*if(cmdFunc == NULL){
        errorCmd(cmd);
        return;
    }
    (*cmdFunc)(NULL);
    endCmd();*/
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

