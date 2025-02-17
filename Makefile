rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
ALL_CPP_FILES = $(call rwildcard,src/,*.cpp)
ALL_HEADER_FILES = $(call rwildcard,src/,*.h)
ALL_ASM_FILES = $(call rwildcard,src/,*.asm)

gcc_options = -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -fno-pie -c


bootloader: src/bootloader/Sector1/bootloader.asm src/bootloader/Sector2+/extended_program.asm src/binaries/binaries.asm
	nasm -f bin src/bootloader/Sector1/bootloader.asm -o compiled/bootloader.bin
	nasm -f elf64 src/bootloader/Sector2+/extended_program.asm -o compiled/extended_program.o
	nasm -f elf64 src/binaries/binaries.asm -o compiled/binaries.o

kernel: $(ALL_CPP_FILES) $(ALL_HEADER_FILES)
	gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -fno-pie -c src/Kernel.cpp -o compiled/kernelC.o
	gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -fno-pie -c src/cpu/idt.cpp -o compiled/idt.o
	gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -fno-pie -c src/cpu/io.cpp -o compiled/io.o
	gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -fno-pie -c src/drivers/Keyboard.cpp -o compiled/Keyboard.o
	gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -fno-pie -c src/Memory.cpp -o compiled/Memory.o
	gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -fno-pie -c src/TextPrint.cpp -o compiled/TextPrint.o
	gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -fno-pie -c src/Commands.cpp -o compiled/Commands.o
	gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -fno-pie -c src/Heap.cpp -o compiled/Heap.o
	gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -fno-pie -c src/ASCITable.cpp -o compiled/ASCITable.o
	gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -fno-pie -c src/Render.cpp -o compiled/Render.o
	gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -fno-pie -c src/Math.cpp -o compiled/Math.o
	gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -fno-pie -c src/Timer.cpp -o compiled/Timer.o
	gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -fno-pie -c src/DiskRead.cpp -o compiled/DiskRead.o
	gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -fno-pie -c src/FileSystem.cpp -o compiled/FileSystem.o
	gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -fno-pie -c src/Compiler.cpp -o compiled/Compiler.o
	gcc -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -fno-pie -c src/drivers/Mouse.cpp -o compiled/Mouse.o
	gcc $(gcc_options) src/usr/shell.cpp -o compiled/shell.o
	gcc $(gcc_options) src/libc/string.cpp -o compiled/string.o

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

vbox_img: new_disk
	qemu-img convert -O vdi diskimg/boot.bin diskimg/vbox_disk.vdi

@PHONY: clean
clean:
	rm compiled/* 
