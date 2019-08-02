[org 0x7c00]
KERNEL_OFFSET  equ 0x1000   ; This is the  memory  offset  to which  we will  load  our  kernel
VIDEO_MEMORY      equ 0xb8000
W_ON_B         equ 0x0f

mov [BOOT_DRIVE], dl ; BIOS  stores  our  boot  drive  in DL

mov bp, 0x9000         ; Set -up the  stack.
mov sp, bp
mov bx, MSG_START
call print_16b

call  load_kernel       ; Load  our  kernel
call  switch_to_pm      ; Switch  to  protected  mode

jmp $ ; do nothing

%include "boot/print_16b.as"
%include "boot/disk_load.as"
%include "boot/gdt.as"
%include "boot/print.as"
%include "boot/switch_to_pm.as"	

[bits 16]

load_kernel:
        mov bx, MSG_LOAD_KERNEL    ; Print a message  to say we are  loading  the  kernel
        call print_16b
        mov bx, KERNEL_OFFSET      ; Set -up  parameters  for  our  disk_load  routine , so
        mov dh, 15                   ; that we load  the  first  15  sectors (excluding
        mov dl, [BOOT_DRIVE]       ; the  boot  sector) from  the  boot  disk (i.e.  our
        call disk_load               ;   kernel  code) to  address  KERNEL_OFFSET
        ret

BEGIN_PM:
	[bits 32]

	mov ebx, MSG
	call print
	
	call KERNEL_OFFSET

	jmp $                      ; Hang.

	MSG                db "STARTING THE DANK KERNEL!", 0x0d, 0x0a, 0
	MSG_LOAD_KERNEL    db "LOADING DAT DANK KERNEL!", 0x0d, 0x0a, 0
	MSG_START	   db "DANK OS IS STARTING MY DUDES!", 0x0d, 0x0a, 0

	BOOT_DRIVE        db 0
        times  510-($-$$) db 0
        dw 0xaa55
        
