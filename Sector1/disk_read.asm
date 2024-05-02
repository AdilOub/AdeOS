NB_OF_SECTOR equ 64 ;nb of sector in extended_program (8) plus some to avoid flashy screen
PROGRAM_SPACE equ 0x8000 ;program space dans le linker
;on va essayer de le load dans 0x10000
;adresse = segment*16 + offset
;0x8000 = 0x1000*16 + 0x0000
;segment register: cs, ds, es, fs, gs et ss


;si on est en -hda on peut lire 128 secteurs directement 
;mais dans ce cas il ne faut pas jne DiskReadFailed
ReadDisk_old:
    mov ax, 0x800 ;adresse de destination 0x800*16=0x8000
    mov es, ax
    mov ah, 0x02 ;mode lecture floppy
    mov bx, 0x000;destination
    mov al, 64 ;nb of sector to read
    mov dl, [BOOT_DISK] ;disk number
    mov ch, 0 ;cylinder 0
    xor dh, dh ;head 0
    mov cl, 2 ;on lit à partir du secteur 2, le secteur 1 est le bootloader

    int 0x13
    ;cmp ah, 0
    ;jne DiskReadFailed ;jmp if err

    ;ret

    ;on reset

    ;on lit le reste à la suite, doonc à l'adresse PROGRAM_SPACE + 512*NB_OF_SECTOR=0x10000
    mov ax, 0x1000 ;adresse de destination 0x1000*16=0x10000
    mov es, ax
    mov ah, 0x02 ;mode lecture floppy
    mov bx, 0x200;destination offset
    mov al, 1 ;nb of sector to read
    mov dl, [BOOT_DISK] ;disk number
    mov ch, 1 ;cylinder 1 car on a deja lu les 64 secteurs du cylinder 0
    xor dh, dh ;head 0
    mov cl, 1 ;secteur 1
    int 0x13
    cmp ah, 0
    jne DiskReadFailed2 ;jmp if err

    ;on affiche le 1er hexa à l'adresse 0x10000
    ; mov ax, 0xFFF
    ; mov es, ax
    ; mov bx, [es:0x00F]
    ; call PrintHex
    ; call JmpLine
    ; jmp $

    ret


;y'a un probleme avec l'alignement dans le linker
ReadDisk:
    ;on lit les 32k premiers octets à l'adresse es:bx (0x8000)
    mov ax, 0x800;
    mov es, ax
    mov ch, 0 ;on commence par le cylinder 0
    mov cl, 2 ;on veut pas lire le bootloader
    call ReadDiskInterupt

    ;on log disk 1 ok
    mov bx, Disk1Ok
    call PrintString
    call JmpLine


    ;on lit le reste à l'adresse PROGRAM_SPACE + 512*NB_OF_SECTOR=0x10000
    mov ax, 0x1000
    mov es, ax
    mov ch, 1 ;on passe au cylinder 1
    mov cl, 1 ;on commence par le secteur 1
    call ReadDiskInterupt

    ret

ReadDiskInterupt: ;on s'attend à ce que le cylindre soit set
    mov ah, 0x02        ; Mode lecture floppy
    mov bx, 0x000       ; Offset de destination
    mov al, 64          ; Nombre de secteurs à lire (64 secteurs)
    mov dl, [BOOT_DISK] ; Numéro du disque
    ;mov ch, 0           ; Numéro du cylindre
    xor dh, dh          ; Tête de disque
    ;mov cl, 2           ; Secteur de départ (2 pour commencer après le bootloader)

    int 0x13 
    cmp ah, 0
    jne DiskReadFailed ;jmp if err

    ret


BOOT_DISK: 
    db 0


Disk1Ok:
    db "Disk 1 OK", 0

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
