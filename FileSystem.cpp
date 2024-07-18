#include "Header Files/FileSystem.h"


SuperBlock superBlock;

//peut être ajouter en argument l'adresse de début et la taille du disque ?
int searchSuperBlock(){
    Print("\n\rSearching...\n\r");
    //pour simplifier on va pour l'instant admettre qu'il est en 0xA000
    uint_8* buffer = (uint_8*)malloc(512); //le superBlock est une sorte d'inode donc 512 bytes
    readDataATA(0xA000, 512, buffer);
    Print("boucle ????");
    if(*buffer != 0xad || *(buffer+1) != 0xe7){
        Print("No super block detected !\n\r");
        return -1;
    }

    buffer+=2;

    superBlock.magic = 0xade7;
    superBlock.blocks_count = (uint_64)(*(++buffer)<<24) | (*(++buffer)<<16) | (*(++buffer)<<8) | *(++buffer); //taille: 32bits ie 4 bytes
    superBlock.inode_count = (uint_64)(*(++buffer)<<24) | (*(++buffer)<<16) | (*(++buffer)<<8) | *(++buffer); 

    return 0;
}

