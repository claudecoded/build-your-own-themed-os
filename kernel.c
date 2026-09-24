#include "cpu.h"

// External driver and subsystem initializers
void init_gdt();
void init_idt();
void init_keyboard();
void clear_screen();
void print_string(char* message, unsigned char color, int row, int col);
void scroll_screen();

// Global cursor tracking pointers
int current_row = 10;
int current_col = 0;
char command_buffer[256];
int command_index = 0;

// Simple memory comparison tool
int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

// Executes terminal shell actions typed by the user
void execute_command(char* input) {
    current_col = 0;
    current_row++;
    if (current_row >= 25) { scroll_screen(); }

    if (strcmp(input, "help") == 0) {
        print_string("Commands: help - Show this list | clear - Reset screen | sysinfo - CPU details", 0x0E, current_row, current_col);
        current_row++;
    } else if (strcmp(input, "clear") == 0) {
        clear_screen();
        current_row = 0;
    } else if (strcmp(input, "sysinfo") == 0) {
        print_string("CPU State: 32-bit Protected Mode | Privilege: Ring 0 (Kernel Max)", 0x0B, current_row, current_col);
        current_row++;
    } else if (strcmp(input, "") == 0) {
        // Do nothing on empty enter keys
    } else {
        print_string("Unknown Command. Type 'help' for available directives.", 0x0C, current_row, current_col);
        current_row++;
    }

    if (current_row >= 25) { scroll_screen(); }
    
    // Print a new terminal prompt command line
    print_string("root@powerful_cpu:# ", 0x0A, current_row, 0);
    current_col = 19;
}

void main() {
    init_gdt();       // Set up dynamic CPU privileges segments
    init_idt();       // Initialize exception maps interrupts structures
    init_keyboard();  // Boot up the I/O interface driver mapping 
    clear_screen();
    
    print_string("================================================================================", 0x0E, 0, 0);
    print_string("                   POWERFUL CPU CORE BOOTED SUCCESSFULLY                        ", 0x0F, 1, 0);
    print_string("================================================================================", 0x0E, 2, 0);
    print_string("System Security: GDT Sandboxing Loaded Correctly.", 0x0A, 4, 2);
    print_string("Fault Defenses: 32 Hardware Intel ISR Gates Active.", 0x0A, 5, 2);
    print_string("Hardware Status: Real-time Keyboard Interface Controller Online.", 0x0A, 6, 2);
    print_string("Type 'help' to begin interface querying.", 0x07, 8, 2);

    print_string("root@powerful_cpu:# ", 0x0A, current_row, 0);
    current_col = 19;

    while(1) {
        asm volatile("hlt"); // Efficient power consumption processing loop
    }
}

void clear_screen() {
    char* video_memory = (char*)0xb8000;
    for (int i = 0; i < 80 * 25 * 2; i += 2) {
        video_memory[i] = ' ';
        video_memory[i+1] = 0x07;
    }
}

void print_string(char* message, unsigned char color, int row, int col) {
    char* video_memory = (char*)0xb8000;
    int offset = (row * 80 + col) * 2;
    int i = 0;
    while (message[i] != '\0') {
        video_memory[offset] = message[i];
        video_memory[offset+1] = color;
        offset += 2;
        i++;
    }
}

void scroll_screen() {
    char* video_memory = (char*)0xb8000;
    // Shift screen text rows upward by 1 line row gap frame space
    for (int i = 0; i < 24 * 80 * 2; i++) {
        video_memory[i] = video_memory[i + 80 * 2];
    }
    // Wipe out the lowest row cleanly
    for (int i = 24 * 80 * 2; i < 25 * 80 * 2; i += 2) {
        video_memory[i] = ' ';
        video_memory[i+1] = 0x07;
    }
    current_row = 24;
}
