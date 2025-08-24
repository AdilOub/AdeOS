#include "../includes/cpu/io.h"

//8 bits
void outb(uint16_t port, uint8_t val){
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

uint8_t inb(uint16_t port){
    uint8_t returnVal;
    asm volatile ("inb %1, %0" : "=a"(returnVal) : "Nd"(port));
    return returnVal;
}


//16 bits
void outsw(uint16_t __port, void* __addr){
      asm volatile ("outsw":"=S" (__addr) :"d" (__port), "0" (__addr));
}

void insw (uint16_t __port, void *__addr)
{
  asm volatile ("insw":"=D" (__addr) :"d" (__port), "0" (__addr));
}

void outw(uint16_t port, uint16_t val){
    asm volatile ("outw %0, %1" : : "a"(val), "Nd"(port));
}

uint16_t inw(uint16_t port){
    uint16_t returnVal;
    asm volatile ("inw %1, %0" : "=a"(returnVal) : "Nd"(port));
    return returnVal;
}

//32 bits
void outl(uint16_t port, uint32_t val){
    asm volatile ("outl %0, %1" : : "a"(val), "Nd"(port));
}

uint32_t inl(uint16_t port){
    uint32_t returnVal;
    asm volatile ("inl %1, %0" : "=a"(returnVal) : "Nd"(port));
    return returnVal;
}

//unused
void PIC_sendEOI(uint8_t irq)
{
	if(irq >= 8){
		outb(PIC2_COMMAND, PIC_EOI);
  }
 
	outb(PIC1_COMMAND, PIC_EOI);
}

//"(It was an IBM design mistake.)" -> c'est pour ça que les interupts 0 à 7 sont déjà utilisé par les erreurs du cpu
void RemapPic(uint16_t offset1, uint16_t offset2){
  uint8_t a1, a2;

  a1 = inb(PIC1_DATA); //save masks
  a2 = inb(PIC2_DATA);

  outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
  outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
  outb(PIC1_DATA, offset1); //offset pic1
  outb(PIC2_DATA, offset2); //offset pic2
  outb(PIC1_DATA, 4);
  outb(PIC2_DATA, 2);
  outb(PIC1_DATA, ICW4_8086);
  outb(PIC2_DATA, ICW4_8086);

  outb(PIC1_DATA, a1); //restore saved masks.
  outb(PIC2_DATA, a2);
}