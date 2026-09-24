[bits 32]
[global load_idt]
[global keyboard_interrupt_handler]
[external keyboard_callback]

; Load the IDT pointer into the CPU
load_idt:
    mov eax, [esp + 4]
    lidt [eax]
    ret

; Assembly wrapper for the keyboard hardware interrupt
keyboard_interrupt_handler:
    pusha               ; Save all general-purpose registers
    mov ax, ds          ; Save data segment descriptor
    push eax

    mov ax, 0x10        ; Load kernel data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call keyboard_callback ; Call our C function to process the key

    pop eax             ; Restore data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa                ; Restore general-purpose registers
    iretd               ; Interrupt return (pops EIP, CS, EFLAGS)
