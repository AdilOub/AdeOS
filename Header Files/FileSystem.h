#pragma once

#include "DiskRead.h"
#include "Typedefs.h"
#include "Render.h"
#include "TextPrint.h"
#include "Heap.h"
#include "Memory.h"

#define FAT_TABLE_SIZE 65536 // nombre de bloc qu'on peut avoir


#define FREE_BLOCK 0
#define END_OF_FILE 0xFFFF
#define BAD_BLOCK 0xFFFE
#define NOT_USED 0xFFFD
#define FOLDER_SIGNATURE 0xFFFC

#define SIGNATURE_SIZE 2 //signature de 2 bytes

//#define BLOCK_SIZE 1024
#define BLOCK_SIZE_POW2 6
#define BLOCK_SIZE (2 << BLOCK_SIZE_POW2)

#define LINK_SIZE 0x10
#define MAX_INODE_PER_DIR (BLOCK_SIZE / LINK_SIZE)-1 

#define DISK_OFFSET 0x10000

#define OS_BLOC_SIZE 512

#define NAME_SIZE (LINK_SIZE - SIGNATURE_SIZE)


struct folder{
    uint16_t parent;
    uint16_t inode;
    char* name;
    uint16_t nb_of_cluster;
    char** children_names;
    uint16_t* children_inodes;
};
typedef struct folder folder;



void setup_root();

//creation
uint16_t create_folder_in_parent(uint16_t parent, char* name);
uint16_t create_file_in_parent(uint16_t parent, char* name, char* data, uint64_t size);
void write_in_data(char* data, const char* stuff);
uint16_t read_begin_of_file(uint16_t inode, char* buffer, int size);


//suppression
uint8_t delete_inode(uint16_t parent, uint16_t inode);

//lecture 
folder* read_folder_info(uint16_t inode);
void destroy_folder(folder* f);

//utils
uint8_t check_if_root();
uint16_t find_file_inode_by_name(uint16_t parent, char* name);