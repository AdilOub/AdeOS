#include "../includes/drivers/pci.h"


/*
Doc:
Registre  Offset  Description
0x00      0x00    31-16 Device ID
                  15-0  Vendor ID

0x01      0x04    31-16 Status
                  15-0 Command  

*/

uint16_t pciConfigReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t address;
    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint16_t tmp = 0;
  
    // Create configuration address as per Figure 1
    address = (uint32_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));
  
    // Write out the address
    outl(0xCF8, address);
    // Read in the data
    // (offset & 2) * 8) = 0 will choose the first word of the 32-bit register
    tmp = (uint16_t)((inl(0xCFC) >> ((offset & 2) * 8)) & 0xFFFF);
    return tmp;
}

void pciConfigWriteWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint16_t value) {
    uint32_t address;
    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint32_t tmp = 0;
  
    // Create configuration address as per Figure 1
    address = (uint32_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));
  
    // Write out the address
    outl(0xCF8, address);
    // Read in the data
    // (offset & 2) * 8) = 0 will choose the first word of the 32-bit register
    tmp = inl(0xCFC);
    if((offset & 2) == 0){
        tmp = (tmp & 0xFFFF0000) | value;
    }else{
        tmp = (tmp & 0x0000FFFF) | (value << 16);
    }
    outl(0xCFC, tmp);
    return;
}


uint16_t getVendorID(uint8_t bus, uint8_t slot, uint8_t func) {
    return pciConfigReadWord(bus, slot, func, 0x00);
}

// Device ID : 16 bits à l'offset 0x02
uint16_t getDeviceID(uint8_t bus, uint8_t slot, uint8_t func) {
    return pciConfigReadWord(bus, slot, func, 0x02);
}
uint8_t getHeaderType(uint8_t bus, uint8_t device, uint8_t function) {
    return (pciConfigReadWord(bus, device, function, 0x0C)) & 0xFF;
}

uint8_t getClassCode(uint8_t bus, uint8_t device, uint8_t function) {
    return (pciConfigReadWord(bus, device, function, 0x0A) >> 8) & 0xFF;
}

uint8_t getSubClassCode(uint8_t bus, uint8_t device, uint8_t function) {
    return (pciConfigReadWord(bus, device, function, 0x0A)) & 0xFF;
}

uint16_t getCommand(uint8_t bus, uint8_t device, uint8_t function) {
    return (pciConfigReadWord(bus, device, function, 0x04)) & 0xFFFF;
}

void setCommand(uint8_t bus, uint8_t device, uint8_t function, uint16_t command) {
    pciConfigWriteWord(bus, device, function, 0x04, command);
    return;
}


//For header Type 0x00:
uint16_t getBar0(uint8_t bus, uint8_t device, uint8_t function) {
    return (pciConfigReadWord(bus, device, function, 0x10));
}

uint8_t getInterruptLine(uint8_t bus, uint8_t device, uint8_t function) {
    return (pciConfigReadWord(bus, device, function, 0x3C)) & 0xFF;
}

uint8_t getInterruptPin(uint8_t bus, uint8_t device, uint8_t function) {
    return (pciConfigReadWord(bus, device, function, 0x3C) >> 8) & 0xFF;
}




void checkDevice(uint8_t bus, uint8_t device) {
    uint8_t function = 0;

    uint16_t vendorID = getVendorID(bus, device, function);

    if (vendorID == 0xFFFF) { printf("Device %d at bus %d doesn't exists\n", device, bus); return; };
    uint16_t deviceID = getDeviceID(bus, device, function);

    //checkFunction(bus, device, function);
    uint8_t classCode = getClassCode(bus, device, function);
    uint8_t subClass = getSubClassCode(bus, device, function);

    uint8_t headerType = getHeaderType(bus, device, function);
    //printf("PCI at bus%d dev%d: vendorID=%x, deviceID=%x, class=%x, subc=%x\n", bus, device, (vendorID), (deviceID), classCode, subClass);


    //  if( (headerType & 0x80) != 0) {
    //      // It's a multi-function device, so check remaining functions
    //      for (function = 1; function < 8; function++) {
    //          if (getVendorID(bus, device, function) != 0xFFFF) {
    //              //checkFunction(bus, device, function);
    //          }
    //      }
    //  }
 }



//TODO utiliser une fonction pour scanner tous les bus proprement, et les sauvegarder dans une structure en mémoire ou un truc du genre
void testPCI(){
        //PrintString("Testing PCI bus...\n\r", FOREGROUND_LIGHTCYAN);
        checkDevice(0, 0); //pci host bridge
        checkDevice(0, 1); //pci-to-isa bridge
        checkDevice(0, 2); //quemu graphic card
        checkDevice(0, 3); //intel e1000 ethernet card
        checkDevice(0, 4); //rtl8139 ethernet card
        checkDevice(0, 5);
        PrintString("Done testing PCI bus.\n\r", FOREGROUND_LIGHTCYAN);
        printf("ProgIf of device at bus0 dev4 func0: %x\n", getBar0(0, 4, 0));
 }


  void checkAllBuses(void) {
     uint16_t bus;
     uint8_t device;

     for (bus = 0; bus < 256; bus++) {
         for (device = 0; device < 32; device++) {
             checkDevice(bus, device);
         }
     }
}


