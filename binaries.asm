%macro IncBin 2
    SECTION .rodata
    GLOBAL %1
%1:
    incbin %2
    db 
    %1_size: dq %1_size - %1
%endmacro

IncBin Test "test.txt"