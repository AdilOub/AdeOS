[BITS 64]

main:
    push rbp
    mov rbp, rsp
    mov DWORD [rbp-4], edi
    mov eax, DWORD [rbp-4]
    add eax, 0x1
    pop rbp
    ret
