#include "Header Files/Mouse.h"

/*TODO normalement: 
Toucher à du USB pour desactiver USB legacy support (pour l'instant flm)
Puis verifier qu'il y a un controleur PS2 avec ACPI (flm aussi atm)
*/

void sendDataToDevice(uint8_t device, uint8_t command){
    uint64_t timeout = 1000;
    if(device==1){
    outb(COMMAND_PORT, 0xD4);
    }
    uint8_t response = inb(COMMAND_PORT);
    while(response  & 0x02 != 0 && timeout > 0){
        response = inb(COMMAND_PORT);
        timeout--;
    }
    if(timeout == 0){
        Print("\n\tError with PS2 send: timeout", BACKGROUND_RED);
        asm("hlt");
        return;
    }
    outb(DATA_PORT, command);
    return;
}

uint8_t readDataFromDevice(){
    uint64_t timeout = 1000;
    uint8_t response = inb(COMMAND_PORT);
    while(response  & 0x01 != 0 && timeout > 0){
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

uint8_t readMouseResponse(){
   //todo poll
    uint8_t response = 0;
    response = inb(DATA_PORT);
    return response;
}

void sendCommandDevice1(uint8_t command){
    outb(DATA_PORT, command);
}

uint8_t readDataDevice1(){
    uint8_t response = 0;
    response = inb(DATA_PORT);
    return response;
}

uint8_t readControllerConfig(){
    outb(COMMAND_PORT, 0x20); //read controller command
    uint8_t status = inb(DATA_PORT);
    uint64_t timeout = 10000000;
    if(status & (~0x04) && timeout > 0){
        status = inb(DATA_PORT);
        timeout--;
    }
    if(timeout == 0){
        Print("\n\tError with PS2 controller: timeout", BACKGROUND_RED);
        return 0xFF;
    }
    return status;
}

void wait_for_controller_ready(){
    uint64_t timeout = 100000;
    uint8_t status = readControllerConfig();
    while(status & 0x02 && timeout > 0){
        status = readControllerConfig();
        timeout--;
    }
    if(timeout == 0){
        Print("\n\tError with PS2 controller: timeout", BACKGROUND_RED);
        return;
    }
}

void initPS2(){
    outb(COMMAND_PORT, 0xAD);
    outb(COMMAND_PORT, 0xA7); //disable devices

    //flush output buffer
    inb(DATA_PORT);

    //setting controller configuration, by reading status, clearing bit 0, 4 and 6  and writing back
    uint8_t status = readControllerConfig();
    Print("PS2 Status: ");Print(HexToString(status));Print("\n\r");
    status &= 0xAE; //clear bit 0 and 6 and 4, disable IRQ, translation and enable clock signal
    outb(COMMAND_PORT, 0x60); //write controller command
    wait_for_controller_ready();
    outb(DATA_PORT, status);

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
    status = readControllerConfig();
    status &= 0xDD;
    outb(COMMAND_PORT, 0x60); //write controller command
    wait_for_controller_ready();
    outb(DATA_PORT, status);

    //interface test
    outb(COMMAND_PORT, 0xAB);
    response = inb(DATA_PORT);
    Print("Response of 1st interface test (should be 0): ");Print(HexToString(response));Print("\n\r");
    outb(COMMAND_PORT, 0xA9);
    response = inb(DATA_PORT);
    Print("Response of 2nd interface test (should be 0): ");Print(HexToString(response));Print("\n\r");

    //enable devices
    outb(COMMAND_PORT, 0xAE); //enable devices for first port
    wait_for_controller_ready();
    outb(COMMAND_PORT, 0xA8); //enable devices for second port
    wait_for_controller_ready();

    //enable interrupts
    status = readControllerConfig();
    status |= 0x03; //enable IRQ, setting bit 0 and 1
    outb(COMMAND_PORT, 0x60); //write controller command
    wait_for_controller_ready();
    outb(DATA_PORT, status);


    //reset devices
    sendDataToDevice(0, 0xFF); //reset device 0
    response = inb(DATA_PORT);
    uint8_t dev0_id = inb(DATA_PORT);
    Print("Response of reset devices 1 (should be 0xAA): ");Print(HexToString(response));Print("\n\r");
    Print("Device 0 ID: ");Print(HexToString(dev0_id));Print("\n\r");
    return;

    sendDataToDevice(1, 0xFF); //reset device 1
    response = inb(DATA_PORT);
    uint8_t dev1_id = inb(DATA_PORT);
    Print("Response of reset devices 2 (should be 0xAA): ");Print(HexToString(response));Print("\n\r");
    Print("Device 1 ID: ");Print(HexToString(dev1_id));Print("\n\r");
}

uint16_t getDeviceID(uint8_t device){
    sendDataToDevice(device, 0xF5); //disable scanning
    uint8_t response = readDataFromDevice();
    uint64_t timeout = 10000000;
    while(response != 0xFA && timeout > 0){
        response = readDataFromDevice();
    }
    if(timeout == 0){
        Print("\n\tError with PS2 device on 0xF5: timeout", BACKGROUND_RED);
        return 0;
    }

    Print("\n\rSending identify command");
    sendDataToDevice(device, 0xF2); //identify
    Print("\n\rIdentifying PS2 device");
    response = readDataFromDevice();
    Print("\n\rResponse from ID: ");Print(HexToString(response));
    timeout = 200;
    while(response != 0xFA && timeout > 0){
        response = readDataFromDevice();
    }
    if(timeout == 0){
        Print("\n\tError with PS2 device on 0xF2: timeout", BACKGROUND_RED);
        return 0;
    }
    Print("OKKK", BACKGROUND_GREEN);

    uint8_t id1 = readMouseResponse();
    uint8_t id2 = readMouseResponse();
    Print(" ID1: ");Print(HexToString(id1));Print("\n\r");
    Print(" ID2: ");Print(HexToString(id2));Print("\n\r");

    sendDataToDevice(device, 0xF4); //enable scanning

    return (id2 << 8) | id1;
}

void initPS2Mouse(){
    initPS2();
    uint16_t id = getDeviceID(0);
    return;

    uint8_t status = 0x00;
    outb(COMMAND_PORT, 0x20); //read controller command
    status = inb(COMMAND_PORT);
    Print("Status: ");
    Print(HexToString(status));
    Print("\n\r");

    status &= 0xFE; //enable mouse irq
    //status &= (~0x20); //clear disable mouse clock
    outb(COMMAND_PORT, 0x60); //write controller command
    outb(DATA_PORT, status);
    uint8_t ack = inb(DATA_PORT);
    Print("Ack: ");
    Print(HexToString(ack));
    Print("\n\r");

    return;
    //

    //outb(0x60, status & 0xAE); //disable interrupts & enable clock signal

    //test PS2 controller
    outb(COMMAND_PORT, 0xAA);
    uint8_t response = inb(DATA_PORT);
    if(response != 0x55){
        Print("Error with PS2: ");Print(HexToString(response));
        return;
    }
    
    Print("PS2 OK: ");Print(HexToString(response));

    //test 2nd PS2 port, la souris
    outb(COMMAND_PORT, 0xA9);
    response = inb(DATA_PORT);
    if(response != 0){
        Print("\n\rError with PS2 port 2: ");Print(HexToString(response));
        return;
    }
    Print("\n\rPS2 port 2 OK: ");Print(HexToString(response));

    //on va enable la souris, sending 0xA8 sur le 2eme port
    //outb(COMMAND_PORT, 0xA8);



    //detecting type
    //send disable scan command 0xF5
    //wait for ack 0xFA
    //send identify command 0xF2
    //wait for ack 0xFA
    //read 2 bytes 
    //send enable scan command 0xF4

    sendCommandDevice1(0xF5); //disable scanning
    response = readDataDevice1();
    if(response != 0xFA){
        Print("\n\rError with PS2 mouse: ");Print(HexToString(response));
        return;
    }
    Print("\n\rPS2 mouse OK: ");Print(HexToString(response));

    // response = 0;
    Print("\n\rSending identify command");
    sendCommandDevice1(0xF2); //identify
    Print("\n\rIdentifying PS2 mouse");

    //reset all
    response = readDataDevice1();
    Print("\n\rResponse from ID: ");Print(HexToString(response));
    
    sendCommandDevice1(0xF4); //enable scanning
    
    outb(COMMAND_PORT, 0xFF);
    response = inb(DATA_PORT);
    Print("\n\rReset Response: ");Print(HexToString(response));
    // response = readMouseResponse();
    // if(response != 0xFA){
    //     Print("\n\rError with PS2 mouse: ");Print(HexToString(response));
    //     return;
    // }
    // Print("\n\rPS2 mouse IDed: ");Print(HexToString(response));
    // uint8_t id1 = readMouseResponse();
    // uint8_t id2 = readMouseResponse();
    // Print(" ID1: ");Print(HexToString(id1));Print("\n\r");
    // Print(" ID2: ");Print(HexToString(id2));Print("\n\r");
    // sendMouseCommand(0xF4); //enable scanning

    
    return;
}



void initPS2Mouse_(){
    outb(COMMAND_PORT, 0x20); //get status
    uint8_t status = inb(DATA_PORT);
    Print("Status: ");Print(HexToString(status));Print("\n\r");
    uint8_t new_status = inb(DATA_PORT);
    uint64_t timeout = 10000000;
    while((new_status & 0x4) && (timeout--) > 0){
        new_status = inb(DATA_PORT);
    }
    Print("New status: ");Print(HexToString(new_status));Print("\n\r");

    status |= 0x2; //enable mouse irq
    status &= (~0x20); //clear disable mouse clock
    outb(COMMAND_PORT, 0x60); //set status
    outb(DATA_PORT, status);
    
    //enable auxilary device & ack
    outb(COMMAND_PORT, 0xA8);
    uint8_t response = inb(DATA_PORT);
    Print("Response: ");Print(HexToString(response));Print("\n\r");


}