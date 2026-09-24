; =====================================================================
; Build Your Own OS - Minimal Stage 1 Bootloader
; =====================================================================
[org 0x7c00]            ; BIOS loads bootloader at physical address 0x7c00

KERNEL_OFFSET equ 0x1000 ; Memory offset where we will load our kernel

    mov [BOOT_DRIVE], dl ; BIOS stores the boot drive number in DL on startup

    ; Set up stack
    mov bp, 0x9000
    mov sp, bp

    mov bx, MSG_REAL_MODE
    call print_string

    call load_kernel     ; Read kernel from disk
    call switch_to_pm    ; Disable interrupts, load GDT, jump to 32-bit PM
    jmp $                ; Hang if execution somehow returns

%include "print_real.asm"
%include "disk.asm"
%include "gdt.asm"
%include "switch_pm.asm"

[bits 32]
BEGIN_PM:
    mov ebx, MSG_PROT_MODE
    call print_string_pm
    call KERNEL_OFFSET   ; Jump to the entry point of our loaded kernel code
    jmp $                ; Hang if the kernel returns

; Global variables
BOOT_DRIVE      db 0
MSG_REAL_MODE   db "Started in 16-bit Real Mode", 0x0a, 0x0d, 0
MSG_PROT_MODE   db "Successfully switched to 32-bit Protected Mode", 0
MSG_LOAD_KERNEL db "Loading kernel into memory...", 0x0a, 0x0d, 0

; Bootsector padding and magic number
times 510-($-$$) db 0
dw 0xaa55
