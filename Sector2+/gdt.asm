GDT_nulldesc:
    dd 0
    dd 0
GDT_codedesc:
    dw 0xFFFF ;limit
    dw 0x0000 ;base low
    db 0x00 ;base medium
    db 10011010b ;flag (present, privilege ring 0, code, read, accessed)
    db 11001111b ;flag upper
    db 0x00 ;base hight
GDT_datadesc:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10010010b
    db 11001111b
    db 0x00
GDT_end:

GDT_desc:
    GDT_size:
        dw GDT_end - GDT_nulldesc - 1
        dd GDT_nulldesc

codeseg equ GDT_codedesc - GDT_nulldesc
dataseg equ GDT_datadesc - GDT_nulldesc

[bits 32]
EditGDT:
    mov [GDT_codedesc + 6], byte 10101111b
    mov [GDT_datadesc + 6], byte 10101111b
    ret

[bits 16]