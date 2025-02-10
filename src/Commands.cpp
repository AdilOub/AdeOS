#include "includes/Commands.h"
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
void helpCmd(){
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

void asm_no_arg_handler(uint64_t ptr){
    asm (   "lea (%0), %%rax\n\t"
            "call *%%rax\n\t"
    : : "c"(ptr)
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


void hi(){
    PrintString("Hi\n\r", FOREGROUND_LIGHTMAGENTA);
}

//va compiler un "lea rax, [function_ptr]; call *rax"
uint8_t* functionToAsmHandler(uint64_t function_ptr){
   uint8_t* buffer = (uint8_t*)malloc(11);

    uint8_t add_1 = (uint8_t)function_ptr;
    uint8_t add_2 = (uint8_t)(function_ptr >> 8);
    uint8_t add_3 = (uint8_t)(function_ptr >> 16);
    uint8_t add_4 = (uint8_t)(function_ptr >> 24);

    if(function_ptr >> 32 != 0){
        PrintString("Function too far\n\r", FOREGROUND_RED);
        return NULL;
    }

    //TODO verifier que le jmp est pas trop loin pour l'instant
    //plus tard on essayera de gerer les jmps plus loin


    //lea rax, [function_ptr]; call rax; ret
   uint8_t asm_code[11] = { 0x48, 0x8D, 0x04, 0x25, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xD0, 0xC3 };
    for(int i = 0; i<11; i++){
         buffer[i] = asm_code[i];
    }
    buffer[4] = add_1;
    buffer[5] = add_2;
    buffer[6] = add_3;
    buffer[7] = add_4;


    return buffer;
}

//TODO faire un truc plus propre en utilisant es et des décalages (pour gerer les 64 bits de mémoires !)
void addCommandToFileSystem(char* name, uint64_t fptr){
    uint8_t* buffer = functionToAsmHandler(fptr);
    uint16_t bin = find_file_inode_by_name(0, "bin");
    if(bin == 0xFFFF){
        PrintString("No bin folder found\n\r", FOREGROUND_RED);
        return;
    }

    uint64_t size = 11;
    uint16_t f = create_file_in_parent(bin, name, (char*)buffer, size);
    free(buffer);
    return;
}

void add_basic_commands(){
    addCommandToFileSystem("hi", (uint64_t)hellow);
    addCommandToFileSystem("help", (uint64_t)helpCmd);
    addCommandToFileSystem("clear", (uint64_t)clearCmd);
}

//todo gerer les arguments !
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

    // if(strcmp(args[0], "clear")){
    //     for(int i = 0; i<nb_of_args; i++){
    //         free(args[i]);
    //     }
    //     free(args);
    //     clearCmd();
    //     return;
    // }

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
    if(nb_of_args == 1){
        Print("No args\n\r", FOREGROUND_LIGHTGREEN);
        asm_no_arg_handler((uint64_t)buffer);
        free(buffer);
        for(int i = 0; i<nb_of_args; i++){
            free(args[i]);
        }
        free(args);
        endCmd();
        return;
    }

    asm_handler((uint64_t)args[1], (uint64_t)args[2], (uint64_t)buffer);
    free(buffer);


    endCmd();
    return;

}
