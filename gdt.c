#include <stdint.h>

// Structure representation of a structural GDT entry layout segment descriptor
typedef struct {
    uint16_t limit_low;     // Lower 16 bits of the limit boundary area
    uint16_t base_low;      // Lower 16 bits of the base allocation pointer
    uint8_t  base_middle;   // Next 8 bits of the base address layout reference
    uint8_t  access;        // Access configuration flags determining ring levels
    uint8_t  granularity;   // Operational bit sizes limits and paging scales
    uint8_t  base_high;     // Final 8 bits of the base layout offset address
} __attribute__((packed)) gdt_entry_t;

// Structural layout pointer for loading code sequence
typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) gdt_ptr_t;

gdt_entry_t gdt_entries[5];
gdt_ptr_t   gdt_ptr;

// Embedded assembly execution link to flush segments mapping
extern void gdt_flush(uint32_t);

void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt_entries[num].base_low    = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high   = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low   = (limit & 0xFFFF);
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;

    gdt_entries[num].granularity |= gran & 0xF0;
    gdt_entries[num].access      = access;
}

void init_gdt() {
    gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
    gdt_ptr.base  = (uint32_t)&gdt_entries;

    // Gate 0: Null descriptor mandatory space
    gdt_set_gate(0, 0, 0, 0, 0);
    // Gate 1: Kernel Code Segment (Privilege Ring 0 Execution)
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    // Gate 2: Kernel Data Segment (Privilege Ring 0 Storage Memory)
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    // Gate 3: User Mode Code Segment (Privilege Ring 3 Sandboxed Apps)
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
    // Gate 4: User Mode Data Segment (Privilege Ring 3 Application Stack)
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

    gdt_flush((uint32_t)&gdt_ptr);
}
