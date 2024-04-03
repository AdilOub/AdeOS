[org 0x7c00]

mov [BOOT_DISK], dl
cmp dl, 1
je DiskErrorOnBoot


mov bp, 0x7c00 ;stack
mov sp, bp


mov bx, HelloString
call PrintString
call JmpLine

call ReadDisk

call SetVGAMod
jmp PROGRAM_SPACE

jmp $


%include "Sector1/print.asm"
%include "Sector1/disk_read.asm"


HelloString:
    db "Bienvenue sur le bootloader d'AdeOS!", 0

SetVGAMod:
    mov ah, 0x00
    mov al, 0x03 ;VGA 0x03 pour  du text et 0x13 pour afficher des pixels
    int 0x10
    ret

times 510-($-$$) db 0
db 0x55, 0xaa