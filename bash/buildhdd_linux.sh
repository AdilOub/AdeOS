nasm -f bin Sector1/bootloader.asm -o compiled/bootloader.bin&&
nasm -f elf64 Sector2+/extended_program.asm -o compiled/extended_program.o&&
nasm -f elf64 binaries.asm -o compiled/binaries.o&&

gcc -Ttext 0x8000 -ffreestanding  -mcmodel=kernel -mno-red-zone -m64 -fno-pie -c "Kernel.cpp" -o "compiled/kernelC.o"&&
gcc -Ttext 0x8000 -ffreestanding  -mcmodel=kernel -mno-red-zone -m64 -fno-pie -c "IDT.cpp" -o "compiled/IDT.o"&&
gcc -Ttext 0x8000 -ffreestanding  -mcmodel=kernel -mno-red-zone -m64 -fno-pie -c "IO.cpp" -o "compiled/IO.o"&&
gcc -Ttext 0x8000 -ffreestanding  -mcmodel=kernel -mno-red-zone -m64 -fno-pie -c "Keyboard.cpp" -o "compiled/Keyboard.o"&&
gcc -Ttext 0x8000 -ffreestanding  -mcmodel=kernel -mno-red-zone -m64 -fno-pie -c "Memory.cpp" -o "compiled/Memory.o"&&
gcc -Ttext 0x8000 -ffreestanding  -mcmodel=kernel -mno-red-zone -m64 -fno-pie -c "TextPrint.cpp" -o "compiled/TextPrint.o"&&
gcc -Ttext 0x8000 -ffreestanding  -mcmodel=kernel -mno-red-zone -m64 -fno-pie -c "Commands.cpp" -o "compiled/Commands.o"&&
gcc -Ttext 0x8000 -ffreestanding  -mcmodel=kernel -mno-red-zone -m64 -fno-pie -c "Heap.cpp" -o "compiled/Heap.o"&&
gcc -Ttext 0x8000 -ffreestanding  -mcmodel=kernel -mno-red-zone -m64 -fno-pie -c "ASCITable.cpp" -o "compiled/ASCITable.o"&&
gcc -Ttext 0x8000 -ffreestanding  -mcmodel=kernel -mno-red-zone -m64 -fno-pie -c "Render.cpp" -o "compiled/Render.o"&&
gcc -Ttext 0x8000 -ffreestanding  -mcmodel=kernel -mno-red-zone -m64 -fno-pie -c "Math.cpp" -o "compiled/Math.o"&&
gcc -Ttext 0x8000 -ffreestanding  -mcmodel=kernel -mno-red-zone -m64 -fno-pie -c "Timer.cpp" -o "compiled/Timer.o"&&
gcc -Ttext 0x8000 -ffreestanding  -mcmodel=kernel -mno-red-zone -m64 -fno-pie -c "DiskRead.cpp" -o "compiled/DiskRead.o"&&
gcc -Ttext 0x8000 -ffreestanding  -mcmodel=kernel -mno-red-zone -m64 -fno-pie -c "FileSystem.cpp" -o "compiled/FileSystem.o"&&
ld -T"link.ld"&&
cat compiled/bootloader.bin compiled/kernel.bin > compiled/boot.bin&&
cp compiled/boot.bin ./diskimg/boot.bin&&
cd diskimg&&
#cp boot.bin boot2.bin&&
#python3 merge.py&&
truncate boot.bin --size 512K&& #s'assure que le fichier fait 64K
qemu-system-x86_64 boot.bin -m 2M -no-reboot -no-shutdown -d cpu_reset,int
