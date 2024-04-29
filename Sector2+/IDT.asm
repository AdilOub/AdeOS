
[extern _idt]
idtDescriptor:
  dw 4095
  dq _idt


  %macro PUSHALL 0
  	push rax
  	push rcx
  	push rdx
  	push r8
  	push r9
  	push r10
  	push r11
  %endmacro

  %macro POPALL 0
  	pop r11
  	pop r10
  	pop r9
  	pop r8
  	pop rdx
  	pop rcx
  	pop rax

  %endmacro

[extern isr0_handler] ;div by 0
[extern isr8_handler] ;double fault

[extern isr32_handler] ;timer
[extern isr33_handler] ;clavier

[extern isr38_handler] ;ata primary

;interupt materiel
isr0:
  PUSHALL
  call isr0_handler
  POPALL
  iretq
  GLOBAL isr0


isr8:
  PUSHALL
  call isr8_handler
  POPALL
  iretq
  GLOBAL isr8

isr32:
  PUSHALL
  call isr32_handler
  POPALL
  iretq
  GLOBAL isr32

isr33:
  PUSHALL
  call isr33_handler
  POPALL
  iretq
  GLOBAL isr33

isr38:
  PUSHALL
  call isr38_handler
  POPALL
  iretq
  GLOBAL isr38


LoadIDT:
  lidt[idtDescriptor]
  sti
  ret
  GLOBAL LoadIDT