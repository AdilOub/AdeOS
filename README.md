# Message pour mon moi futur
Après plusieurs heures de débug:
On a determiné que l'erreur ne provenais surement pas du linker, mais de la façon dont on charge l'OS


Idée:
Charger 32M dans 0x8000
Puis le reste dans 0x8000+0x8000
etc...

VOIR COMMENT LE TYPE DE https://github.com/VendelinSlezak/BleskOS à fait



bootloader.asm ligne ~820

 ;load first 8 sectors to align reading from floppy
 mov ch, 0 ;cylinder
 mov dh, 0 ;head
 mov cl, 11 ;sector
 mov dl, byte [boot_drive]
 mov bx, 0x0000
 mov ax, 0x1000
 mov es, ax
 mov fs, ax
 mov ah, 0x2 ;read
 mov al, 8 ;8 sectors
 pusha
 int 13h
 popa
 jc error_loading
 
 mov edi, 0x10000+8*512
 mov esi, 0
 mov ch, 0 ;cylinder
 mov dh, 1 ;head
 mov cl, 1 ;sector
 
 .floppy_load_cylinder:
  cmp ch, 22 ;load 22x36 sectors = 396 KB
  ja .select_graphic_mode
  
  ;load