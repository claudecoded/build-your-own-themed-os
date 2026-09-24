; Disk Reading Utility using BIOS Interrupts
load_kernel:
    pusha
    push dx

    mov bx, MSG_LOAD_KERNEL
    call print_string

    mov ah, 0x02        ; BIOS read sector function
    mov al, 2           ; Number of sectors to read (increase if kernel grows)
    mov ch, 0x00        ; Cylinder 0
    mov dh, 0x00        ; Head 0
    mov cl, 0x02        ; Start reading from sector 2 (sector 1 is the bootloader)
    mov dl, [BOOT_DRIVE]
    mov bx, KERNEL_OFFSET ; Destination buffer pointer [ES:BX]

    int 0x13
    jc disk_error       ; Jump if error flag (carry) is set

    pop dx
    cmp al, dh          ; BIOS sets AL to number of sectors actually read
    jne disk_error
    popa
    ret

disk_error:
    mov bx, DISK_ERROR_MSG
    call print_string
    jmp $

DISK_ERROR_MSG db "Disk read error occurred!", 0x0a, 0x0d, 0
