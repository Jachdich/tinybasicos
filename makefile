C_SOURCES = $(wildcard kernel/*.c drivers/*.c libc/*.c cpu/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h libc/*.h cpu/*.h)
#C_SOURCES = $(wildcard kernel/*.c drivers/videodriver.c libc/mem.c)
#HEADERS   = $(wildcard kernel/*.h drivers/videodriver.h libc/mem.h)

# Nice syntax for file extension replacement
OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o} 

# Change this if your cross-compiler is somewhere else
CC = /usr/bin/gcc
#GDB = /usr/local/i386elfgcc/bin/i386-elf-gdb
# -g: Use debugging symbols in gcc
CFLAGS = -m32 -g

# First rule is run by default
os-image: boot/boot.bin kernel.bin
	cat $^ > os-image

# '--oformat binary' deletes all symbols as a collateral, so we don't need
# to 'strip' them manually on this case
kernel.bin: boot/kernel_entry.o ${OBJ}
	ld -o $@ -Ttext 0x1000 $^ --oformat binary -m elf_i386

# Used for debugging purposes
kernel.elf: boot/kernel_entry.o ${OBJ}
	ld -o $@ -Ttext 0x1000 $^ -m elf_i386

run: os-image
	qemu-system-x86_64 -fda os-image

# Open the connection to qemu and load our kernel-object file with symbols
debug: os-image kernel.elf
	qemu-system-i386 -s -fda os-image -d guest_errors,int &
	gdb -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

# Generic rules for wildcards
# To make an object, always compile from its .c
%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.as
	nasm $< -f elf -o $@

%.bin: %.as
	nasm $< -f bin -o $@


clean:
	rm -rf *.bin *.dis *.o os-image *.elf
	rm -rf kernel/*.o boot/*.bin drivers/*.o boot/*.o
