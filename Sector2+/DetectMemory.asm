MemoryRegionCount:
 db 0
 GLOBAL MemoryRegionCount

DetectMemeory:
    mov ax, 0
    mov es, ax
    mov di, 0x5000 ;aribtary
    mov edx, 0x534D4150 ;magic number
    xor ebx, ebx

    .repeat:
        mov eax, 0xE820 ;mn
        mov ecx, 24
        int 0x15
        cmp ebx, 0
        je .finished
        add di, 24 ;os dev wiki moment
        inc byte [MemoryRegionCount]
        jmp .repeat

    .finished:
        ret