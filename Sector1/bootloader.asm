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
jmp PROGRAM_SPACE
jmp $


%include "Sector1/print.asm"
%include "Sector1/disk_read.asm"


HelloString:
    db "Bienvenue sur Emi OS!", 0

times 510-($-$$) db 0
db 0x55, 0xaa
