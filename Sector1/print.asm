PrintString: ;bx: pointer on string, 0 terminated
    mov ah, 0x0e
    .loop:
    cmp [bx], byte 0
    je .exit
    mov al, [bx]
    int 0x10
    inc bx
    jmp .loop
    .exit:
    ret

PrintHex: ;bx - number
    mov cx, 4
    .loop:
    mov dx, bx
    shr dx, 12
    cmp dx, 10
    jl .digit
    add dl, 'A' - 10
    jmp .print
    .digit:
    add dl, '0'
    .print:
    mov ah, 0x0e
    mov al, dl
    int 0x10
    shl bx, 4
    loop .loop
    ret

JmpLine:
    mov ah, 0x0e
    mov al, 0x0d ;\r
    int 0x10
    mov al, 0x0a ;\n
    int 0x10
    ret
