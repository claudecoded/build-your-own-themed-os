; 16-bit Real Mode Printing Utility
print_string:
    pusha
.loop:
    mov al, [bx]
    cmp al, 0
    je .done
    mov ah, 0x0e
    int 0x10
    inc bx
    jmp .loop
.done:
    popa
    ret
