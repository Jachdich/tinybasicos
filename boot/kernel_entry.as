[bits 32]
[extern main]
global _start;
extern kernel_main;
_start:
call kernel_main
jmp $
