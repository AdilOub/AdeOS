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

setVGA_graphic:
    mov ah, 0x00
    mov al, 0x13
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

    ; mov [0xb8000], byte '6'
    ; mov [0xb8002], byte '4'
    ; mov [0xb8004], byte ' '
    ; mov [0xb8006], byte 'b'
    ; mov [0xb8008], byte 'i'
    ; mov [0xb800A], byte 't'
    ; mov [0xb800C], byte 's'
    
    ;jmp $
    ;mov [0xa0C8a], byte 0x2F

    call _start ;on lance le kernel en C 
    
    ; mov [0xb80F0], byte 'F'
    ; mov [0xb80F2], byte 'i'
    ; mov [0xb80F4], byte 'n'
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


NoSSEMsg:
    db "SSE not available", 0
ExtendedProgramStart:
    db "jmpOK, Debut du programme etendu...", 0

ProtectedModeOk:
    db "Protected mode ok...", 0

Debug:
    db "Fin extended program", 0


times 512 dw 0xcafe
times 4096-($-$$) db 122 ;512*8 sectors
