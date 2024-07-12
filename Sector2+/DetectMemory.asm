MemoryRegionCount:
 db 0
 GLOBAL MemoryRegionCount

DetectMemeory:
    ;mov ax, 0x800;
    mov ax, 0
    mov es, ax
    mov di, 0x5000 ;adresse ou on va stocker les regions de memoire: 0x800:0x4=0x8004, à modifier dans Memory.h
    xor ebx, ebx
    xor eax, eax;

    .repeat:
        mov eax, 0xE820 ;magic number
        mov ecx, 24 ;magic number 2
        int 0x15
        cmp ebx, 0
        je .finished
        ;cmp edx, 0x534D4150 ;on doit verifier ça avant ebx pour le 1er appel
        ;jne .ErrorDetectMemory
        add di, 24 ;taille de la structure
        inc byte [MemoryRegionCount]
        jmp .repeat

    .ErrorDetectMemory:
        mov bx, ErrorDetectMemoryMsg
        call PrintString
        call JmpLine
        jmp $

    .finished:
        ret
    


ErrorDetectMemoryMsg:
    db "Error detecting memory regions", 0

