#include "includes/FileSystem.h"




void read(char* buffer, int size, int offset){
    readDATA(offset+DISK_OFFSET, size, (uint8_t*)buffer);
}


void write(const char* buffer, int size, int offset) {
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


uint16_t create_folder(uint16_t parent, const char* name){
    //on l'ajoute à la fat table
    uint16_t new_inode = get_next_free_inode_nb();
    write_nth_inode(new_inode, FOLDER_SIGNATURE);

    //on écrit ensuite le parent dans le cluster du nouveau dossier
    uint16_t* ptr = (uint16_t*)malloc(sizeof(uint16_t));
    *ptr = parent;
    write((char*)ptr, sizeof(uint16_t), new_inode * BLOCK_SIZE + FAT_TABLE_SIZE);
    //write("<-pof ", 6, new_inode * BLOCK_SIZE + FAT_TABLE_SIZE + sizeof(uint16_t)); //TODO rmv after debug, si je le met je dois add +6 arpès sizeof(uint16) en bas
    write(name, strlen(name), new_inode * BLOCK_SIZE + FAT_TABLE_SIZE + sizeof(uint16_t)); //TODO rmv after debug

    //le nouveau dossier est vide, on met tous ses inodes fils à NOT_USED
    *ptr = NOT_USED;
    for(int i = 0; i<MAX_INODE_PER_DIR; i++){
        write((char*)ptr, sizeof(uint16_t), new_inode * BLOCK_SIZE + FAT_TABLE_SIZE + (i+1) * LINK_SIZE);
    }
    
    free(ptr);
    return new_inode;
}

uint16_t create_folder_in_parent(uint16_t parent, const char* name){
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


uint16_t create_file(uint16_t parent, const char* data, uint16_t nb_of_cluster, uint64_t size){
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

uint16_t create_file_in_parent(uint16_t parent, const char* name, const char* data, uint64_t size){
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


uint16_t read_file(uint16_t inode, char* buffer, int size){
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
        printf("Indoe %d is not a folder", inode);
        return NULL;
    }


    folder* f = (folder*)calloc(sizeof(folder));
    f->inode = inode;
    char* buffer = (char*)calloc(sizeof(char)*BLOCK_SIZE);
    read(buffer, BLOCK_SIZE, inode * BLOCK_SIZE + FAT_TABLE_SIZE);
    f->name = (char*)calloc(sizeof(char)*NAME_SIZE);
    memcopy(f->name, buffer+SIGNATURE_SIZE, NAME_SIZE);
    f->parent = *(uint16_t*)buffer;

    
    uint16_t nb_of_cluster = 0;
    f->children_names = (char**)calloc(sizeof(char*)*MAX_INODE_PER_DIR);
    f->children_inodes = (uint16_t*)calloc(sizeof(uint16_t)*MAX_INODE_PER_DIR);
    for(int i = 0; i<MAX_INODE_PER_DIR; i++){
        uint8_t* child_inode = (uint8_t*)buffer + SIGNATURE_SIZE + NAME_SIZE + i * LINK_SIZE;
        if(*((uint16_t*)child_inode) != NOT_USED){
            nb_of_cluster++;
        }
        f->children_inodes[i] = *((uint16_t*)child_inode);
        f->children_names[i] = (char*)calloc(sizeof(char)*(NAME_SIZE+1));
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


void print_folder_info(folder* f){
    printf("Folder name: %s\n", f->name);
    printf("Parent inode: %d\n", f->parent);
    printf("Inode: %d\n", f->inode);
    printf("Number of clusters: %d\n", f->nb_of_cluster);
    for(int i = 0; i<f->nb_of_cluster; i++){
        if(f->children_inodes[i] != NOT_USED){
            printf("CI_%d: '%s'\n", f->children_inodes[i], f->children_names[i]);
        }
    }
    return;
}

uint16_t find_file_inode_by_name(uint16_t parent, char* name){
    folder* f = NULL;
    f = read_folder_info(parent);
    if(f == NULL){
        return 0xFFFF;
    }
    for(int i = 0; i<MAX_INODE_PER_DIR; i++){
        if(strcmp(f->children_names[i], name) == 1 && f->children_inodes[i] != NOT_USED){
            uint16_t inode = f->children_inodes[i];
            destroy_folder(f);
            return inode;
        }
    }
    destroy_folder(f);
    return 0xFFFF;
}


char* get_path_to_root(uint16_t inode){
    if(get_nth_inode(inode) != FOLDER_SIGNATURE){
        PrintString("Not a folder\n\r");
        return NULL;
    }

    folder* f = NULL;
    f = read_folder_info(inode);
    if(f==NULL){
        PrintString("Failed to read folder\n\r");
        return NULL;
    }

    if(inode == 0){
        //c'est le root:
        PrintString("On a le root !");
        char* path = (char*)malloc(2);
        path[0] = '/';
        path[1] = 0;
        return path;
    }

    char* parent_path = get_path_to_root(f->parent);
    char* path = (char*)malloc(strlen(parent_path) + strlen(f->name) + 1);
    memcopy(path, parent_path, strlen(parent_path));
    printf("\n[!] Name: '%s'\n", f->name);
    printf("[!] Parent path: '%s' %d\n", parent_path, strlen(parent_path));
    memcopy(path+strlen(parent_path), f->name, strlen(f->name));
    free(parent_path);
    return path;

    
}

/*

TODO des fonction à modifier si on prend plus de place!

*/

