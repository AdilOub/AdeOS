#include "includes/FileSystem.h"



int strlen(const char* str){
    int i = 0;
    while(str[i] != '\0'){
        i++;
    }
    return i;
}


void read(char* buffer, int size, int offset){
    readDATA(offset+DISK_OFFSET, size, (uint8_t*)buffer);
}


void write(char* buffer, int size, int offset) {
    writeDATA(offset+DISK_OFFSET, size, (uint8_t*)buffer);
}

uint16_t* read_fat_table(){
    uint16_t *fat_table = (uint16_t*)malloc(FAT_TABLE_SIZE * sizeof(uint16_t));
    read((char*)fat_table, FAT_TABLE_SIZE * sizeof(uint16_t), 0);
    return fat_table;
}

uint16_t get_next_free_inode_nb(){
    uint16_t* fat_table = read_fat_table();
    for(uint16_t i = 0; i < FAT_TABLE_SIZE; i++){
        if(fat_table[i] == FREE_BLOCK){
            free(fat_table);
            return i;
        }
    }
    free(fat_table);
    return -1;
}

uint16_t get_nth_inode(uint16_t n){
    uint16_t* fat_table = read_fat_table();
    uint16_t r = fat_table[n];
    free(fat_table);
    return r;
}

void write_nth_inode(uint16_t n, uint16_t value){
    uint16_t* fat_table = read_fat_table();
    fat_table[n] = value;
    write((char*)fat_table, FAT_TABLE_SIZE * sizeof(uint16_t), 0);
    free(fat_table);
    return;
}


uint16_t find_not_used_in_folder(uint16_t parent){
    if(get_nth_inode(parent) != FOLDER_SIGNATURE){
        PrintString("Parent is not a folder\n");
        return -1;
    }

    uint16_t* ptr = (uint16_t*)malloc(sizeof(uint16_t));
    *ptr = 0x0000;

    for(int i = 0; i<MAX_INODE_PER_DIR; i++){
        read((char*)ptr, sizeof(uint16_t), parent * BLOCK_SIZE + FAT_TABLE_SIZE + (i+1) * LINK_SIZE);
        if(*ptr == NOT_USED){
            free(ptr);
            return i;
        }
    }
    free(ptr);
    return -1;
}

uint16_t find_inode_in_folder(uint16_t parent, uint16_t inode){
    if(get_nth_inode(parent) != FOLDER_SIGNATURE){
        PrintString("Parent is not a folder\n");
        return -1;
    }

    uint16_t* ptr = (uint16_t*)malloc(sizeof(uint16_t));
    *ptr = 0x0000;

    for(int i = 0; i<MAX_INODE_PER_DIR; i++){
        read((char*)ptr, sizeof(uint16_t), parent * BLOCK_SIZE + FAT_TABLE_SIZE + (i+1) * LINK_SIZE);
        if(*ptr == inode){
            free(ptr);
            return i;
        }
    }
    free(ptr);
    return -1;
}


uint16_t create_folder(uint16_t parent, char* name){
    //on l'ajoute à la fat table
    uint16_t new_inode = get_next_free_inode_nb();
    write_nth_inode(new_inode, FOLDER_SIGNATURE);

    //on écrit ensuite le parent dans le cluster du nouveau dossier
    uint16_t* ptr = (uint16_t*)malloc(sizeof(uint16_t));
    *ptr = parent;
    write((char*)ptr, sizeof(uint16_t), new_inode * BLOCK_SIZE + FAT_TABLE_SIZE);
    write("<-pof ", 6, new_inode * BLOCK_SIZE + FAT_TABLE_SIZE + sizeof(uint16_t)); //TODO rmv after debug
    write(name, strlen(name), new_inode * BLOCK_SIZE + FAT_TABLE_SIZE + sizeof(uint16_t) + 6); //TODO rmv after debug

    //le nouveau dossier est vide, on met tous ses inodes fils à NOT_USED
    *ptr = NOT_USED;
    for(int i = 0; i<MAX_INODE_PER_DIR; i++){
        write((char*)ptr, sizeof(uint16_t), new_inode * BLOCK_SIZE + FAT_TABLE_SIZE + (i+1) * LINK_SIZE);
    }
    
    free(ptr);
    return new_inode;
}

uint16_t create_folder_in_parent(uint16_t parent, char* name){
    //on récupère le premier liens libre dans le dossier parent
    uint16_t not_used = find_not_used_in_folder(parent);
    if(not_used == 0xFFFF){
        PrintString("No more space in parent folder\n\r");
        return -1;
    }

    //PrintString("Creating folder s in parent d at pos s: ");
    //PrintString(name); PrintString(" ");PrintString(IntToString(parent)); PrintString(" ");PrintString(IntToString(not_used));
    //PrintString("\n\r");

    //on crée le dossier
    uint16_t new_inode = create_folder(parent, name);
    uint16_t* ptr = (uint16_t*)malloc(sizeof(uint16_t));
    *ptr = new_inode;

    //on écrit le nom du dossier et son lien dans le parent
    write((char*)ptr, sizeof(uint16_t), parent * BLOCK_SIZE + FAT_TABLE_SIZE + (not_used+1) * LINK_SIZE);
    write(name, strlen(name), parent * BLOCK_SIZE + FAT_TABLE_SIZE + (not_used+1) * LINK_SIZE + sizeof(uint16_t));

    return new_inode;
}


uint16_t create_file(uint16_t parent, char* data, uint16_t nb_of_cluster, uint64_t size){
    //on veut commencer par trouver nb_of_cluster clusters libres dnas la fat table
    uint16_t* clusters = (uint16_t*)malloc(nb_of_cluster * sizeof(uint16_t));

    for(int chunk = 0; chunk < nb_of_cluster; chunk++){
        clusters[chunk] = get_next_free_inode_nb();
        write_nth_inode(clusters[chunk], BAD_BLOCK);
        if(clusters[chunk] == 0xFFFF){
            PrintString("No more space in the fat table\n\r");
            return -1;
        }
        //si c'est le dernier, on écrit que la taille restante
        //TODO fix et écrire la taille restante + 0 en padding pour la fin de block
        if(chunk == nb_of_cluster-1){
            write(data+chunk*BLOCK_SIZE, size%BLOCK_SIZE, clusters[chunk] * BLOCK_SIZE + FAT_TABLE_SIZE);
        }else{
            write(data + chunk * BLOCK_SIZE, BLOCK_SIZE, clusters[chunk] * BLOCK_SIZE + FAT_TABLE_SIZE);
        }
        
        //write(data + chunk * BLOCK_SIZE, chunk == nb_of_cluster-1 ? size%BLOCK_SIZE : BLOCK_SIZE, clusters[chunk] * BLOCK_SIZE + FAT_TABLE_SIZE); //WHY DECALAGE DE 7
    }

    for(int chunk = 0; chunk < nb_of_cluster-1; chunk++){
        //printf("Writing %d to %d\n\r", clusters[chunk], clusters[chunk+1]);
        write_nth_inode(clusters[chunk], clusters[chunk+1]);
    }
    //printf("Writing %d to %d(EOF)\n\r", clusters[nb_of_cluster-1], END_OF_FILE);
    write_nth_inode(clusters[nb_of_cluster-1], END_OF_FILE);

    uint16_t first_cluster = clusters[0];
    free(clusters);
    return first_cluster;
}

uint16_t create_file_in_parent(uint16_t parent, char* name, char* data, uint64_t size){
    //uint64_t size = strlen(data);
    uint16_t nb_of_cluster = (size / BLOCK_SIZE) + !!(size%BLOCK_SIZE); //j'aime bien, !! pour avoir 0 ou 1
    //uint16_t nb_of_cluster = (size >> BLOCK_SIZE_POW2) + !!(size & BLOCK_SIZE_MASK); //encore plus opti parceque why not mdr

    //on récupère le premier liens libre dans le dossier parent //TODO verifier que le nom est unique !
    uint16_t not_used = find_not_used_in_folder(parent);
    if(not_used == 0xFFFF){
        PrintString("No more space in parent folder\n\r");
        return -1;
    }
    //printf("Creating file %s in parent %d at pos %d with %d clusters\n\r", name, parent, not_used, nb_of_cluster);


    //on crée le fichier
    uint16_t new_inode = create_file(parent, data, nb_of_cluster, size);
    uint16_t* ptr = (uint16_t*)malloc(sizeof(uint16_t));
    *ptr = new_inode;

    //on écrit le nom du fichier et son lien dans le parent
    write((char*)ptr, sizeof(uint16_t), parent * BLOCK_SIZE + FAT_TABLE_SIZE + (not_used+1) * LINK_SIZE);
    write(name, strlen(name), parent * BLOCK_SIZE + FAT_TABLE_SIZE + (not_used+1) * LINK_SIZE + sizeof(uint16_t));

    return new_inode;
}


void setup_root(){
    //on initialise le disque dur en overwritant le root
    write_nth_inode(0, FOLDER_SIGNATURE);
    Print("Wrote!\n\r", FOREGROUND_GREEN);

    uint16_t new_inode = 0; //OVERWRITE ROOT
    uint16_t parent = NOT_USED;

    uint16_t* ptr = (uint16_t*)malloc(sizeof(uint16_t));
    *ptr = parent;
    write((char*)ptr, sizeof(uint16_t), new_inode * BLOCK_SIZE + FAT_TABLE_SIZE);
    write("<-root", 6, new_inode * BLOCK_SIZE + FAT_TABLE_SIZE + sizeof(uint16_t)); //TODO rmv after debug

    *ptr = NOT_USED;
    for(int i = 0; i<MAX_INODE_PER_DIR; i++){
        write((char*)ptr, sizeof(uint16_t), new_inode * BLOCK_SIZE + FAT_TABLE_SIZE + (i+1) * LINK_SIZE);
    }

    
    free(ptr);
    return;
}

uint8_t delete_inode(uint16_t parent, uint16_t inode){
    write_nth_inode(inode, FREE_BLOCK);
    uint16_t pos = find_inode_in_folder(parent, inode);
    if(pos == 0xFFFF){
        PrintString("Inode not found in parent\n\r");
        return -1;
    }
    uint16_t* ptr = (uint16_t*)malloc(sizeof(uint16_t));
    *ptr = NOT_USED;
    write((char*)ptr, sizeof(uint16_t), parent * BLOCK_SIZE + FAT_TABLE_SIZE + (pos+1) * LINK_SIZE);
    free(ptr);
    
    return 0;
}


uint16_t read_begin_of_file(uint16_t inode, char* buffer, int size){
    uint16_t* fat_table = read_fat_table();
    uint16_t current = inode;
    int i = 0;
    while(current != END_OF_FILE && i < size){
        read(buffer + i * BLOCK_SIZE, BLOCK_SIZE, current * BLOCK_SIZE + FAT_TABLE_SIZE);
        current = fat_table[current];
        i++;
    }
    free(fat_table);
    return i;
}

void write_in_data(char* data, const char* stuff){
    for(int i = 0; i<strlen(stuff); i++){
        data[i] = stuff[i];
    }
    //data[strlen(stuff)] = '\0';
}

/*uint16_t* read_folder(uint16_t parent){
    if(get_nth_inode(parent) != FOLDER_SIGNATURE){
        PrintString("Parent is not a folder\n\r");
        return NULL;
    }
    //print max inode per dir

    uint16_t* inodes = (uint16_t*)malloc(MAX_INODE_PER_DIR * sizeof(uint16_t));
    
    for(uint16_t i=0; i<MAX_INODE_PER_DIR; i++){
        inodes[i] = -1;
    }

    uint16_t* ptr = (uint16_t*)malloc(sizeof(uint16_t));
    for(uint16_t i = 0; i<MAX_INODE_PER_DIR; i++){
        read((char*)ptr, sizeof(uint16_t), parent * BLOCK_SIZE + FAT_TABLE_SIZE + (i+1) * LINK_SIZE);
        inodes[i] = *ptr;
    }
    free(ptr);
    return inodes;
}*/

folder* read_folder_info(uint16_t inode){
    if(get_nth_inode(inode) != FOLDER_SIGNATURE){
        PrintString("Inode is not a folder\n\r");
        return NULL;
    }

    folder* f = (folder*)malloc(sizeof(folder));
    f->inode = inode;
    char* buffer = (char*)malloc(sizeof(char)*BLOCK_SIZE);
    read(buffer, BLOCK_SIZE, inode * BLOCK_SIZE + FAT_TABLE_SIZE);
    f->name = (char*)malloc(sizeof(char)*NAME_SIZE);
    memcopy(f->name, buffer+SIGNATURE_SIZE, NAME_SIZE);
    f->parent = *(uint16_t*)buffer;

    uint16_t nb_of_cluster = 0;
    f->children_names = (char**)malloc(sizeof(char*)*MAX_INODE_PER_DIR);
    f->children_inodes = (uint16_t*)malloc(sizeof(uint16_t)*MAX_INODE_PER_DIR);
    for(int i = 0; i<MAX_INODE_PER_DIR; i++){
        uint8_t* child_inode = (uint8_t*)buffer + SIGNATURE_SIZE + NAME_SIZE + i * LINK_SIZE;
        if(*((uint16_t*)child_inode) != NOT_USED){
            nb_of_cluster++;
        }
        f->children_inodes[i] = *((uint16_t*)child_inode);
        f->children_names[i] = (char*)malloc(sizeof(char)*(NAME_SIZE+1));
        memcopy(f->children_names[i], child_inode+SIGNATURE_SIZE, NAME_SIZE);
        f->children_names[i][NAME_SIZE] = '\0';
    }
    f->nb_of_cluster = nb_of_cluster;
    free(buffer);
    return f;
}

uint8_t check_if_root(){
    uint8_t* buffer = (uint8_t*)malloc(sizeof(uint8_t)*SIGNATURE_SIZE);
    *buffer = 0x00;
    *(buffer+1) = 0x00;
    Print("Reading signature\n\r", FOREGROUND_RED);
    read((char*)buffer, SIGNATURE_SIZE, 0); 
    Print("Read signature\n\r", FOREGROUND_GREEN);
    if(*buffer == 0xFC && *(buffer+1) == 0xFF){ //TODO modifier ça si on change SIGNATURE_SIZE
        free(buffer);
        return 1;
    }
    free(buffer);
    return 0;
}

void destroy_folder(folder* f){
    free(f->name);
    for(int i = 0; i<MAX_INODE_PER_DIR; i++){
        free(f->children_names[i]);
    }
    free(f->children_names);
    free(f->children_inodes);
    free(f);
}


//TODO restructuré le projet un peu mdr
bool strcmp2(char* a, char* b){
    int n = 0;
    while(a[n] != '\0'){
        if(a[n] != b[n]){
            return false;
        } 
        n++;
    }

    return a[n] == b[n];
}

uint16_t find_file_inode_by_name(uint16_t parent, char* name){
    folder* f = NULL;
    f = read_folder_info(parent);
    if(f == NULL){
        return -1;
    }
    for(int i = 0; i<MAX_INODE_PER_DIR; i++){
        if(strcmp2(f->children_names[i], name) == 1){
            uint16_t inode = f->children_inodes[i];
            destroy_folder(f);
            return inode;
        }
    }
    destroy_folder(f);
    return -1;
}


/*

TODO des fonction à modifier si on prend plus de place!

*/

