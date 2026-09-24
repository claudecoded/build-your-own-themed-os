[bits 32]
[global gdt_flush]

gdt_flush:
    mov eax, [esp + 4]  ; Fetch data object argument offset reference memory 
    lgdt [eax]          ; Load the actual new active structure blueprint map

    mov ax, 0x10        ; 0x10 maps accurately into the Kernel Data Segment 
    mov ds, ax          ; Sync internal pipelines safely
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.flush     ; Far jump targeting segment position 0x08 (Kernel Code)
.flush:
    ret
