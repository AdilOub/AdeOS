#include "../../includes/drivers/network/rtl8139.h"

const uint32_t RTL8139_BUS = _RTL8139_BUS;
const uint32_t RTL8139_DEVICE = _RTL8139_DEVICE;


char* rtl8139_setup(){
    uint32_t bar0 = getBar0(RTL8139_BUS, RTL8139_DEVICE, 0) & 0xFFFFFFFC; //masking the last 2 bits (to get the address)
    uint8_t interruptLine = getInterruptLine(RTL8139_BUS, RTL8139_DEVICE, 0);
    uint8_t interruptPin = getInterruptPin(RTL8139_BUS, RTL8139_DEVICE, 0);

    printf("Testing rtl8139...\n");
    printf("RTL8139 BAR0=%x, interruptLine=%x, interruptPin=%x\n", bar0, interruptLine, interruptPin);
    
    //Enabling PCI Bus Mastering
    uint16_t command = getCommand(RTL8139_BUS, RTL8139_DEVICE, 0);
    command |= 0x4; //set bit 2
    setCommand(RTL8139_BUS, RTL8139_DEVICE, 0, command);
    command = getCommand(RTL8139_BUS, RTL8139_DEVICE, 0);
    if(! (command & 0x4))
        printf("Failed to enable bus mastering\n");
        
    printf("Bus mastering enabled\n");
    

    //Turning on the RTL8139
    outb(bar0 + 0x52, 0x0); //set the command register to 0x0 to turn it on
    printf("RTL8139 turned on\n");

    //Software reset
    outb(bar0 + 0x37, 0x10); //set the reset bit
    while( (inb(bar0 + 0x37) & 0x10) != 0); //wait for the reset bit to be cleared
    printf("RTL8139 reset done\n");


    //Init Recive Buffer
    char* rbuffer = (char*)malloc(9708); //warning: must be a 32bit address at least 8K + 16 + 1500 bytes
    outl(bar0 + 0x30, (uint32_t)( (uint64_t)rbuffer & 0xFFFFFFFF)); //set the recive buffer address
    printf("Allocated & init read buffer (at %x)\n", (uint64_t)rbuffer);

    //Set IMR+ISR
    outw(bar0 + 0x3C, 0x0005); //clear all interrupt status bits

    //Configuring receive buffer (RCR)
    outl(bar0 + 0x44, 0xf | (1<<7)); //set RCR to accept all packets
    printf("Set RCR to: ALL_PACKETS\n");

    //Enable Recieve & Transmit
    outb(bar0 + 0x37, 0x0C); 


    return rbuffer;
}

void printMACAddress(){
    uint32_t bar0 = getBar0(RTL8139_BUS, RTL8139_DEVICE, 0) & 0xFFFFFFFC; //masking the last 2 bits (to get the address)
    printf("MAC Address: ");
    for(int i = 0; i < 6; i++){
        uint8_t mac = inb(bar0 + i);
        printf("%x:", mac);
    }

    printf("\n");
}