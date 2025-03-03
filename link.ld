OUTPUT_FORMAT(binary)

ENTRY (_start)

INPUT
(
    compiled/extended_program.o
    compiled/kernelC.o
    compiled/binaries.o
    compiled/idt.o
    compiled/io.o
    compiled/Keyboard.o
    compiled/Memory.o
    compiled/TextPrint.o
    compiled/Commands.o
    compiled/Heap.o
    compiled/ASCITable.o
    compiled/Render.o
    compiled/Math.o
    compiled/Timer.o
    compiled/DiskRead.o
    compiled/FileSystem.o
    compiled/Compiler.o
    compiled/Mouse.o
    compiled/shell.o
    compiled/string.o
)

OUTPUT
(
    compiled/kernel.bin
)


/*bon y'a un petit probleme avec les string ici 
le rodata n'est pas load correctement dans la mémoire
au lieu de ça text est load dans la mémoire

EDIT: j'ai mis le rodata au dessus du data et c'est inversé:
le rodata est load correctement dans la mémoire mais pas le data

EDIT2: post résolution: le pb était dans la façon de load boot.bin en mémoire dans disk_read.asm
*/

/*KERNEL_VMA = 0x8000;*/
/*KERNEL_VMA = 0xFFFFFFFF80000000;*/
SECTIONS
{
    . = 0x8000;
    
    .text :ALIGN(0x1000)
    {
        *(.text)
        /**(.rodata*)*/
    }
    


    .rodata : ALIGN(0x1000){
        *(.rodata) /**(.rodata*)*/
        
    }

    
    .data : ALIGN(0x1000){
       *(.data) /**(.data*)*/
    }


    .bss : ALIGN(0x1000){
        *(COMMON)
        *(.bss) 
    }

    .idt BLOCK(0x1000) : ALIGN(0x1000)
    {
        _idt = .;
        . = . + 0x1000;
    }
}