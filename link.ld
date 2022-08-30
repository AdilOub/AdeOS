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
)

OUTPUT
(
    compiled/kernel.bin
)

SECTIONS
{
    . = 0x8000;

    .text : ALIGN(0x1000)
    {
        *(.text)
    }
    
    .idt BLOCK(0x1000) : ALIGN(0x1000)
    {
        _idt = .;
        . = . + 0x1000;
    }

    .data : ALIGN(0x1000){
       *(.data) 
    }

    .rodata : ALIGN(0x1000){
       *(.rodata) 
    }

    .bss : ALIGN(0x1000){
        *(COMMON)
        *(.bss) 
    }
}