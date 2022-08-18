NB_OF_SECTOR equ 128 ;nb of sector in extended_program (8) plus some to avoid flashy screen
PROGRAM_SPACE equ 0x8000

ReadDisk:
    mov ah, 0x02
    mov bx, PROGRAM_SPACE
    mov al, NB_OF_SECTOR
    mov dl, [BOOT_DISK]
    xor ch, ch
    xor dh, dh
    mov cl, ah

    int 0x13
    ;je DiskReadFailed ;jmp if err
    ret


BOOT_DISK: 
    db 0


DiskErrorOnBootMsg:
    db "Disk Error On Boot", 0
DiskReadErrorMsg:
    db "Disk Read Failed", 0

DiskErrorOnBoot:
    mov bx, DiskErrorOnBootMsg
    call PrintString
    jmp $

DiskReadFailed:
    mov bx, DiskReadErrorMsg
    call PrintString
    jmp $