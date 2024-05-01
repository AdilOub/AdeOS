OUTPUT_FORMAT(binary)

ENTRY (_start)

INPUT
(
    compiled/extended_program.o
    compiled/kernelC.o
    compiled/binaries.o
    compiled/IDT.o
    compiled/IO.o
    compiled/Keyboard.o
    compiled/Memory.o
    compiled/TextPrint.o
    compiled/Commands.o
    compiled/Heap.o
    compiled/ASCITable.o
    compiled/Render.o
    compiled/Math.o
    compiled/Timer.o
    compiled/FloppyDisk.o
)

OUTPUT
(
    compiled/kernel.bin
)


/*bon y'a un petit probleme avec les string ici 
le rodata n'est pas load correctement dans la mémoire
au lieu de ça text est load dans la mémoire
*/

KERNEL_VMA = 0x8000;
SECTIONS
{
    . = KERNEL_VMA;


    .text : AT(ADDR(.text) - KERNEL_VMA)
    {
        _code = .;
        *(.text)
        *(.rodata*)
        . = ALIGN(0x1000);
    }
    
    .idt BLOCK(0x1000) : ALIGN(0x1000)
    {
        _idt = .;
        . = . + 0x1000;
    }


    .data : AT(ADDR(.data) - KERNEL_VMA)
    {
        _data = .;
       *(.data) 
       . = ALIGN(0x1000);
    }

    .bss : ALIGN(0x1000){
        *(COMMON)
        *(.bss) 
    }
}