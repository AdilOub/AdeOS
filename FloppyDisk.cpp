#include "Header Files/FloppyDisk.h"
#define WAIT_DURATION 5000


//source:
//http://www.brokenthorn.com/Resources/OSDev20.html

static uint_8	_CurrentDrive = 0; //normalement 0
const int FLPY_SECTORS_PER_TRACK = 18;

#define DMA_BUFFER 0x1000; //par defaut

//c'est juste du code pour configurer le floppy disk en DMA, copier du tuto
//je verrai c'est quoi le DMA après

//! initialize DMA to use phys addr 1k-64k
void flpydsk_initialize_dma () {
 
	outb(0x0a,0x06);	//mask dma channel 2
	outb(0xd8,0xff);	//reset master flip-flop
	outb(0x04, 0);     //address=0x1000 
	outb(0x04, 0x10);
	outb(0xd8, 0xff);  //reset master flip-flop
	outb(0x05, 0xff);  //count to 0x23ff (number of bytes in a 3.5" floppy disk track)
	outb(0x05, 0x23);
	outb(0x80, 0);     //external page register = 0
	outb(0x0a, 0x02);  //unmask dma channel 2
}
 
//! prepare the DMA for read transfer
void flpydsk_dma_read () {
 
	outb(0x0a, 0x06); //mask dma channel 2
	outb(0x0b, 0x56); //single transfer, address increment, autoinit, read, channel 2
	outb(0x0a, 0x02); //unmask dma channel 2
}
 
//! prepare the DMA for write transfer
void flpydsk_dma_write () {
	outb(0x0a, 0x06); //mask dma channel 2
	outb(0x0b, 0x5a); //single transfer, address increment, autoinit, write, channel 2
	outb(0x0a, 0x02); //unmask dma channel 2
}

uint_8 flpydsk_read_status(){
	return inb(FLPYDSK_MSR);
}


void flpydsk_wait_irq(){
	
	//TODO FIX LA RACE CONDITION
	PrintString("flpydsk_wait_irq: waiting for irq ", FOREGROUND_RED);
	
	/*while (get_flpydisk_int() == 0)
	{
	}
	set_flpydisk_int(0);
	*/
	for(int i = 0; i < 100000; i++ ){ //on attend 100000 cycles sinon on risque de bloquer si l'interupt se déclare avant
		if(get_flpydisk_int() == 1){
			set_flpydisk_int(0);
			return;
		}
	}
	PrintString("flpydsk_wait_irq: timeout ", FOREGROUND_RED);
}



void flpydsk_send_command (uint_8 cmd) {
	//on attend que ça soit pret
	for(int i = 0; i < WAIT_DURATION; i++ ){
		if(flpydsk_read_status() & FLPYDSK_MSR_MASK_DATAREG){
			return outb(FLPYDSK_FIFO, cmd);
		}
	}
}

uint_8 flpydsk_read_data () {
 
	//! same as above function but returns data register for reading
	for (int i = 0; i < WAIT_DURATION; i++ )
		if ( flpydsk_read_status () & FLPYDSK_MSR_MASK_DATAREG )
			return inb(FLPYDSK_FIFO);
	
	PrintString("flpydsk_read_data: timeout\n", FOREGROUND_RED);
	return 0;
}

//check interrupt
void flpydsk_check_int (uint_32* st0, uint_32* cyl) {

	flpydsk_send_command (FDC_CMD_CHECK_INT);

	*st0 = flpydsk_read_data ();
	*cyl = flpydsk_read_data ();
}


//les registres utiles:

//le Digital Output Register (DOR) permet de controler les moteurs 
//bits 0-1: drive select
//bit 2: rest (0 = reset, 1 = enabled)
//bit 3: mode (0=irq, 1=DMA)
//bit 4-7: motor control (0 = stop motor for drive, 1 = start motor for drive)

//le Main Status Register (MSR) contient... les informations sur le status du floppy
//bit 0: 1 si fdd 0 occupé en recherche
//bit 1: 1 si fdd 1 occupé en recherche
//bit 2: 1 si fdd 2 occupé en recherche
//bit 3: 1 si fdd 3 occupé en recherche (sinon 0)
//bit 4: 1 si fdc busy
//bit 5: 1 si fcd en non DMA (0 = DMA)
//bit 6: 0 si attend des données du CPU, 1 si il a les données
//bit 7: 0 si data register not ready, 1 si data register ready

//Data Register 8 ou 16 bits, qui contient les données lues ou à écrire
//pas de format particulier, c'est juste un registre de données

//CCR: Configuation Control Register permet de modifier la vitesse de lecture
void flpydsk_write_ccr (uint_8 val) {
	outb(FLPYDSK_CTRL, val); //on ecrit sur ctrl
}

//Commandes:

//Write: M F 0 0 0 1 1 0
//Read: M F S 0 0 1 1 0

//Parameters:
//x x x x HD DR DR0
//cylindre
//tete
//numéro du secteur
//taille du secteur
//taille de la piste (track length)
//taille du gap3
//taille de la data
//Return:
//ST0
//ST1
//ST2
//Cylindre actuel
//Tête actuelle
//Numéro du secteur actuel
//Taille du secteur actuel


void flpydsk_read_sector_imp(uint_8 head, uint_8 track, uint_8 sector){
	uint_32 st0, cyl;
	flpydsk_dma_read(); //activation du mode lecture en DMA

	//on se met en read dans le bon secteur
	flpydsk_send_command(FDC_CMD_READ_SECT | FDC_CMD_EXT_MULTITRACK |
		FDC_CMD_EXT_SKIP | FDC_CMD_EXT_DENSITY); //envoie de la commande de lecture
	flpydsk_send_command(head << 2 | _CurrentDrive);
	flpydsk_send_command(track);
	flpydsk_send_command(head);
	flpydsk_send_command(sector);
	flpydsk_send_command(FLPYDSK_SECTOR_DTL_512);

	flpydsk_send_command (
		( ( sector + 1 ) >= FLPY_SECTORS_PER_TRACK )
			? FLPY_SECTORS_PER_TRACK : sector + 1 );
	flpydsk_send_command(FLPYDSK_GAP3_LENGTH_3_5);
	flpydsk_send_command(0xff);

	//on attend l'interupt du floppy
	flpydsk_wait_irq();

	//on lit les info pour clear le fdc
	for (int j=0; j<7; j++)
		flpydsk_read_data();

	//on dit au fdc que c'est ok
	flpydsk_check_int(&st0, &cyl);

}




//fix drive/data specify, utilisé pour passer des information de controle au FDC
void flpydsk_drive_data (uint_32 stepr, uint_32 loadt, uint_32 unloadt, bool dma ) {
 
	uint_32 data = 0;
	flpydsk_send_command (FDC_CMD_SPECIFY);
	data = ( (stepr & 0xf) << 4) | (unloadt & 0xf);
	flpydsk_send_command (data);
	data = (loadt) << 1 | (dma==true) ? 1 : 0;
	flpydsk_send_command (data);
}


//allume le moteur
void flpydsk_control_motor (bool b) {

	//! sanity check: invalid drive
	if (_CurrentDrive > 3)
		return;

	uint_32 motor = 0;

	//! select the correct mask based on current drive
	switch (_CurrentDrive) {

		case 0:
			motor = FLPYDSK_DOR_MASK_DRIVE0_MOTOR;
			break;
		case 1:
			motor = FLPYDSK_DOR_MASK_DRIVE1_MOTOR;
			break;
		case 2:
			motor = FLPYDSK_DOR_MASK_DRIVE2_MOTOR;
			break;
		case 3:
			motor = FLPYDSK_DOR_MASK_DRIVE3_MOTOR;
			break;
	}

	//! turn on or off the motor of that drive
	if (b)
		outb(FLPYDSK_DOR, _CurrentDrive | motor | FLPYDSK_DOR_MASK_RESET | FLPYDSK_DOR_MASK_DMA);
	else
		outb(FLPYDSK_DOR, FLPYDSK_DOR_MASK_RESET);

	//on est censé attendre, mais on n'utilise pas vraiment de floppy disk
	//donc y'a pas de moteur à attendre
	for(int i = 0; i < 100000; i++){
		asm("nop");
	}
}

//calibrate drive: positioner la tete de lecture sur le cylindre 0
int flpydsk_calibrate (uint_32 drive) {

	uint_32 st0, cyl;
 
	if (drive >= 4)
		return -2;
 
	//! turn on the motor
	flpydsk_control_motor (true);
 
	for (int i = 0; i < 10; i++) {
 
		//! send command
		flpydsk_send_command ( FDC_CMD_CALIBRATE );
		flpydsk_send_command ( drive );
		flpydsk_wait_irq ();
		flpydsk_check_int ( &st0, &cyl);
 
		//! did we fine cylinder 0? if so, we are done
		if (!cyl) {
 
			flpydsk_control_motor (false);
			return 0;
		}
	}
 
	flpydsk_control_motor (false);
	return -1;
}

//seek/park head: utilisé pour positionner la tete de lecture sur un cylindre
int flpydsk_seek(uint_32 cyl, uint_32 head ) {
 
	uint_32 st0, cyl0;
 
	if (_CurrentDrive >= 4)
		return -1;
 
	for (int i = 0; i < 10; i++ ) {
 
		//! send the command
		flpydsk_send_command (FDC_CMD_SEEK);
		flpydsk_send_command ( (head) << 2 | _CurrentDrive);
		flpydsk_send_command (cyl);
 
		//! wait for the results phase IRQ
		flpydsk_wait_irq ();
		flpydsk_check_int (&st0,&cyl0);
 
		//! found the cylinder?
		if ( cyl0 == cyl)
			return 0;
	}
 
	return -1;
}

//desactive le controller
void flpydsk_disable_controller () {
	outb(FLPYDSK_DOR, 0);
}

//active le controller
void flpydsk_enable_controller () {
	outb(FLPYDSK_DOR, FLPYDSK_DOR_MASK_RESET | FLPYDSK_DOR_MASK_DMA);
}

//initialise le controller
void flpydsk_reset () {
 
	uint_32 st0, cyl;
 
	//! reset the controller
	flpydsk_disable_controller ();
	flpydsk_enable_controller ();
	flpydsk_wait_irq ();
 
	//! send CHECK_INT/SENSE INTERRUPT command to all drives
	for (int i=0; i<4; i++)
		flpydsk_check_int (&st0,&cyl);
 
	//! transfer speed 500kb/s
	flpydsk_write_ccr (0);
 
	//! pass mechanical drive info. steprate=3ms, unload time=240ms, load time=16ms
	flpydsk_drive_data (3,16,240,true);
 
	//! calibrate the disk
	flpydsk_calibrate ( _CurrentDrive );
}


//READ A SECTOR
//! convert LBA to CHS
void flpydsk_lba_to_chs (int lba,int *head,int *track,int *sector) {

   *head = ( lba % ( FLPY_SECTORS_PER_TRACK * 2 ) ) / ( FLPY_SECTORS_PER_TRACK );
   *track = lba / ( FLPY_SECTORS_PER_TRACK * 2 );
   *sector = lba % FLPY_SECTORS_PER_TRACK + 1;
}

uint_8* flpydsk_read_sector (int sectorLBA) {

	if (_CurrentDrive >= 4)
		return 0;

	//! convert LBA sector to CHS
	int head=0, track=0, sector=1;
	flpydsk_lba_to_chs (sectorLBA, &head, &track, &sector);

	//! turn motor on and seek to track
	flpydsk_control_motor (true);
	if (flpydsk_seek (track, head) != 0)
		return 0;

	//! read sector and turn motor off
	flpydsk_read_sector_imp (head, track, sector);
	flpydsk_control_motor (false);

	//! warning: this is a bit hackish
	return (uint_8*) DMA_BUFFER;
}

void initFloppy(){
	flpydsk_initialize_dma() ;
	flpydsk_reset();
}