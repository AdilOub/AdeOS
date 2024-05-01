NB_OF_SECTOR equ 64 ;nb of sector in extended_program (8) plus some to avoid flashy screen
PROGRAM_SPACE equ 0x8000 ;program space dans le linker


ReadDisk:
    mov ah, 0x02 ;mode lecture floppy
    mov bx, PROGRAM_SPACE ;destination
    mov al, NB_OF_SECTOR ;nb of sector to read
    mov dl, [BOOT_DISK] ;disk number
    xor ch, ch ;cylinder 0
    xor dh, dh ;head 0
    mov cl, ah ;secteur

    int 0x13
    cmp ah, 0
    jne DiskReadFailed ;jmp if err

    ret


BOOT_DISK: 
    db 0


DiskErrorOnBootMsg:
    db "Disk Error On Boot", 0
DiskReadErrorMsg:
    db "Disk Read Failed", 0
DiskReadErrorMsg2:
    db "Disk Read Failed 2", 0

;differente message d'erreur possible lors de la lecture du disque
; InvalidCommandMsg: ;0x01
;     db "Invalid Command", 0
; CannotFindAdressMarkMsg: ;0x02
;     db "Cannot Find Address Mark", 0

; AttemptWriteToWriteProtectedDiskMsg: ;0x03
;     db "Attempt Write To Write Protected Disk", 0

; SectorNotFoundMsg: ;0x04
;     db "Sector Not Found", 0

; ResetFailedMsg: ;0x05
;     db "Reset Failed", 0

; OtherMsg:
;     db "Other Error", 0


DiskErrorOnBoot:
    mov bx, DiskErrorOnBootMsg
    call PrintString
    jmp $


; InvalidCmd:
;     mov bx, InvalidCommandMsg
;     call PrintString
;     jmp $

; CannotFindAdressMark:
;     mov bx, CannotFindAdressMarkMsg
;     call PrintString
;     jmp $

; AttemptWriteToWriteProtectedDisk:
;     mov bx, AttemptWriteToWriteProtectedDiskMsg
;     call PrintString
;     jmp $

; SectorNotFound:
;     mov bx, SectorNotFoundMsg
;     call PrintString
;     jmp $   

; ResetFailed:
;     mov bx, ResetFailedMsg
;     call PrintString
;     jmp $

; OtherError:
;     mov bx, OtherMsg
;     call PrintString
;     jmp $



DiskReadFailed2:
    mov bx, DiskReadErrorMsg2
    call PrintString
    call JmpLine
    call DiskReadFailed

DiskReadFailed:
    push ax
    mov bx, DiskReadErrorMsg
    call PrintString
    call JmpLine
    pop ax
    and ax, 0xFF00 ;on garde que le code d'erreur
    mov bx, ax
    ;push ax
    call PrintHex
    call JmpLine
    jmp $
    ;pop ax
    ;on determine quelle est l'erreur
    ; cmp ah, 0x01
    ; je InvalidCmd
    ; cmp ah, 0x02
    ; je CannotFindAdressMark
    ; cmp ah, 0x03
    ; je AttemptWriteToWriteProtectedDisk
    ; cmp ah, 0x04
    ; je SectorNotFound
    ; cmp ah, 0x05
    ; je ResetFailed
    ; jmp OtherError
