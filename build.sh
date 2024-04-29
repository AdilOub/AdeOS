nasm -f bin Sector1/bootloader.asm -o compiled/bootloader.bin&&
nasm -f elf64 Sector2+/extended_program.asm -o compiled/extended_program.o&&
nasm -f elf64 binaries.asm -o compiled/binaries.o&&
x86_64-elf-gcc -Ttext 0x8000 -ffreestanding  -mno-red-zone -m64 -c "Kernel.cpp" -o "compiled/kernelC.o"&&
x86_64-elf-gcc -Ttext 0x8000 -ffreestanding  -mno-red-zone -m64 -c "IDT.cpp" -o "compiled/IDT.o"&&
x86_64-elf-gcc -Ttext 0x8000 -ffreestanding  -mno-red-zone -m64 -c "IO.cpp" -o "compiled/IO.o"&&
x86_64-elf-gcc -Ttext 0x8000 -ffreestanding  -mno-red-zone -m64 -c "Keyboard.cpp" -o "compiled/Keyboard.o"&&
x86_64-elf-gcc -Ttext 0x8000 -ffreestanding  -mno-red-zone -m64 -c "Memory.cpp" -o "compiled/Memory.o"&&
x86_64-elf-gcc -Ttext 0x8000 -ffreestanding  -mno-red-zone -m64 -c "TextPrint.cpp" -o "compiled/TextPrint.o"&&
x86_64-elf-gcc -Ttext 0x8000 -ffreestanding  -mno-red-zone -m64 -c "Commands.cpp" -o "compiled/Commands.o"&&
x86_64-elf-gcc -Ttext 0x8000 -ffreestanding  -mno-red-zone -m64 -c "Heap.cpp" -o "compiled/Heap.o"&&
x86_64-elf-gcc -Ttext 0x8000 -ffreestanding  -mno-red-zone -m64 -c "ASCITable.cpp" -o "compiled/ASCITable.o"&&
x86_64-elf-gcc -Ttext 0x8000 -ffreestanding  -mno-red-zone -m64 -c "Render.cpp" -o "compiled/Render.o"&&
x86_64-elf-gcc -Ttext 0x8000 -ffreestanding  -mno-red-zone -m64 -c "Math.cpp" -o "compiled/Math.o"&&
x86_64-elf-gcc -Ttext 0x8000 -ffreestanding  -mno-red-zone -m64 -c "Timer.cpp" -o "compiled/Timer.o"&&
x86_64-elf-gcc -Ttext 0x8000 -ffreestanding  -mno-red-zone -m64 -c "FloppyDisk.cpp" -o "compiled/FloppyDisk.o"&&
x86_64-elf-ld -T"link.ld"&&
cat compiled/bootloader.bin compiled/kernel.bin > compiled/boot.bin&&
qemu-system-x86_64 compiled/boot.bin -m 1G
