#include "Header Files/Commands.h"

/*
const char* colorString[] = {"blue", "green", "red", "purple", "white", "black"};
const int colorForeground[] = {};
const int colorBackground[] = {};
*/



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


//pour les commandes on va utiliser un dictionaire 