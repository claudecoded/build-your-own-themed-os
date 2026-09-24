#include "cpu.h"

extern void print_string(char* message, unsigned char color, int row, int col);

// Pre-defined structural system exception text string errors reference maps
const char *exception_messages[] = {
    "Division By Zero Exception",
    "Debug Exception",
    "Non Maskable Interrupt Exception",
    "Breakpoint Exception",
    "Into Detected Overflow Exception",
    "Out of Bounds Exception",
    "Invalid Opcode Exception",
    "No Coprocessor Exception",
    "Double Fault Exception",
    "Coprocessor Segment Overrun",
    "Bad TSS Exception",
    "Segment Not Present Exception",
    "Stack Fault Exception",
    "General Protection Fault Exception",
    "Page Fault Memory Allocation Exception",
    "Unknown Interrupt Exception Exception",
    "Coprocessor Fault Exception",
    "Alignment Check Exception",
    "Machine Check Exception",
    "Reserved Exceptions Group",
    "Reserved Exceptions Group",
    "Reserved Exceptions Group",
    "Reserved Exceptions Group",
    "Reserved Exceptions Group",
    "Reserved Exceptions Group",
    "Reserved Exceptions Group",
    "Reserved Exceptions Group",
    "Reserved Exceptions Group",
    "Reserved Exceptions Group",
    "Reserved Exceptions Group",
    "Reserved Exceptions Group",
    "Reserved Exceptions Group"
};

void isr_handler(registers_t regs) {
    if (regs.int_no < 32) {
        clear_screen();
        print_string("============= CRITICAL ARCHITECTURE EXCEPTION =============", 0x4F, 2, 10); // Red screen layout
        print_string((char*)exception_messages[regs.int_no], 0x4F, 4, 12);
        print_string("SYSTEM EXECUTION HALTED SECURELY TO PREVENT CORRUPTION", 0x4F, 6, 12);
        
        // Lock process state directly 
        while(1) {
            asm volatile("cli; hlt");
        }
    }
}
