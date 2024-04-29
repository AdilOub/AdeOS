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
EnterProtectedMode:

    call DetectMemeory
    call EnableA20
    cli 
    lgdt [GDT_desc]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
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
    ;mov rax, 0x1f201f201f201f20
    ;mov ecx, 500
    ;rep stosq

    ;mov rbp, 0x90000
    ;mov rsp, rbp

    call ActivateSSE

    ;mov [0xa0C8a], byte 0x2F
    call _start

    jmp $

ActivateSSE:
    mov rax, cr0
    and ax, 0b11111101
    or ax, 0b00000001
    mov cr0, rax
    mov rax, cr4
    or ax, 0b1100000000
    mov cr4, rax
    ret

times 4096-($-$$) db 0 ;512*8 sectors