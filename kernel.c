// Minimal 32-bit Kernel in C
void clear_screen();
void print_string(char* message, unsigned char color, int row, int col);

void main() {
    clear_screen();
    
    // Draw welcome screen interface
    print_string("==================================================", 0x0E, 2, 15); // Yellow
    print_string("         BUILD YOUR OWN OS - KERNEL READY         ", 0x0F, 3, 15); // White
    print_string("==================================================", 0x0E, 4, 15);
    
    print_string("Welcome to your custom operating system environment!", 0x0A, 7, 12); // Green
    print_string("Modify 'kernel.c' to implement your features.", 0x07, 9, 12);   // Light Gray
    print_string("Status: Running smoothly in 32-bit Protected Mode.", 0x0B, 11, 12); // Cyan
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
