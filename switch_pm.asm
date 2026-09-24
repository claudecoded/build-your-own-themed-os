; Transition mechanism from Real Mode to Protected Mode
[bits 16]
switch_to_pm:
    cli                     ; 1. Clear/disable interrupts
    lgdt [gdt_descriptor]   ; 2. Load the Global Descriptor Table
    mov eax, cr0            ; 3. Set the first bit of CR0 control register
    or eax, 0x1
    mov cr0, eax
    jmp CODE_SEG:init_pm    ; 4. Far jump to flush the pre-fetch pipeline

[bits 32]
init_pm:
    mov ax, DATA_SEG        ; 5. Update segment registers with data selector
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000        ; 6. Update stack position to safe area
    mov esp, ebp

    call BEGIN_PM
