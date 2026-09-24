# ... (Mantenha as configurações iniciais do Makefile anteriores)

kernel.bin: kernel_entry.o interrupts.o idt.o keyboard.o kernel.o
	$(LD) $(LDFLAGS) -o kernel.bin kernel_entry.o interrupts.o idt.o keyboard.o kernel.o

kernel_entry.o: kernel_entry.asm
	$(ASM) -f elf kernel_entry.asm -o kernel_entry.o

interrupts.o: interrupts.asm
	$(ASM) -f elf interrupts.asm -o interrupts.o

idt.o: idt.c cpu.h
	$(CC) $(CFLAGS) idt.c -o idt.o

keyboard.o: keyboard.c cpu.h
	$(CC) $(CFLAGS) keyboard.c -o keyboard.o

kernel.o: kernel.c
	$(CC) $(CFLAGS) kernel.c -o kernel.o

# ... (Mantenha as regras run e clean anteriores)
