#include "../includes/Mouse.h"

/*TODO normalement: 
Toucher à du USB pour desactiver USB legacy support (pour l'instant flm)
Puis verifier qu'il y a un controleur PS2 avec ACPI (flm aussi atm)
*/

uint8_t readControllerConfig(){
    outb(COMMAND_PORT, 0x20); //read controller command
    uint8_t config = inb(DATA_PORT);
    uint64_t timeout = 1000;
    if(config & (~0x04) && timeout > 0){
        config = inb(DATA_PORT);
        timeout--;
    }
    if(timeout == 0){
        Print("\n\rError with PS2 controller config: timeout", BACKGROUND_RED);
        return 0x69;
    }
    return config;
}

/*void wait_for_controller_ready(){
    uint64_t timeout = 1;
    uint8_t status = readControllerConfig();
    while(status & 0x02 && timeout > 0){
        status = readControllerConfig();
        timeout--;
    }
    if(timeout == 0){
        Print("\n\tError with PS2 controller: timeout", BACKGROUND_RED);
        return;
    }
}*/

//poll pour attendre quand on commnique avec les devices
void wait_status_write_ok(){
    uint64_t timeout = 1000;
    uint8_t status = inb(COMMAND_PORT);
    while(status & 0x02 && timeout > 0){
        status = inb(COMMAND_PORT);
        timeout--;
    }
    if(timeout == 0){
        Print("\n\rError with PS2 status write: timeout", BACKGROUND_RED);
        return;
    }
}

void wait_status_read_ok(){
    uint64_t timeout = 1000;
    uint8_t status = inb(COMMAND_PORT);
    while(status & 0x01 && timeout > 0){
        status = inb(COMMAND_PORT);
        timeout--;
    }
    if(timeout == 0){
        Print("\n\rError with PS2 status read: timeout", BACKGROUND_RED);
        return;
    }
}

void sendDataToDevice(uint8_t port, uint8_t command){
    uint64_t timeout = 1000;
    if(port==1){
    wait_status_write_ok();
    outb(COMMAND_PORT, 0xD4);
    }
    wait_status_read_ok();
    //wait_for_controller_ready();
    uint8_t response = inb(COMMAND_PORT);
    while(response  & 0x02 != 0 && timeout > 0){
        wait_status_read_ok();
        response = inb(COMMAND_PORT);
        timeout--;
    }
    if(timeout == 0){
        Print("\n\rError with PS2 send: timeout", BACKGROUND_RED);
        return;
    }
    wait_status_write_ok();
    outb(DATA_PORT, command);
    return;
}

uint8_t readDataFromDevice(){
    uint64_t timeout = 10000;
    uint8_t response = inb(COMMAND_PORT);
    while(response  & 0x01 != 1 && timeout > 0){
        response = inb(COMMAND_PORT);
        timeout--;
    }
    if(timeout == 0){
        Print("\n\rError with PS2 read: timeout", BACKGROUND_RED);
        return 0x69;
    }
    return inb(DATA_PORT);
}

uint8_t waitACKFromDevice(){
    uint64_t timeout = 1000000;
    uint8_t response = readDataFromDevice();
    while(response != 0xFA && timeout > 0){
        response = readDataFromDevice();
        timeout--;
    }
    if(timeout == 0){
        Print("\n\rError with PS2 device_ACK: timeout\n\r", BACKGROUND_RED);
        return 0x69;
    }
    return response;
}

uint8_t readMouseResponse(){
   //todo poll
    uint8_t response = 0;
    response = inb(DATA_PORT);
    return response;
}


void sendResetToDevice(uint8_t port){
    sendDataToDevice(port, 0xFF);

    uint8_t rep1 = readDataFromDevice();
    uint8_t rep2 = readDataFromDevice();
    while(rep2==rep1){
        rep2 = readDataFromDevice();
    }
    uint8_t id = readDataFromDevice();

    //Print("Response of reset devices num: ");Print(IntToString(port));Print(" should be 0xFA 0xAA 0x00): ");Print(HexToString(rep1));
    //Print(" ");Print(HexToString(rep2));
    //Print(" ");Print(HexToString(id));Print("\n\r");

    
}


void initPS2(){
    //3-disable devices
    outb(COMMAND_PORT, 0xAD);
    outb(COMMAND_PORT, 0xA7); 

    //flush output buffer
    inb(DATA_PORT);

    //setting controller configuration, by reading status, clearing bit 0, 4 and 6  and writing back
     uint8_t config = readControllerConfig();
    Print("PS2 config: ");Print(HexToString(config));Print("\n\r");
    // config &= 0xAE; //clear bit 0 and 6 and 4, disable IRQ, translation and enable clock signal
    // outb(COMMAND_PORT, 0x60); //write controller command
    // outb(DATA_PORT, config);

    //self test
    outb(COMMAND_PORT, 0xAA);
    uint8_t response = inb(DATA_PORT);
    while(response != 0x55 && response != 0xFC){
        Print("Self test failed, retrying\n\r", BACKGROUND_RED);
        response = inb(DATA_PORT);
    }
    Print("Response of self test (should be 0x55): ");Print(HexToString(response));Print("\n\r");

    //7- determine if 2 chan test 2nd PS2 port, la souris
    outb(COMMAND_PORT, 0xA8); //enable 2nd PS2 port
    uint8_t res2 = readControllerConfig();
    Print("Response of 2nd PS2 (5th bit should be 0): ");Print(HexToString(res2));Print(" (");Print(IntToString(res2 & 0x20));Print(")\n\r");
    //if it's set, it means there is no mouse, and we're sad
    //sinon on disable la mouse:
    outb(COMMAND_PORT, 0xA7); //disable 2nd PS2 port
    //clear bits 1 and 5 of the controller config
    config = readControllerConfig();
    config &= 0xDD; //clear bit 1 and 5
    outb(COMMAND_PORT, 0x60); //write controller command
    outb(DATA_PORT, config);

    //interface test
    outb(COMMAND_PORT, 0xAB);
    response = inb(DATA_PORT);
    Print("Response of interface test 1: (should be 0): ");Print(HexToString(response));
    outb(COMMAND_PORT, 0xA9);
    response = 0xFF;
    response = inb(DATA_PORT);
    Print(" and  2 (should be 0): ");Print(HexToString(response));Print("\n\r");

    

    //ack from keyboard

    //enable interrupts
    config = readControllerConfig();
    config |= 0x03; //enable IRQ, setting bit 0 and 1
    config &= ~0x20; //clear bit 5= disable mouse signal
    outb(COMMAND_PORT, 0x60); //set compaq status
    outb(DATA_PORT, config); //status byte

    //enable devices
    //Print("enabling devices\n\r", BACKGROUND_GREEN);
    outb(COMMAND_PORT, 0xAE); //enable devices for first port
    //waitACKFromDevice(); d'après os dev y'a pas de ACK
    outb(COMMAND_PORT, 0xA8); //enable devices for second port
    //waitACKFromDevice();
    Print("1 & 2nd port enabled\n\r", BACKGROUND_GREEN);


    
    Print("Resetting 1st devices\n\r", BACKGROUND_BLUE);
    //reset devices
    //sendDataToDevice(0, 0xFF); //reset device 0
    //sendDataToDevice(1, 0xFF); //reset device 1

    sendResetToDevice(0);
    Print("reseting 2nd device\n\r", BACKGROUND_BLUE);
    sendResetToDevice(1);

    //print config
    //Print("PS2 config: ");Print(HexToString(readControllerConfig()));Print("\n\r");
    
}

void disableTranlation(){
    uint8_t config = readControllerConfig();
    config &= 0xBF; //clear bit 6
    outb(COMMAND_PORT, 0x60); //write controller command
    outb(DATA_PORT, config);
}

void enableTranslation(){
    uint8_t config = readControllerConfig();
    config |= 0x40; //set bit 6
    outb(COMMAND_PORT, 0x60); //write controller command
    outb(DATA_PORT, config);
}

uint16_t getDeviceID(uint8_t port){
    if(port==0)
        disableTranlation();

    //sendDataToDevice(port, 0xF5); //disable scanning

    //uint8_t response = readDataFromDevice();

    //Print("\n\rSending identify command");
    sendDataToDevice(port, 0xF2); //identify
    //Print("\n\rIdentifying PS2 port");
    uint8_t response = readDataFromDevice();
    //Print("\n\rResponse from ID: ");Print(HexToString(response));Print("\n\r");
    if(response != 0xFA){
        Print("Error with PS2 device ID: response not 0xFA\n\r", BACKGROUND_RED);
        return 0x69;
    }

    uint8_t id1 = readDataFromDevice();
    uint8_t id2 = readDataFromDevice();
    //Print("\n\rID1: ");Print(HexToString(id1));
    //Print(" ID2: ");Print(HexToString(id2));Print("\n\r");

    if(port==0)
        enableTranslation();
    
    //sendDataToDevice(port, 0xF4); //enable scanning

    return (id2<<8) | id1;
}



void tellDeviceToSendPacket(uint8_t port){
    sendDataToDevice(port, 0xEB); //tell device to send packet
    uint8_t response = readDataFromDevice();
    Print("Response from packet request 1,2 3 (0xFA, 0xAA, 0X00): ");Print(HexToString(response));
    response = readDataFromDevice();
    Print("  ");Print(HexToString(response));
    response = readDataFromDevice();
    Print(" ");Print(HexToString(response));Print("\n\r");

}

void enableMouse(){
    sendDataToDevice(1, 0xF4);
}

void disableMouse(){
    sendDataToDevice(1, 0xF5);
}

void initPS2Mouse(){
    initPS2();

    Print("FINISHED INITIALIZING PS2\n\r", BACKGROUND_GREEN);
   // uint16_t id = getDeviceID(1);
    //print ps2 config


    Print("PS2 new conf: ");Print(HexToString(readControllerConfig()));Print("\n\r");

    //print device 1 id
    uint16_t id = getDeviceID(1);
    Print("Device ID: ");Print(HexToString(id));Print("\n\r");

    //allumer led du clavier (device 0)
    sendDataToDevice(0, 0xED);
    sendDataToDevice(0, 0x04); //allumer led 2
    uint8_t response = readDataFromDevice();
    Print("Response from led command (0xFA): ");Print(HexToString(response));Print("\n\r"); //work on vbox, not qemu

    //set sample rate to 40
    sendDataToDevice(1, 0xF3);
    sendDataToDevice(1, 20);
    response = readDataFromDevice();

    if(response != 0xFA){
        Print("Error with PS2 mouse sample rate: response not 0xFA\n\r", BACKGROUND_RED);
        return;
    }

    sendDataToDevice(1, 0xF4);
    return;
}

