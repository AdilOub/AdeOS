PROGRAM_SPACE equ 0x8000 ;program space dans le linker
SECTEUR_COUNT equ 68 ;nombre de secteur à lire, taille de boot.bin/512 - 1


;WARNING!
;on n'est pas censé pouvoir lire plus de 64 secteurs à la fois
;mais avec un disque dur ça marche (pas avec un floppy) et on n'a pas d'erreur
;à l'inverse, essayer de lire les secteurs de manière séparée ne marche pas :/
;on va donc tout lire d'un coup... (j'ai passé trop de temps à essayer de résoudre ce problème)
;pour l'instant on est en dessous de 64 ça va.
;peut être qu'un hdd à plus que 64 secteurs par cylindre et que c'est pour ça que ça marche?


ReadDisk:
    xor ah, ah
    int 0x13 ;reset disk

    ;; ON COMMENCE PAR LIRE LE EXTENDED BOOTLOADER
    ;; un secteur fait 512 bytes. On doit lire 8 secteurs pour lire le bootloader extendu
    mov ax, 0x800 ;adresse de destination
    mov es, ax
    mov ch, 0 ;on commence par le cylinder 0
    mov cl, 2 ;on veut pas lire le bootloader, on commence par le secteur 2
    mov ah, 0x02 ;mode lecture floppy
    mov bx, 0 ;offset de destination
    mov al, SECTEUR_COUNT ;nb de secteurs à lire
    mov dl, [BOOT_DISK] ;on lit les 8 secteurs du bootloader extended
    xor dh, dh ;tête de disque
    int 0x13
    ;cmp ah, 0
    jc DiskReadFailed ;jmp if err


    ;on log disk 1 ok
    mov bx, Disk1Ok
    call PrintString
    call JmpLine

    ;on lit ensuite le reste


    ; mov ax, 0x8E0;
    ; mov es, ax
    ; mov ch, 0 ;on commence par le cylinder 0
    ; mov cl, 9 ;on veut pas lire le bootloader, on commence par le secteur 2
    ; mov bl, 57 ;le nombre de secteur j'aurai dit 55 mais c'est 57
    ; call ReadDiskInterupt ;on a décalé de 1 donc on peut pas lire 64 secteurs

    ret

;argument: bl: nombre de secteur à lire
ReadDiskInterupt: ;on s'attend à ce que le cylindre soit set
    mov ah, 0x02        ; Mode lecture floppy
    mov al, bl          ; Nombre de secteurs à lire (64 secteurs)
    xor bx, bx       ; Offset de destination
    mov dl, [BOOT_DISK] ; Numéro du disque
    ;mov ch, 0           ; Numéro du cylindre
    xor dh, dh          ; Tête de disque
    ;mov cl, 2           ; Secteur de départ (2 pour commencer après le bootloader)

    int 0x13 
    cmp ah, 0
    jne DiskReadFailed ;jmp if err
    ret


BOOT_DISK: 
    db 0b ;0x80 pour le disque C, 0 pour le floppy


Disk1Ok:
    db "boot ext OK", 0

Disk2Ok:
    db "Other Disks OK", 0

DiskErrorOnBootMsg:
    db "Disk Error On Boot", 0
DiskReadErrorMsg:
    db "Disk Read Failed", 0

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
    ;ret
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
