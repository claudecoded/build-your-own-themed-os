#ifndef CPU_H
#define CPU_H

#include <stdint.h>

// Registers structure passed to interrupt handlers
typedef struct {
    uint32_t ds;                                     // Data segment selector
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha
    uint32_t int_no, err_code;                       // Interrupt number and error code
    uint32_t eip, cs, eflags, useresp, ss;           // Pushed by the processor automatically
} __attribute__((packed)) registers_t;

// Layout of an IDT entry
typedef struct {
    uint16_t low_offset;  // Lower 16 bits of handler function address
    uint16_t sel;         // Kernel segment selector (GDT)
    uint8_t  always0;     // Always set to 0
    uint8_t  flags;       // Flags (Presence, Privilege, Type)
    uint16_t high_offset; // Higher 16 bits of handler function address
} __attribute__((packed)) idt_gate_t;

// Pointer structure used by 'lidt' assembly command
typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_register_t;

#define IDT_ENTRIES 256

#endif
