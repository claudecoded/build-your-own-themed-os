# Cross-compiler configuration (adjust targets if native on Linux x86_64)
ASM=nasm
CC=gcc
LD=ld

# Directives for building x86 32-bit architecture binaries
CFLAGS=-m32 -ffreestanding -c
LDFLAGS=-m elf_i386 -Ttext 0x1000 --oformat binary

all: os-image.bin

os-image.bin: boot.bin kernel.bin
	cat boot.bin kernel.bin > os-image.bin

boot.bin: boot.asm
	$(ASM) -f bin boot.asm -o boot.bin

kernel.bin: kernel_entry.o kernel.o
	$(LD) $(LDFLAGS) -o kernel.bin kernel_entry.o kernel.o

kernel_entry.o: kernel_entry.asm
	$(ASM) -f elf kernel_entry.asm -o kernel_entry.o

kernel.o: kernel.c
	$(CC) $(CFLAGS) kernel.c -o kernel.o

run: os-image.bin
	qemu-system-x86_64 -drive format=raw,file=os-image.bin

clean:
	rm -rf *.bin *.o *.img
