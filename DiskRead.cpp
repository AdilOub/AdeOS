#include "Header Files/DiskRead.h"
#include "Header Files/IO.h"


//ATTENTION RISQUE DE POSER DES PROBLEMES 
//lire https://wiki.osdev.org/ATA_PIO_Mode#Device_Control_Register_(Control_base_+_0)
//surtout x86 Direction, 28bit PIO !!!


//numsector: nombre de secteur de 512 octets à lire
//taille des registres: 
//rax: 64 bits
//eax: 32 bits
//ax: 16 bits
//ah/al: 8 bits
void readDataATASector(uint_64 lba, uint_8 num_sectors, uint_8* buffer) {
    
    uint_32 masked_lba = lba & 0x0FFFFFFF;

    outb(0x01F6, 0xE0 | ((masked_lba >> 24) & 0x0F)); // Bits 24-27 du LBA avec le mode LBA

    outb(0x01F2, num_sectors); // Nombre de secteurs à lire

    outb(0x01F3, masked_lba & 0xFF);        // Bits 0-7 du LBA
    outb(0x01F4, (masked_lba >> 8) & 0xFF);  // Bits 8-15 du LBA
    outb(0x01F5, (masked_lba >> 16) & 0xFF); // Bits 16-23 du LBA

    outb(0x01F7, 0x20); // Commande : lire avec retry

    while(!(inb(0x01F7) & 0x08));

    

    uint_16 dataPort = 0x1F0;
    uint_64 size = num_sectors * 512;
    uint_16* data = (uint_16*)malloc(2);

	//TODO il y a un décalage, des données ne sont jamais lu et ça fait que le disque est encore en cours de lecture
    for(uint_64 i = 0; i < size; i+=2){
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
uint_8 get_status(){
	outb(0x1f6, 0x04);
    outb(0x1f6, 0x00);
	uint_8 status;
    status = inb(0x1f6);
    status = inb(0x1f6);
    status = inb(0x1f6);
    status = inb(0x1f6);
	return status;
}

void resetDisk(){
	uint_32 portCommand = 0x01f7;
	outb(portCommand, 0xE7 ); //reset
	return;
}

void writeDataATASector(uint_64 lba, uint_8 num_sectors, uint_8* buffer){
    uint_32 masked_lba = lba & 0x0FFFFFFF;

    outb(0x01F6, 0xE0 | ((masked_lba >> 24) & 0x0F)); // Bits 24-27 du LBA avec le mode LBA

    outb(0x01F2, num_sectors); // Nombre de secteurs à lire

    outb(0x01F3, masked_lba & 0xFF);        // Bits 0-7 du LBA
    outb(0x01F4, (masked_lba >> 8) & 0xFF);  // Bits 8-15 du LBA
    outb(0x01F5, (masked_lba >> 16) & 0xFF); // Bits 16-23 du LBA

	//0x01F7 port command
    outb(0x01F7, 0x30); //0x30 : write sectors with retry

    while(!(inb(0x01F7) & 0x08));
    

    uint_16 dataPort = 0x1F0;
    uint_64 size = num_sectors * 512;



	for(uint_64 i = 0; i < size; i+=2){
		outsw(dataPort, buffer+i);
	}

	// uint_8* error = (uint_8*)malloc(sizeof(uint_8)*2);
	// *error = 0xca;
	// *(error+1) = 0xfe;

	while(get_status() & 0b01000000){ //TODO Fix may cause infinite loop
		//still busy
		//outsw(dataPort, error);
	}

	//PrintString("write ok\n");

	//free(error);

	return;
}

void readDataATA(uint_64 lba, uint_64 size, uint_8* buffer){
    uint_8 num_sector = (size / 512) + (size%512) ? 1 : 0;
    PrintString("Num sector: ");
    PrintString(IntToString(num_sector));
    uint_8* buffer_aligned = (uint_8*)malloc(sizeof(uint_8)*num_sector*512);
    PrintString("AA");
    readDataATASector(lba, num_sector, buffer_aligned);
    PrintString("BB");
    memcopy(buffer, buffer_aligned, sizeof(uint_8)*size);
    return;
}

void writeDataATA(uint_64 lba, uint_64 size, uint_8* buffer){
    uint_8 num_sector = (size / 512) + (size%512) ? 1 : 0;
    uint_8* buffer_aligned = (uint_8*)malloc(sizeof(uint_8)*num_sector*512);
    readDataATASector(lba, num_sector, buffer_aligned);
    memcopy(buffer_aligned, buffer, sizeof(uint_8)*size);
    writeDataATASector(lba, num_sector, buffer_aligned);

}