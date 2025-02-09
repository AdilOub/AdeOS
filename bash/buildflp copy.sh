nasm -f bin Sector1/bootloader.asm -o compiled/bootloader.bin&&
nasm -f elf64 Sector2+/extended_program.asm -o compiled/extended_program.o&&
nasm -f elf64 binaries.asm -o compiled/binaries.o&&
x86_64-elf-gcc  -march=x86-64 -fno-pie -ffreestanding -nostdlib -nostartfiles -Ttext=0x8000 -mno-red-zone -c "Kernel.cpp" -o "compiled/kernelC.o"&&
x86_64-elf-gcc  -march=x86-64 -fno-pie -ffreestanding -nostdlib -nostartfiles -Ttext=0x8000 -mno-red-zone -c "IDT.cpp" -o "compiled/IDT.o"&&
x86_64-elf-gcc  -march=x86-64 -fno-pie -ffreestanding -nostdlib -nostartfiles -Ttext=0x8000 -mno-red-zone -c "IO.cpp" -o "compiled/IO.o"&&
x86_64-elf-gcc  -march=x86-64 -fno-pie -ffreestanding -nostdlib -nostartfiles -Ttext=0x8000 -mno-red-zone -c "Keyboard.cpp" -o "compiled/Keyboard.o"&&
x86_64-elf-gcc  -march=x86-64 -fno-pie -ffreestanding -nostdlib -nostartfiles -Ttext=0x8000 -mno-red-zone -c "Memory.cpp" -o "compiled/Memory.o"&&
x86_64-elf-gcc  -march=x86-64 -fno-pie -ffreestanding -nostdlib -nostartfiles -Ttext=0x8000 -mno-red-zone -c "TextPrint.cpp" -o "compiled/TextPrint.o"&&
x86_64-elf-gcc  -march=x86-64 -fno-pie -ffreestanding -nostdlib -nostartfiles -Ttext=0x8000 -mno-red-zone -c "Commands.cpp" -o "compiled/Commands.o"&&
x86_64-elf-gcc  -march=x86-64 -fno-pie -ffreestanding -nostdlib -nostartfiles -Ttext=0x8000 -mno-red-zone -c "Heap.cpp" -o "compiled/Heap.o"&&
x86_64-elf-gcc  -march=x86-64 -fno-pie -ffreestanding -nostdlib -nostartfiles -Ttext=0x8000 -mno-red-zone -c "ASCITable.cpp" -o "compiled/ASCITable.o"&&
x86_64-elf-gcc  -march=x86-64 -fno-pie -ffreestanding -nostdlib -nostartfiles -Ttext=0x8000 -mno-red-zone -c "Render.cpp" -o "compiled/Render.o"&&
x86_64-elf-gcc  -march=x86-64 -fno-pie -ffreestanding -nostdlib -nostartfiles -Ttext=0x8000 -mno-red-zone -c "Math.cpp" -o "compiled/Math.o"&&
x86_64-elf-gcc  -march=x86-64 -fno-pie -ffreestanding -nostdlib -nostartfiles -Ttext=0x8000 -mno-red-zone -c "Timer.cpp" -o "compiled/Timer.o"&&
x86_64-elf-gcc  -march=x86-64 -fno-pie -ffreestanding -nostdlib -nostartfiles -Ttext=0x8000 -mno-red-zone -c "FloppyDisk.cpp" -o "compiled/FloppyDisk.o"&&
x86_64-elf-ld -T"link.ld" &&
cat compiled/bootloader.bin compiled/kernel.bin > compiled/boot.bin&&
cp compiled/boot.bin ./floppyimg/boot.bin&&
cd floppyimg&&
dd if=boot.bin of=floppy.flp bs=512 count=2880&&
qemu-system-x86_64 -fda floppy.flp -m 32M 


#-boot a -d cpu_reset  
