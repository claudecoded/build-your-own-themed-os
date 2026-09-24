#include "cpu.h"

idt_gate_t idt[IDT_ENTRIES];
idt_register_t idt_reg;

// Implemented in interrupts.asm
extern void load_idt(uint32_t idt_ptr);

void set_idt_gate(int n, uint32_t handler) {
    idt[n].low_offset = (uint16_t)(handler & 0xFFFF);
    idt[n].sel = 0x08; // Kernel Code Segment offset in GDT
    idt[n].always0 = 0;
    idt[n].flags = 0x8E; // 10001110b (Present, Ring 0, 32-bit Interrupt Gate)
    idt[n].high_offset = (uint16_t)((handler >> 16) & 0xFFFF);
}

void init_idt() {
    idt_reg.limit = (sizeof(idt_gate_t) * IDT_ENTRIES) - 1;
    idt_reg.base = (uint32_t)&idt;

    // Load the IDT pointer into the processor
    load_idt((uint32_t)&idt_reg);
}
