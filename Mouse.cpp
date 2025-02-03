#include "Header Files/Mouse.h"

/*TODO normalement: 
Toucher à du USB pour desactiver USB legacy support (pour l'instant flm)
Puis verifier qu'il y a un controleur PS2 avec ACPI (flm aussi atm)
*/

void sendMouseCommand(uint8_t command){
    outb(COMMAND_PORT, 0xD4);
    uint8_t response = inb(DATA_PORT);
    while(response  & 0x02 != 0){
        response = inb(DATA_PORT);
    }
    outb(DATA_PORT, command);
    return;
}

uint8_t readMouseResponse(){
   //todo poll
    uint8_t response = inb(DATA_PORT);
    return response;
}

void initPS2Mouse(){
    uint8_t status = 0x00;
    status = inb(COMMAND_PORT);
    Print("Status: ");
    Print(HexToString(status));
    Print("\n\r");

    //

    //outb(0x60, status & 0xAE); //disable interrupts & enable clock signal

    //test PS2 controller
    outb(COMMAND_PORT, 0xAA);
    uint8_t response = inb(DATA_PORT);
    if(response != 0x55){
        Print("Error with PS2\n\r");Print(HexToString(response));
        return;
    }
    
    Print("PS2 OK\n\r");Print(HexToString(response));

    //test 2nd PS2 port, la souris
    outb(COMMAND_PORT, 0xA9);
    response = inb(DATA_PORT);
    if(response != 0){
        Print("\n\rError with PS2 port 2\n\r");Print(HexToString(response));
        return;
    }
    Print("\n\rPS2 port 2 OK\n\r");Print(HexToString(response));

    //on va enable la souris, sending 0xA8 sur le 2eme port
    outb(COMMAND_PORT, 0xA8);



    //detecting type
    //send disable scan command 0xF5
    //wait for ack 0xFA
    //send identify command 0xF2
    //wait for ack 0xFA
    //read 2 bytes 
    //send enable scan command 0xF4

    outb(COMMAND_PORT, 0xF5);
    response = inb(DATA_PORT);
    if(response != 0xFA){
        Print("Error with PS2\n\r");Print(HexToString(response));
        return;
    }
    Print("PS2 disable scan OK\n\r");Print(HexToString(response));

    out

    return;
}