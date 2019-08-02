[bits  32]
        ; Define  some  constants
        VIDEO_MEM  equ 0xb8000
        WHITE_ON_BLACK  equ 0x0f


print:
	pusha
	mov edx, VIDEO_MEM
	mov ah, 0x0f

	printstart:
		mov al, [ebx]
		cmp al, 0

		jz endprint
		mov ah, 0x0f
	
		mov [edx], ax

		add edx, 2
		inc ebx

		jmp printstart

	endprint:
	popa
	ret
