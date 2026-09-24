void clear_screen();
void print_string(char* message, unsigned char color, int row, int col);
void init_idt();
void init_keyboard();

void main() {
    clear_screen();
    
    print_string("==================================================", 0x0E, 2, 15);
    print_string("         BUILD YOUR OWN OS - KERNEL READY         ", 0x0F, 3, 15);
    print_string("==================================================", 0x0E, 4, 15);
    
    print_string("Initializing IDT and Keyboard drivers...", 0x07, 7, 12);
    init_idt();
    init_keyboard();
    
    print_string("System Status: Operational and Ready!", 0x0A, 9, 12);
    print_string("Type something on your keyboard below:", 0x0B, 13, 12);
    
    // Infinite loop waiting for hardware interrupt calls
    while(1) {
        asm volatile("hlt"); // Halts the CPU until the next interrupt arrives
    }
}
// (Mantém as funções clear_screen e print_string iguais às de antes)
