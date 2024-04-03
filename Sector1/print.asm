PrintString: ;bx: pointer on string, 0 terminated
    mov ah, 0x0e
    .loop:


JmpLine:
    mov ah, 0x0e
    mov al, 0x0d ;\r
    int 0x10
    mov al, 0x0a ;\n
    int 0x10
    ret
