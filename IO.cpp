#include "Header Files/IO.h"

void outb(uint_16 port, uint_8 val){
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

uint_8 inb(uint_16 port){
    uint_8 returnVal;
    asm volatile ("inb %1, %0" : "=a"(returnVal) : "Nd"(port));
    return returnVal;
}

//unused
void PIC_sendEOI(uint_8 irq)
{
	if(irq >= 8){
		outb(PIC2_COMMAND, PIC_EOI);
  }
 
	outb(PIC1_COMMAND, PIC_EOI);
}

//"(It was an IBM design mistake.)" -> c'est pour ça que les interupts 0 à 7 sont déjà utilisé par les erreurs du cpu
void RemapPic(uint_16 offset1, uint_16 offset2){
  uint_8 a1, a2;

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