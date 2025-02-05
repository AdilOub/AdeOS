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



#pragma region cmds
void helpCmd(char* args){
    PrintString(" Cmds: help; clear; arrow; lang-fr; lang-en; slt; fatal; reboot; cube");
    endCmd();
}

void clearCmd(){
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

    uint8_t floppy_info = inb(FLPYDSK_MSR);
    PrintString("Floppy disk status: ");
    PrintString(IntToString(floppy_info));
    PrintString("\n\r");

    uint8_t *ptr = (uint8_t*)(0x1000 + 0x8000); 
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
    asm("int $32"); //normalement c'est un div0
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

    int offX = 0;
    int offY = 0;
    

    //on dessine les arrêtes
    for(int i = 0; i<8; i++){
        for(int j=0; j<8; j++){
            if(vertices[i][j]){
                //on dessine l'arrête directement dans la mémoire VGA, l'écran fait 80x25
                drawline(screen[i], screen[j]);
            }
        }
    }

/*
    //on dessine un point rouge au projeté de chaque sommet
    for(int i = 0; i<8; i++){
        int x = (int)(edges[i][0] * dist) / (edges[i][2] + dist);
        int y = (int)(edges[i][1] * dist) / (edges[i][2] + dist);
        int index = x + y*80;
        *(VGA_MEMORY + index*2) = 0x0F; //'*'
        *(VGA_MEMORY + index*2 +1) = FOREGROUND_RED;
    }*/
    

}
#pragma endregion cmds

void endCmd(){
    PrintString("\n\r>");
}

void errorCmd(char* cmd){
    PrintString("--Unknown cmd: ", FOREGROUND_RED);
    PrintString(cmd, FOREGROUND_RED);
    endCmd();
}


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

void asm_handler(uint64_t arg0, uint64_t arg1, uint64_t ptr){
    asm ( "mov %1, %%rax\n\t"
          "mov %2, %%rbx\n\t"
          "call *%0\n\t"
    : : "c"(ptr), "m"(arg0), "m"(arg1)
    );
    return;
}


uint8_t get_nb_of_args(char* cmd){
    uint8_t n = 0;
    uint8_t args = 0;
    while(cmd[n] != '\0'){
        n++;
        if(cmd[n] == ' '){
            args++;
        }
    }
    return args + (*cmd != '\0');
}

uint8_t strportionlength(char* str, char sep){
    uint8_t n = 0;
    while(str[n] != sep && str[n] != '\0'){
        n++;
    }
    return str[n] == '\0' ? (sep==0 ? n : -1) : n+1;
}

char** separe_string(char* str, char separator){
    uint8_t nb_of_args = get_nb_of_args(str);
    char** res = (char**)malloc(sizeof(char*) * nb_of_args);
    char** res_begin = res;
    char* str_begin = str;
    uint8_t res_debut = 0;
    while(*str != '\0'){
        uint8_t len = strportionlength(str, separator);
        *res = (char*)malloc(len+1);
        for(int i = 0; i<len; i++){
            (*res)[i] = str[i];
        }
        (*res)[len] = '\0';
        res++;
        str += len;
    }
    return res_begin;
}


void handleCmds(char* cmd){
    //todo separer la cmd et les arguments
    //PrintString("handling...\n\r", FOREGROUND_GREEN);
    PrintString("\n\r");
    uint8_t nb_of_args = get_nb_of_args(cmd);
    
    if(nb_of_args == 0){
        Print("\n\rNo cmd\n\r");
        endCmd();
        return;
    }

    char** args = separe_string(cmd, ' ');

    //affichage des arguments
    for(int i = 0; i<nb_of_args; i++){
        PrintString(args[i], FOREGROUND_LIGHTCYAN);
        PrintString("\n\r", FOREGROUND_LIGHTCYAN);
    }

    if(strcmp(args[0], "clear")){
        for(int i = 0; i<nb_of_args; i++){
            free(args[i]);
        }
        free(args);
        clearCmd();
        return;
    }

    //on essaye de trouver la commande dans bin
    uint16_t bin = find_file_inode_by_name(0, "bin");
    if(bin == 0xFFFF){
        PrintString("No bin folder found\n\r", FOREGROUND_RED);
        for(int i = 0; i<nb_of_args; i++){
            free(args[i]);
        }
        free(args);
        return;
    }

    uint16_t cmd_inode = find_file_inode_by_name(bin, args[0]);
    if(cmd_inode == 0xFFFF){
        errorCmd(args[0]);
        for(int i = 0; i<nb_of_args; i++){
            free(args[i]);
        }
        free(args);
        return;
    }


    //on lit le fichier
    uint8_t* buffer = (uint8_t*)malloc(BLOCK_SIZE);
    read_begin_of_file(cmd_inode, (char*)buffer, BLOCK_SIZE);

    //on execute la commande
    asm_handler((uint64_t)args[1], (uint64_t)args[2], (uint64_t)buffer);
    free(buffer);


    endCmd();
    return;

}
