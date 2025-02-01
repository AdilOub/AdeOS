;[org 0x7e00] ;linker do that for us




jmp EnterProtectedMode



%include "Sector2+/gdt.asm"
%include "Sector1/print.asm"
%include "Sector2+/DetectMemory.asm"

;it:
;disable int
;enable A20 Line
;load GDT
;set GDT in register

;de base c'était set dans bootloader.asm en mode 0x03 (text mode)
setVGA_graphic:
    mov ah, 0x00
    mov al, 0x12
    int 0x10
    ret

EnterProtectedMode:
    mov bx, ExtendedProgramStart
    call PrintString
    call JmpLine

    ;call setVGA_graphic
    
    call DetectMemeory
    call EnableA20
    cli ;jmp to 32 bits on desactive les interruptions
    lgdt [GDT_desc]
    mov eax, cr0 ;enable gdt, chaging last bit of cr0 to 1
    or eax, 1
    mov cr0, eax

    ;

    jmp codeseg:StartProtectedMode

EnableA20:
    in al, 0x92
    or al, 2
    out 0x92, al
    ret


[bits 32]
%include "Sector2+/CPUID.asm"
%include "Sector2+/SimplePaging.asm"
StartProtectedMode:
    mov ax, dataseg ;point segment register to new data from GDT 
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ;we can move our stack to smthg larger
    ;mov ebp, 0x9000
    ;mov esp, ebp


    ;mov [0xb8000], byte '3'
    ;mov [0xb8002], byte '2'
    ;mov [0xb8004], byte ' '
    ;mov [0xb8006], byte 'b'
    ;mov [0xb8008], byte 'i'
    ;mov [0xb800A], byte 't'
    ;mov [0xb800C], byte 's'

    ;TODO break memory print
    call DetectCPUID
    call DetectLongMode
    call SetupIDPaging
    call EditGDT ;for 64 bits


    jmp codeseg:Start64Bit

[bits 64]
[extern _start]
%include "Sector2+/IDT.asm"

Start64Bit:
    ;mov edi, 0xb8000
    ;mov rax, 0x1f201f201f201f20 ;met l'écran en bleu
    ;mov ecx, 500
    ;rep stosq

    mov rbp, 0x90000
    mov rsp, rbp
    call ActivateSSE

    ;mov [0xA000], byte '6'
    ;call _start 
    ;jmp $

    mov [0xb8000], byte '6'
    mov [0xb8002], byte '4'
    mov [0xb8004], byte ' '
    mov [0xb8006], byte 'b'
    mov [0xb8008], byte 'i'
    mov [0xb800A], byte 't'
    mov [0xb800C], byte '!'
    
    ;jmp $
    ;mov [0xa0C8a], byte 0x2F

    ; mov rdi, 0
    ; mov rsi, 1
    ; mov rdx, buffer
    ; call ata_lba_read

    ;afficher la valeur de buffer[0] dans l'écran
    
    ; mov bx, [buffer]
    ; mov rcx, 0xb80D0
    ; call PrintHexInVGA


    call _start ;on lance le kernel en C 
    
    mov [0xb80F0], byte 'F'
    mov [0xb80F2], byte 'i'
    mov [0xb80F4], byte 'n'
    mov [0xb80F6], byte '!'
    ; mov ax, [0x8000+4093]
    ; mov [0xb80F6], ax

    jmp $

ActivateSSE:
    ;check if SSE is available
    mov eax, 0x1
    cpuid
    test edx, 1 << 25
    jz .no_sse
    mov rax, cr0
    and ax, 0xFFB ;0xFFFB clear EM bit
    or ax, 0x2 ;on met le coprocessor en C0.MP 
    mov cr0, rax 
    mov rax, cr4
    or ax, 3<<9 ;on active les bits 9 et 10 pour CR4.OSFXSR et CR4.OSXMMEXCPT
    mov cr4, rax
    ret
    .no_sse:
    mov rdi, NoSSEMsg
    call PrintString
    jmp $


;bl - number < 256
;rcx - endroit ou on veut afficher le nombre
;pas d'interuption, pour afficher on fait mov [cx], al
PrintHexInVGA:
    mov [rcx], byte '0'
    mov [rcx+2], byte 'x'
    push bx
    and bl, 0xF0
    shr bl, 4
    cmp bl, 0x0A
    jl .digit
    ;c'est une lettre
    mov al, 'A'
    add al, bl
    sub al, 10
    mov [rcx+4], al
    jmp .nodigit
    .digit:
    mov al, '0'
    add al, bl
    mov [rcx+4], al
    .nodigit:
    pop bx
    and bl, 0x0F
    cmp bl, 0xA
    jl .digit2
    ;c'est une lettre
    mov al, 'A'
    add al, bl
    sub al, 10
    mov [rcx+6], al
    ret
    .digit2:
    mov al, '0'
    add al, bl
    mov [rcx+6], al
    ret

    


;=============================================================================
; ATA read sectors (LBA mode) 
;
; @param EAX Logical Block Address of sector
; @param CL  Number of sectors to read
; @param RDI The address of buffer to put data obtained from disk
;
; @return None
;=============================================================================
;argument rdi, rsi, rdx pour utiliser l'extern "C"
ata_lba_read:
            mov rax, rdi
            mov rcx, rsi
            mov rdi, rdx
             pushfq
               and rax, 0x0FFFFFFF
               push rax
               push rbx
               push rcx
               push rdx
               push rdi

               mov rbx, rax         ; Save LBA in RBX
               
               mov edx, 0x01F6      ; Port to send drive and bit 24 - 27 of LBA
               shr eax, 24          ; Get bit 24 - 27 in al
               or al, 11100000b     ; Set bit 6 in al for LBA mode
               out dx, al

               mov edx, 0x01F2      ; Port to send number of sectors
               mov al, cl           ; Get number of sectors from CL
               out dx, al
               
               mov edx, 0x1F3       ; Port to send bit 0 - 7 of LBA
               mov eax, ebx         ; Get LBA from EBX
               out dx, al

               mov edx, 0x1F4       ; Port to send bit 8 - 15 of LBA
               mov eax, ebx         ; Get LBA from EBX
               shr eax, 8           ; Get bit 8 - 15 in AL
               out dx, al


               mov edx, 0x1F5       ; Port to send bit 16 - 23 of LBA
               mov eax, ebx         ; Get LBA from EBX
               shr eax, 16          ; Get bit 16 - 23 in AL
               out dx, al

               mov edx, 0x1F7       ; Command port
               mov al, 0x20         ; Read with retry.
               out dx, al

.still_going:  in al, dx
               test al, 8           ; the sector buffer requires servicing.
               jz .still_going      ; until the sector buffer is ready.

               mov rax, 256         ; to read 256 words = 1 sector
               xor bx, bx
               mov bl, cl           ; read CL sectors
               mul bx
               mov rcx, rax         ; RCX is counter for INSW
               mov rdx, 0x1F0       ; Data port, in and out
               rep insw             ; in to [RDI]

               pop rdi
               pop rdx
               pop rcx
               pop rbx
               pop rax
               popfq
            ret
GLOBAL ata_lba_read

buffer:
    times 256 db 0x12
buffer2:
    times 256 db 0x12

NoSSEMsg:
    db "SSE not available", 0
ExtendedProgramStart:
    db "jmpOK, Debut du programme etendu...", 0

ProtectedModeOk:
    db "Protected mode ok...", 0

Debug:
    db "Fin extended program", 0


;times 512 dw 0xcafe
times 4096-($-$$) db 122 ;512*8 sectors
