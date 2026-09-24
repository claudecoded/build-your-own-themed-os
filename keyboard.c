#include "cpu.h"

const char scancode_table[] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	'9', '0', '-', '=', '\b',	
  '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	
    0,  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',   0,	
 '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',   0, '*',    0, ' '
};

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(uint16_t port, uint8_t val) {
    asm volatile("outb %0, %1" :: "a"(val), "Nd"(port));
}

extern int current_row;
extern int current_col;
extern char command_buffer[256];
extern int command_index;

extern void print_string(char* message, unsigned char color, int row, int col);
extern void execute_command(char* input);
extern void scroll_screen();

void keyboard_callback() {
    uint8_t scancode = inb(0x60);

    if (!(scancode & 0x80)) {
        if (scancode < sizeof(scancode_table)) {
            char key = scancode_table[scancode];
            
            if (key == '\n') { // Enter key pressed
                command_buffer[command_index] = '\0';
                execute_command(command_buffer);
                command_index = 0;
            } 
            else if (key == '\b') { // Backspace key configuration logic
                if (command_index > 0 && current_col > 19) {
                    command_index--;
                    current_col--;
                    char erase[] = { ' ', '\0' };
                    print_string(erase, 0x07, current_row, current_col);
                }
            } 
            else if (key != 0 && command_index < 254) { // Standard alpha key
                command_buffer[command_index++] = key;
                char visual[] = { key, '\0' };
                print_string(visual, 0x0F, current_row, current_col);
                current_col++;
                
                if (current_col >= 78) {
                    current_col = 0;
                    current_row++;
                    if (current_row >= 25) { scroll_screen(); }
                }
            }
        }
    }
    outb(0x20, 0x20); // End of Interrupt transmission
}

void init_keyboard() {
    outb(0x20, 0x11); outb(0xA0, 0x11);
    outb(0x21, 0x20); outb(0xA1, 0x28);
    outb(0x21, 0x04); outb(0xA1, 0x02);
    outb(0x21, 0x01); outb(0xA1, 0x01);
    outb(0x21, 0x0);  outb(0xA1, 0x0);

    extern void keyboard_interrupt_handler();
    extern void set_idt_gate(int n, uint32_t handler);
    set_idt_gate(33, (uint32_t)keyboard_interrupt_handler);

    asm volatile("sti");
}
