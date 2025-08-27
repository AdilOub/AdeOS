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

uint8_t ping(uint8_t argc, char* args[]){
    if(argc < 2){
        broadcast_ping((const uint8_t*)"hello from adeOs", 17, 12);
        printf("pingning...\n");
    }
    else{
        printf("todo...\n");
    }

    return 0;
}

uint8_t test(uint8_t argc, char* args[]){
    char* path = get_path_to_root(2);
    printf("Path: '%s'", path);
    free(path);
    return 0;
}


uint8_t helpCmd(uint8_t argc, char* args[]){
    printf(" Cmds: help; clear; arrow; lang-fr; lang-en; slt; fatal; reboot; cube. argc: %d.", argc);
    for(int i = 0; i< argc; i++){
        printf("Value of arg %d: '%s'. ", i, args[i]);
    }
    endCmd();
    return 0;
}

uint8_t clearCmd(uint8_t argc, char* args[]){
    ClearScreen();
    SetCursorPosition(PosFromCoord(0,0));
    endCmd();
    return 0;
}

uint8_t arrowCmd(uint8_t argc, char* args[]){
    if(getArrowState()){
        enableArrow(false);
        PrintString(" Arrow disabled");
    }else{
        enableArrow(true);
        PrintString(" Arrow enabled");
    }
    endCmd();
    return 0;
}

uint8_t sltCmd(uint8_t argc, char* args[]){
    ClearScreen(BACKGROUND_BLACK);
    SetCursorPosition(0);
    //PrintString(Test);
    printf("todo :(");
    endCmd();
    return 0;
}

uint8_t fatalCmd(uint8_t argc, char* args[]){
    PrintString(" Generating fatal error...", FOREGROUND_RED);
    endCmd();
    int a = 1/0;
    return 0;
}

uint8_t ls(uint8_t argc, char* args[]){
    folder* info = read_folder_info(0);
    print_folder_info(info);
    destroy_folder(info);
    return 0;
}

//TODO meilleur truc car la cest terrible.
//Nottament gestion du path avec le shell (faire comme linux et utiliser arg0)
uint8_t touch(uint8_t argc, char* args[]){
    if(argc < 2){
        printf("Usage: touch <filename>");
        endCmd();
        return 1;
    }
    uint16_t f = create_file_in_parent(0, args[1], "hi honey this is a test file!\0", 32);
    if(f == 0xFFFF){
        printf("Error creating file\n");
        return 2;
    }else{
        printf("File '%s' created with inode %d! \n", args[1], f);
    }
    endCmd();

    return 0;
}

uint8_t cat(uint8_t argc, char* args[]){

    if(argc < 2){
        printf("Usage: cat <filename>");
        endCmd();
        return 1;
    }

    uint16_t f = find_file_inode_by_name(0, args[1]);
    if(f == 0xFFFF){
        printf("File not found");
        endCmd();
        return 2;
    }

    char* buffer = (char*)malloc(sizeof(char)*512);
    read_file(f, buffer, 512);
    //printf("File content: '%s'", buffer);
    PrintString(buffer);
    free(buffer);
    endCmd();

    return 0;
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
    printf("Unknown command '%s'.", cmd);
    endCmd();
}


void clearCmdBuffer(char* buffer, int bufferSize){
    for(int i = 0; i<bufferSize; i++){
        buffer[i] = '\0';
    }
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
        (*res)[0] = 0;
        for(int i = 0; i<len; i++){
            (*res)[i] = str[i];
        }
        (*res)[len-1] = '\0';
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


void hellow(){
    Print("Helloow ^_^\n\r", FOREGROUND_LIGHTMAGENTA);
}


//TODO faire un truc plus propre en utilisant es et des décalages (pour gerer les 64 bits de mémoires !)
void addCommandToFileSystem(const char* name, uint64_t fptr){
    uint8_t* buffer = functionToAsmHandler(fptr);
    uint16_t bin = find_file_inode_by_name(0, "bin");
    if(bin == 0xFFFF){
        PrintString("No bin folder found in adding\n\r", FOREGROUND_RED);
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
    addCommandToFileSystem("ls", (uint64_t)ls);
    addCommandToFileSystem("touch", (uint64_t)touch);
    addCommandToFileSystem("cat", (uint64_t)cat);
    addCommandToFileSystem("test", (uint64_t)test);
    addCommandToFileSystem("ping", (uint64_t)ping);

}


void handleCmds(char* cmd){

    if(*cmd == 0){
        return;
    }
   

    PrintString("\n\r");

    uint16_t argc = strargc(cmd, ' ');
    char** argv = splitargv(cmd, ' ');

    uint16_t bin = find_file_inode_by_name(0, "bin");

    if(bin == 0xFFFF){
        PrintString("No bin folder found\n\r", FOREGROUND_RED);
        freeargv(argv, argc);
        return;
    }


    uint16_t cmd_inode = find_file_inode_by_name(bin, argv[0]);
    if(cmd_inode == 0xFFFF){
        errorCmd(argv[0]);
        freeargv(argv, argc);
        return;
    }

    //TODO: modifier argv[0] pour qu'il corresponde au path !

    //on lit le fichier
    uint8_t* loaded_cmd = (uint8_t*)malloc(BLOCK_SIZE);
    read_file(cmd_inode, (char*)loaded_cmd, BLOCK_SIZE); 

    //uint8_t (*func)(uint8_t, char**) = (void*)buffer; 
    void* f = (void*)loaded_cmd;
    uint8_t (*func)(uint8_t, char**) = (uint8_t(*)(uint8_t, char**))f;
    uint8_t res = (*func)(argc, argv);


    printf("[-] Exit code: %d\n", res);

    freeargv(argv, argc);
    free(loaded_cmd);

    return;

}
