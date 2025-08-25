#pragma once

#include "../../TextPrint.h"
#include "../../libc/stdtypes.h"
#include "../../Memory.h"
#include "../pci.h"

#define RTL8139_VENDOR_ID 0x10EC
#define RTL8139_DEVICE_ID 0x8139


#define TX_BUFFER_SIZE 1514
#define TX_BUFFER_COUNT 4

#define RX_BUFFER_SIZE 9708

extern uint8_t rx_buffer[];


uint64_t rtl8139_setup();
void sendDummyPackage();

uint8_t rtl8139_transmit_packet(char* packet, uint16_t length);

uint16_t getRtlBus();
uint16_t getRtlDevice();

