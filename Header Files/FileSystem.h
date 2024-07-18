#pragma once

#include "DiskRead.h"
#include "Typedefs.h"
#include "Render.h"

//on doit définir une structure cohérente pour le filesystem.
//actuellement l'idée c'est qu'il commence en 0xA000 dans le fichier 
//ensuite on pourra chercher dans le fichier un block qui commence par le magic number

//limiation: on peut lire & écrire des blocs de 512 bytes

typedef struct SuperBlock SuperBlock;
typedef struct inode inode;

struct SuperBlock
{
    uint_16 magic; //should be 0xade7
    uint_64 blocks_count;
    uint_32 inode_count;
    inode* inodes; //inode du root  
};

//un fichier c'est quoi ?
//c'est un inode + les donnée

struct inode{
    uint_64 size;
    uint_64 location;
    uint_16 creation_tmp;
    uint_16 modification_tmp;
    bool is_dir;
    char name[64]; //askip c'est pas bien mébon...
    inode* parent;
    uint_32 children_count;
    inode* children;
}; //j'ai un peu pris les types au hasard pour que la structure fasse pile 64bytes



//vu que les inodes doivent contenir une liste, on va faire en sorte qu'ils fassent tous 512bytes




int searchSuperBlock();