#include "../../includes/drivers/network/rtl8139.h"



const uint32_t RTL8139_BUS = _RTL8139_BUS;
const uint32_t RTL8139_DEVICE = _RTL8139_DEVICE;

static uint8_t tx_buffers[TX_BUFFER_COUNT][TX_BUFFER_SIZE] __attribute__((aligned(4)));
static uint8_t tx_cur = 0;

uint8_t rx_buffer[9708] __attribute__((aligned(4)));

uint64_t rtl8139_setup(){
    uint16_t bar0 = getBar0(RTL8139_BUS, RTL8139_DEVICE, 0) & 0xFFFFFFFC; //masking the last 2 bits (to get the address)
    uint8_t interruptLine = getInterruptLine(RTL8139_BUS, RTL8139_DEVICE, 0);
    uint8_t interruptPin = getInterruptPin(RTL8139_BUS, RTL8139_DEVICE, 0);

    //printf("Testing rtl8139...\n");
    //printf("RTL8139 BAR0=%x, interruptLine=%x, interruptPin=%x\n", bar0, interruptLine, interruptPin);
    
    //Enabling PCI Bus Mastering
    uint16_t command = getCommand(RTL8139_BUS, RTL8139_DEVICE, 0);
    command |= 0x4; //set bit 2
    setCommand(RTL8139_BUS, RTL8139_DEVICE, 0, command);
    command = getCommand(RTL8139_BUS, RTL8139_DEVICE, 0);
    if(! (command & 0x4))
        printf("Failed to enable bus mastering\n");
        
    //printf("Bus mastering enabled\n");
    

    //Turning on the RTL8139
    outb(bar0 + 0x52, 0x0); //set the command register to 0x0 to turn it on
    //printf("RTL8139 turned on\n");

    //Software reset
    outb(bar0 + 0x37, 0x10); //set the reset bit
    while( (inb(bar0 + 0x37) & 0x10) != 0); //wait for the reset bit to be cleared
    //printf("RTL8139 reset done\n");


    //Init Recive Buffer
    outl(bar0 + 0x30, (uint32_t)( (uint64_t)rx_buffer & 0xFFFFFFFF)); //set the recive buffer address
    //printf("Allocated & init read buffer (at %x)\n", (uint64_t)rx_buffer);

    //Set IMR+ISR
    outw(bar0 + 0x3C, 0x0005);

    //Configuring receive buffer (RCR)
    outl(bar0 + 0x44, 0xf | (1<<7)); //set RCR to accept all packets
    //printf("Set RCR to: ALL_PACKETS\n");

    //Enable Recieve & Transmit
    outb(bar0 + 0x37, 0x0C); 

    //Set TX config (to write)
    for(int i = 0; i < TX_BUFFER_COUNT; i++){
        outl(bar0 + 0x20 + i*4, (uint32_t)((uint64_t) (tx_buffers[i]) & 0xFFFFFFFF)); 
    }
    
    printf("RTL8139 setup done\n");
    return (uint64_t)rx_buffer;
}


// void sendDummyPackage(){
//     uint32_t bar0 = getBar0(RTL8139_BUS, RTL8139_DEVICE, 0) & 0xFFFFFFFC; //masking the last 2 bits (to get the address)
//     uint8_t mac[6];
//     for(int i = 0; i < 6; i++){
//         mac[i] = inb(bar0 + i);
//     }

//     printf("Testing broadcast...\n");
//     memset(tx_buffer, 0x12, 1500); //filling the buffer with 0xFF (broadcast)

//     //Destination mac FF:FF:FF:FF:FF:FF (broadcast)
//     for(int i = 0; i < 6; i++){
//         tx_buffer[i] = 0xFF;
//     }

//     //Source mac (the one of the card)
//     for(int i = 0; i < 6; i++){
//         tx_buffer[i + 6] = mac[i];
//     }
//     //Ethernet type (0x0800 for IP)
//     tx_buffer[12] = 0x08;
//     tx_buffer[13] = 0x00;

//     //fun stuff:
//     for(int i = 14; i < 60; i++){
//         tx_buffer[i] = i;
//     }

//     //on envoie le paquet
//     outl(bar0+ 0x20, (uint32_t)( (uint64_t)tx_buffer & 0xFFFFFFFF)); //set the transmit buffer address
//     outl(bar0 + 0x10, 60); //set the length of the packet

//     printf("Packet sent\n");
//     return;
// }


//todo attendre que le buffer soit libre avant d'envoyer
uint8_t rtl8139_transmit_packet(char* packet, uint16_t length){
    uint16_t bar0 = getBar0(RTL8139_BUS, RTL8139_DEVICE, 0) & 0xFFFFFFFC; //masking the last 2 bits (to get the address)

    // uint16_t status0 = inw(bar0+0x10);
    // uint16_t status1 = inw(bar0+0x14);
    // printf("RTL8139 TX0=%x, TX1=%x\n", status0, status1);

    // if(status0 & 0x8000){ //bit 15 is transmit ok
    //     printf("RTL8139 TX0 is not busy\n");
    // }

    memcopy(tx_buffers[tx_cur], packet, length);
    outl(bar0 + 0x10 + tx_cur*4, length & 0x0000FFFF);
    tx_cur = (tx_cur + 1) % TX_BUFFER_COUNT;

    return 0;
}

uint16_t getRtlBus(){
    return RTL8139_BUS;
}
uint16_t getRtlDevice(){
    return RTL8139_DEVICE;
}
