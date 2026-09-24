#include "cpu.h"

// Scancode mapping to US Keyboard layout
const char scancode_table[] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	'9', '0', '-', '=', '\b',	
  '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	
    0,  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',   0,	
 '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',   0, '*',    0, ' '
};

// Assembly inline functions to communicate with hardware I/O ports
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(uint16_t port, uint8_t val) {
    asm volatile("outb %0, %1" :: "a"(val), "Nd"(port));
}

// Global visual terminal variables (tracks where to print next character)
int current_row = 15;
int current_col = 12;

extern void print_string(char* message, unsigned char color, int row, int col);

void keyboard_callback() {
    // Read raw code byte from keyboard controller register port
    uint8_t scancode = inb(0x60);

    // If highest bit is clear, it means a key was pressed (not released)
    if (!(scancode & 0x80)) {
        if (scancode < sizeof(scancode_table) && scancode_table[scancode] != 0) {
            char key[2] = { scancode_table[scancode], '\0' };
            
            // Print the key pressed to screen
            print_string(key, 0x0F, current_row, current_col);
            current_col++;
            
            // Basic screen wrapping protection line wrap
            if (current_col >= 70) {
                current_col = 12;
                current_row++;
            }
        }
    }

    // Send EOI (End of Interrupt) signal acknowledgement to PIC (Programmable Interrupt Controller)
    outb(0x20, 0x20);
}

void init_keyboard() {
    // Initialize the Programmable Interrupt Controllers (PIC)
    // Remap hardware IRQs so IRQ0-7 maps to software interrupts 32-39
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);

    // Register our assembly wrapper handler into IDT gate position 33 (IRQ1)
    extern void keyboard_interrupt_handler();
    extern void set_idt_gate(int n, uint32_t handler);
    set_idt_gate(33, (uint32_t)keyboard_interrupt_handler);

    // Re-enable interrupts execution on the CPU architecture level
    asm volatile("sti");
}
