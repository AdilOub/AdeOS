[org 0x7c00]

mov [BOOT_DISK], dl ;normalement 0x80 pour C
cmp dl, 1
je DiskErrorOnBoot

;on met le stack au bon endroit
cli
xor ax, ax
mov ds, ax
mov es, ax
mov ss, ax
mov sp, 0x7c00
sti 







; ;call oldReadDisk
call ReadDisk



mov bx, DiskOk
call PrintString
call JmpLine


call SetVGAMod

mov bx, VGAOk
call PrintString
call JmpLine

mov bx, TryToJmp
call PrintString
call JmpLine

mov bx, PROGRAM_SPACE
call PrintHex
call JmpLine

jmp PROGRAM_SPACE

mov bx, FailedToJmp
call PrintString
call JmpLine

jmp $



%include "src/bootloader/Sector1/print.asm"
%include "src/bootloader/Sector1/disk_read.asm"


DiskOk:
    db "DD OK...", 0

VGAOk:
    db "VGA OK...", 0

TryToJmp:
    db "jmping extended prog at:", 0

FailedToJmp:
    db "jmp failed :c", 0

SetVGAMod:
    mov ah, 0x00
    mov al, 0x03 ;VGA 0x03 pour du text et 0x13 pour afficher des pixels
    int 0x10
    ret

times 510-($-$$) db 0
db 0x55, 0xaa