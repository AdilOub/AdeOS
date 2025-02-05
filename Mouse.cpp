#include "Header Files/Mouse.h"

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
        Print("\n\tError with PS2 controller config: timeout", BACKGROUND_RED);
        return 0xFF;
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
        Print("\n\tError with PS2 status write: timeout", BACKGROUND_RED);
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
        Print("\n\tError with PS2 status read: timeout", BACKGROUND_RED);
        return;
    }
}

void sendDataToDevice(uint8_t device, uint8_t command){
    uint64_t timeout = 1000;
    if(device==1){
    outb(COMMAND_PORT, 0xD4);
    }
    //wait_for_controller_ready();
    readControllerConfig();
    uint8_t response = inb(COMMAND_PORT);
    while(response  & 0x02 != 0 && timeout > 0){
        response = inb(COMMAND_PORT);
        timeout--;
    }
    if(timeout == 0){
        Print("\n\tError with PS2 send: timeout", BACKGROUND_RED);
        return;
    }
    outb(DATA_PORT, command);
    return;
}

uint8_t readDataFromDevice(){
    uint64_t timeout = 1000;
    readControllerConfig();
    uint8_t response = inb(COMMAND_PORT);
    while(response  & 0x01 != 1 && timeout > 0){
        response = inb(COMMAND_PORT);
        timeout--;
    }
    if(timeout == 0){
        Print("\n\tError with PS2 read: timeout", BACKGROUND_RED);
        asm("hlt");
        return 0xFF;
    }
    return inb(DATA_PORT);
}

uint8_t waitACKFromDevice(){
    uint64_t timeout = 1000;
    uint8_t response = inb(DATA_PORT);
    while(response != 0xFA && timeout > 0){
        response = inb(DATA_PORT);
        timeout--;
    }
    if(timeout == 0){
        Print("\n\tError with PS2 ACK: timeout", BACKGROUND_RED);
        return 0xFF;
    }
    return response;
}

uint8_t readMouseResponse(){
   //todo poll
    uint8_t response = 0;
    response = inb(DATA_PORT);
    return response;
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
        response = inb(DATA_PORT);
    }
    Print("Response of self test (should be 0x55): ");Print(HexToString(response));Print("\n\r");

    //7- determine if 2 chan test 2nd PS2 port, la souris
    outb(COMMAND_PORT, 0xA8); //enable 2nd PS2 port
    uint8_t res2 = readControllerConfig();
    Print("Response of 2nd PS2 (5th bit should be 0): ");Print(HexToString(res2));Print(" (");Print(IntToString(res2 & 0x20));Print(")\n\r");
    //if it's set, it means there is no mouse, and we're sad
    //sinon on disable la mouse:
    outb(COMMAND_PORT, 0xA7); //disable devices
    //clear bits 1 and 5 of the controller config
    config = readControllerConfig();
    config &= 0xDD;
    outb(COMMAND_PORT, 0x60); //write controller command
    outb(DATA_PORT, config);

    //interface test
    outb(COMMAND_PORT, 0xAB);
    response = inb(DATA_PORT);
    Print("Response of 1st interface test (should be 0): ");Print(HexToString(response));Print("\n\r");
    outb(COMMAND_PORT, 0xA9);
    response = inb(DATA_PORT);
    Print("Response of 2nd interface test (should be 0): ");Print(HexToString(response));Print("\n\r");

    //enable devices
    Print("enabling devices\n\r", BACKGROUND_GREEN);
    outb(COMMAND_PORT, 0xAE); //enable devices for first port
    Print("1st port enabled\n\r", BACKGROUND_GREEN);
    outb(COMMAND_PORT, 0xA8); //enable devices for second port
    Print("2nd port enabled\n\r", BACKGROUND_GREEN);

    //ack from keyboard

    //enable interrupts
    config = readControllerConfig();
    config |= 0x03; //enable IRQ, setting bit 0 and 1
    outb(COMMAND_PORT, 0x60); //write controller command
    outb(DATA_PORT, config);

    
    Print("Resetting devices\n\r", BACKGROUND_BLUE);
    //reset devices
    sendDataToDevice(0, 0xFF); //reset device 0
    sendDataToDevice(1, 0xFF); //reset device 1

    uint8_t rep1 = waitACKFromDevice();
    uint8_t rep2 = inb(DATA_PORT);
    Print("Response of reset devices 1 (should be 0xFA): ");Print(HexToString(rep1));Print("\n\r");
    Print("Response of reset devices 2 (should be 0xAA): ");Print(HexToString(rep2));Print("\n\r");
    uint8_t rep3 = inb(DATA_PORT); //clear buffer
    Print("Response of reset devices 3 (should be 0x00): ");Print(HexToString(rep3));Print("\n\r");
    //print config
    //Print("PS2 config: ");Print(HexToString(readControllerConfig()));Print("\n\r");
    
}

uint16_t getDeviceID(uint8_t device){
    sendDataToDevice(device, 0xF5); //disable scanning

    uint8_t response = readDataFromDevice();


    Print("\n\rSending identify command");
    sendDataToDevice(device, 0xF2); //identify
    Print("\n\rIdentifying PS2 device");
    response = readDataFromDevice();
    Print("\n\rResponse from ID: ");Print(HexToString(response));
    Print("OKKK", BACKGROUND_GREEN);

    uint8_t id1 = readDataFromDevice();
    uint8_t id2 = readDataFromDevice();
    Print(" ID1: ");Print(HexToString(id1));Print("\n\r");
    Print(" ID2: ");Print(HexToString(id2));Print("\n\r");

    sendDataToDevice(device, 0xF4); //enable scanning

    return (id2 << 8) | id1;
}

void initPS2Mouse(){
    initPS2();

    Print("FINISHED INITIALIZING PS2\n\r", BACKGROUND_GREEN);
   // uint16_t id = getDeviceID(1);
    //print ps2 config
    Print("PS2 config: ");Print(HexToString(readControllerConfig()));Print("\n\r");

    //enable mouse
    sendDataToDevice(0, 0xFF); //enable mouse
    Print("Mouse enabled\n\r", BACKGROUND_GREEN);


    return;
}

