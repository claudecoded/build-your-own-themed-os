#include "cpu.h"

idt_gate_t idt[IDT_ENTRIES];
idt_register_t idt_reg;

extern void load_idt(uint32_t idt_ptr);

// Protótipos externos dos vetores ISR (isr0 até isr31)...
// Configura uma entrada (gate) individual na tabela IDT
void set_idt_gate(int n, uint32_t handler) {
    idt[n].low_offset = (uint16_t)(handler & 0xFFFF);
    idt[n].sel = 0x08;
    idt[n].always0 = 0;
    idt[n].flags = 0x8E;
    idt[n].high_offset = (uint16_t)((handler >> 16) & 0xFFFF);
}

// Inicializa a tabela IDT vinculando os 32 vetores e carregando o registrador
void init_idt() {
    idt_reg.limit = (sizeof(idt_gate_t) * IDT_ENTRIES) - 1;
    idt_reg.base = (uint32_t)&idt;

    // Vinculação de isr0 a isr31 via set_idt_gate(...)
    
    load_idt((uint32_t)&idt_reg);
}
