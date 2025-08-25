
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


;;erreur handler 0 à 31
[extern isr0_handler] ;div by 0
; ;[extern isr1_handler] ;debug
; [extern isr2_handler] ;non maskable interrupt
; ;[extern isr3_handler] ;breakpoint
; [extern isr4_handler] ;overflow
; [extern isr5_handler] ;bound range
[extern isr6_handler] ;invalid opcode
; [extern isr7_handler] ;device not available
[extern isr8_handler] ;double fault
; [extern isr9_handler] ;coprocessor segment overrun (outdated)
; [extern isr10_handler] ;invalid tss
; [extern isr11_handler] ;11
; [extern isr12_handler] ;12
[extern isr13_handler] ; general protection fault
[extern isr14_handler] ; page fault
; [extern isr15_handler] ;15
; [extern isr16_handler] ;16
; [extern isr17_handler] ;17
; [extern isr18_handler] ;18
; [extern isr19_handler] ;19
; [extern isr20_handler] ;20
; [extern isr21_handler] ;21
; [extern isr22_handler] ;22
; [extern isr23_handler] ;23
; [extern isr24_handler] ;24
; [extern isr25_handler] ;25
; [extern isr26_handler] ;26
; [extern isr27_handler] ;27
; [extern isr28_handler] ;28
; [extern isr29_handler] ;29
; [extern isr30_handler] ;30
; [extern isr31_handler] ;31


[extern isr32_handler] ;timer
[extern isr33_handler] ;clavier

[extern isr43_handler] ;rtl8139 ethernet card
[extern isr44_handler] ;ps2 (mouse)

[extern isr38_handler] ;ata primary

[extern isr128_handler] ;software interrupt

;interupt materiel
isr0:
  PUSHALL
  call isr0_handler
  POPALL
  iretq
  GLOBAL isr0


; isr1:
;   PUSHALL
;   call isr1_handler
;   POPALL
;   iretq
;   GLOBAL isr1

; isr2:
;   PUSHALL
;   call isr2_handler
;   POPALL
;   iretq
;   GLOBAL isr2

; isr3:
;   PUSHALL
;   call isr3_handler
;   POPALL
;   iretq
;   GLOBAL isr3

; isr4:
;   PUSHALL
;   call isr4_handler
;   POPALL
;   iretq
;   GLOBAL isr4

; isr5:
;   PUSHALL
;   call isr5_handler
;   POPALL
;   iretq
;   GLOBAL isr5

isr6:
  PUSHALL
  call isr6_handler
  POPALL
  iretq
  GLOBAL isr6

; isr7:
;   PUSHALL
;   call isr7_handler
;   POPALL
;   iretq
;   GLOBAL isr7

isr8:
  PUSHALL
  call isr8_handler
  POPALL
  iretq
  GLOBAL isr8

; isr9:
;   PUSHALL
;   call isr9_handler
;   POPALL
;   iretq
;   GLOBAL isr9

; isr10:
;   PUSHALL
;   call isr10_handler
;   POPALL
;   iretq
;   GLOBAL isr10

; isr11:
;   PUSHALL
;   call isr11_handler
;   POPALL
;   iretq
;   GLOBAL isr11

; isr12:
;   PUSHALL
;   call isr12_handler
;   POPALL
;   iretq
;   GLOBAL isr12

isr13:
  PUSHALL
  call isr13_handler
  POPALL
  iretq
  GLOBAL isr13

isr14:
  PUSHALL
  call isr14_handler
  POPALL
  iretq
  GLOBAL isr14

; isr15:
;   PUSHALL
;   call isr15_handler
;   POPALL
;   iretq
;   GLOBAL isr15

; isr16:
;   PUSHALL
;   call isr16_handler
;   POPALL
;   iretq
;   GLOBAL isr16

; isr17:
;   PUSHALL
;   call isr17_handler
;   POPALL
;   iretq
;   GLOBAL isr17

; isr18:
;   PUSHALL
;   call isr18_handler
;   POPALL
;   iretq
;   GLOBAL isr18

; isr19:
;   PUSHALL
;   call isr19_handler
;   POPALL
;   iretq
;   GLOBAL isr19

; isr20:
;   PUSHALL
;   call isr20_handler
;   POPALL
;   iretq
;   GLOBAL isr20

; isr21:
;   PUSHALL
;   call isr21_handler
;   POPALL
;   iretq
;   GLOBAL isr21

; isr22:
;   PUSHALL
;   call isr22_handler
;   POPALL
;   iretq
;   GLOBAL isr22

; isr23:
;   PUSHALL
;   call isr23_handler
;   POPALL
;   iretq
;   GLOBAL isr23

; isr24:
;   PUSHALL
;   call isr24_handler
;   POPALL
;   iretq
;   GLOBAL isr24

; isr25:
;   PUSHALL
;   call isr25_handler
;   POPALL
;   iretq
;   GLOBAL isr25

; isr26:
;   PUSHALL
;   call isr26_handler
;   POPALL
;   iretq
;   GLOBAL isr26

; isr27:
;   PUSHALL
;   call isr27_handler
;   POPALL
;   iretq
;   GLOBAL isr27

; isr28:
;   PUSHALL
;   call isr28_handler
;   POPALL
;   iretq
;   GLOBAL isr28

; isr29:
;   PUSHALL
;   call isr29_handler
;   POPALL
;   iretq
;   GLOBAL isr29

; isr30:
;   PUSHALL
;   call isr30_handler
;   POPALL
;   iretq
;   GLOBAL isr30

; isr31:
;   PUSHALL
;   call isr31_handler
;   POPALL
;   iretq
;   GLOBAL isr31








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


isr43: ;rtl8139 ethernet card
  PUSHALL
  call isr43_handler
  POPALL
  iretq
  GLOBAL isr43

isr44:
  PUSHALL
  call isr44_handler
  POPALL
  iretq
  GLOBAL isr44

;software interrupt
isr128: ;0x80 pour faire comme unix hihi
  PUSHALL
  call isr128_handler
  POPALL
  iretq
  GLOBAL isr128


LoadIDT:
  lidt[idtDescriptor]
  sti
  ret
  GLOBAL LoadIDT