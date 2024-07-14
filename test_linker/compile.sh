x86_64-elf-gcc -Ttext 0x100 -ffreestanding  -mno-red-zone -m64 -c func.c -o func.o&&
x86_64-elf-ld -T"link.ld"