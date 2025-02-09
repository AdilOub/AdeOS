#include "includes/DiskRead.h"
#include "includes/cpu/IO.h"


//UTILISE LE MODE PIO
//https://wiki.osdev.org/ATA_PIO_Mode (cf. //https://wiki.osdev.org/ATA_PIO_Mode#x86_Code_Examples)

//ATTENTION RISQUE DE POSER DES PROBLEMES car pas de wait
//surtout x86 Direction, 28bit PIO !!!


//numsector: nombre de secteur de 512 octets à lire
//taille des registres: 
//rax: 64 bits
//eax: 32 bits
//ax: 16 bits
//ah/al: 8 bits
void readDataATASector(uint64_t lba, uint8_t num_sectors, uint8_t* buffer) {
    
    uint32_t masked_lba = lba & 0x0FFFFFFF;

    outb(0x01F6, 0xE0 | ((masked_lba >> 24) & 0x0F)); // Bits 24-27 du LBA avec le mode LBA

    outb(0x01F2, num_sectors); // Nombre de secteurs à lire

    outb(0x01F3, masked_lba & 0xFF);        // Bits 0-7 du LBA
    outb(0x01F4, (masked_lba >> 8) & 0xFF);  // Bits 8-15 du LBA
    outb(0x01F5, (masked_lba >> 16) & 0xFF); // Bits 16-23 du LBA

    outb(0x01F7, 0x20); // Commande : lire avec retry

    while(!(inb(0x01F7) & 0x08)); //TODO Fix may cause infinite loop on error (par exemple si le disque est pas de la bonne taille)

    uint16_t dataPort = 0x1F0;
    uint64_t size = num_sectors * 512;
    uint16_t* data = (uint16_t*)malloc(2);

	//TODO il y a un décalage, des données ne sont jamais lu et ça fait que le disque est encore en cours de lecture
    for(uint64_t i = 0; i < size; i+=2){
        insw(dataPort, data); //reversed
        buffer[i+1] = ((*data&0xFF00)>>8);
        buffer[i] = (*data&0x00FF);
    }

    free(data);
    return;
}

//cf.doc mais normalement on a A0 et pas E0 si tout est bon car:
//bit 7: BSY
//bit 6: RDY (clear si rdy)
//bit 5: DF (drive fault) PAS BIEN !!!
uint8_t get_status(){
	outb(0x1f6, 0x04);
    outb(0x1f6, 0x00);
	uint8_t status;
    status = inb(0x1f6);
    status = inb(0x1f6);
    status = inb(0x1f6);
    status = inb(0x1f6);
	return status; //A0->ok, E0->erreur :(
}

void resetDisk(){
	uint32_t portCommand = 0x01f7;
	outb(portCommand, 0xE7 ); //reset
	return;
}

void writeDataATASector(uint64_t lba, uint8_t num_sectors, uint8_t* buffer){
    uint32_t masked_lba = lba & 0x0FFFFFFF;

    outb(0x01F6, 0xE0 | ((masked_lba >> 24) & 0x0F)); // Bits 24-27 du LBA avec le mode LBA

    outb(0x01F2, num_sectors); // Nombre de secteurs à lire

    outb(0x01F3, masked_lba & 0xFF);        // Bits 0-7 du LBA
    outb(0x01F4, (masked_lba >> 8) & 0xFF);  // Bits 8-15 du LBA
    outb(0x01F5, (masked_lba >> 16) & 0xFF); // Bits 16-23 du LBA

	//0x01F7 port command
    outb(0x01F7, 0x30); //0x30 : write sectors with retry

    while(!(inb(0x01F7) & 0x08));
    

    uint16_t dataPort = 0x1F0;
    uint64_t size = num_sectors * 512;



	for(uint64_t i = 0; i < size; i+=2){
		outsw(dataPort, buffer+i);
	}

	// uint8_t* error = (uint8_t*)malloc(sizeof(uint8_t)*2);
	// *error = 0xca;
	// *(error+1) = 0xfe;

	while(get_status() & 0b01000000){ //TODO Fix may cause infinite loop on error
		//still busy
		//outsw(dataPort, error);
	}


	//free(error);

	return;
}

void readDataATA(uint64_t lba, uint64_t size, uint8_t* buffer){
    uint8_t num_sector = (size / 512) + (size%512) ? 1 : 0;
    //PrintString("Num sector: ");
    //PrintString(IntToString(num_sector));
    uint8_t* buffer_aligned = (uint8_t*)malloc(sizeof(uint8_t)*num_sector*512);
    //PrintString("\n\rstart read-");
    readDataATASector(lba, num_sector, buffer_aligned);
    //PrintString("-end read:");
    memcopy(buffer, buffer_aligned, sizeof(uint8_t)*size);
    free(buffer_aligned);
    return;
}

void writeDataATA(uint64_t lba, uint64_t size, uint8_t* buffer){
    PrintString("writeDATA_ATA\n\r", FOREGROUND_LIGHTMAGENTA);
    uint8_t num_sector = (size / 512) + (size%512) ? 1 : 0;
    uint8_t* buffer_aligned = (uint8_t*)malloc(sizeof(uint8_t)*num_sector*512);
    readDataATASector(lba, num_sector, buffer_aligned);
    memcopy(buffer_aligned, buffer, sizeof(uint8_t)*size);
    PrintString("start write-");
    writeDataATASector(lba, num_sector, buffer_aligned);
    PrintString("-end write:");
    free(buffer_aligned);
}

//TODO fix ou test ça, si on lit/ecrit qu'un secteur c'est pas bon
//L'alginement est OK, mais il faut qu'on puisse lire 512+1 octets !!!
//actuellement fine car BLOCKSIZE dans FileSystem est 512 mais pas ouf
void readDATA(uint64_t address, uint64_t size, uint8_t* buffer){
    uint64_t lba = address / 512;
    uint64_t offset = address % 512;
    uint8_t* buffer_aligned = (uint8_t*)malloc(sizeof(uint8_t)*512);
    readDataATASector(lba, 1, buffer_aligned);
    memcopy(buffer, buffer_aligned+offset, size);
    free(buffer_aligned);
    return;
}

void writeDATA(uint64_t address, uint64_t size, uint8_t* buffer){
    uint64_t lba = address / 512;
    uint64_t offset = address % 512;
    uint8_t* buffer_aligned = (uint8_t*)malloc(sizeof(uint8_t)*512);
    readDataATASector(lba, 1, buffer_aligned);
    memcopy(buffer_aligned+offset, buffer, size);
    writeDataATASector(lba, 1, buffer_aligned);
    free(buffer_aligned);
    return;
}