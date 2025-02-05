bootloader: Sector1/bootloader.asm Sector2+/extended_program.asm binaries.asm
	nasm -f bin Sector1/bootloader.asm -o compiled/bootloader.bin
	nasm -f elf64 Sector2+/extended_program.asm -o compiled/extended_program.o
	nasm -f elf64 binaries.asm -o compiled/binaries.o

kernel: Kernel.cpp IDT.cpp IO.cpp Keyboard.cpp Memory.cpp TextPrint.cpp Commands.cpp Heap.cpp ASCITable.cpp Render.cpp Math.cpp Timer.cpp Timer.cpp DiskRead.cpp FileSystem.cpp Mouse.cpp
	gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -fno-pie -c Kernel.cpp -o compiled/kernelC.o
	gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -fno-pie -c IDT.cpp -o compiled/IDT.o
	gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -fno-pie -c IO.cpp -o compiled/IO.o
	gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -fno-pie -c Keyboard.cpp -o compiled/Keyboard.o
	gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -fno-pie -c Memory.cpp -o compiled/Memory.o
	gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -fno-pie -c TextPrint.cpp -o compiled/TextPrint.o
	gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -fno-pie -c Commands.cpp -o compiled/Commands.o
	gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -fno-pie -c Heap.cpp -o compiled/Heap.o
	gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -fno-pie -c ASCITable.cpp -o compiled/ASCITable.o
	gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -fno-pie -c Render.cpp -o compiled/Render.o
	gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -fno-pie -c Math.cpp -o compiled/Math.o
	gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -fno-pie -c Timer.cpp -o compiled/Timer.o
	gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -fno-pie -c DiskRead.cpp -o compiled/DiskRead.o
	gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -fno-pie -c FileSystem.cpp -o compiled/FileSystem.o
	gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -fno-pie -c Compiler.cpp -o compiled/Compiler.o
	gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -fno-pie -c Mouse.cpp -o compiled/Mouse.o

link: 
	ld -T"link.ld"

new_disk: bootloader kernel link
	cat compiled/bootloader.bin compiled/kernel.bin > compiled/boot.bin
	cp compiled/boot.bin ./diskimg/boot.bin
	truncate diskimg/boot.bin --size 1M #pour s'assurer que le disque virtuel est assez grand

quemu_new_disk: new_disk
	qemu-system-x86_64 diskimg/boot.bin -m 8M -no-reboot -no-shutdown -d cpu_reset


recomp_only_kernel: bootloader kernel link
	cat compiled/bootloader.bin compiled/kernel.bin > compiled/boot.bin
	cp compiled/boot.bin ./diskimg/boot_new.bin
	echo "taille: " $$(stat -c %s diskimg/boot_new.bin)
	dd if=diskimg/boot_new.bin of=diskimg/boot.bin bs=1 count=$$(stat -c %s diskimg/boot_new.bin) conv=notrunc

quemu_persist_disk: recomp_only_kernel
	qemu-system-x86_64 diskimg/boot.bin -m 8M -no-reboot -no-shutdown -d cpu_reset,int 


@PHONY: clean
clean:
	rm compiled/* 
