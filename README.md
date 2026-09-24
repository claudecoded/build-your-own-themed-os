<img width="1456" height="720" alt="image" src="https://github.com/user-attachments/assets/9d7e83bc-3e84-4500-8f76-30dc0b7df8ca" />

# Build Your Own OS 🚀

Welcome to the ultimate base boilerplate template to build your very own standalone x86 Operating System from absolute scratch. This repository contains a fully working 16-bit Bootloader, transitions directly to 32-bit Protected Mode, loads a custom freestanding C Kernel into physical memory, and initializes an independent visual framework on screen.

## 🛠️ Project Architecture

```text
├── boot.asm          # Primary Bootloader sector (Stage 1)
├── print_real.asm    # Real Mode 16-bit BIOS string output routines
├── disk.asm          # ATA Drive block sector mapping using BIOS interrupts
├── gdt.asm           # Global Descriptor Table segmentation profiles 
├── switch_pm.asm     # Bit management logic to step up into protected space
├── print_pm.asm      # 32-bit Protected Mode direct-to-VRAM text renderer
├── kernel_entry.asm  # Top alignment linker interface bridge to C entrypoint
├── kernel.c          # Freestanding C operating system kernel code execution
└── Makefile          # Compiling pipelines, build rules, and runtime runner
```

## ⚙️ Dependencies Setup

To compile, map, link, and test this project, you will need an Assembler compiler, standard target optimization tools, and a virtual machine hypervisor platform.

### Ubuntu / Debian
```bash
sudo apt update
sudo apt install nasm qemu-system-x86 build-essential gcc-multilib
```

### macOS
```bash
brew install nasm qemu
# Note: For full compilation pipelines on Apple Silicon or macOS Intel, 
# you will need an x86_64-elf cross-compiler target installation toolchain (gcc/ld).
```

## 🚀 Building and Running

Simply execute the compiled automated orchestration workflows via standard make rules:

1. **Compile the system image:**
   ```bash
   make
   ```
   This generates `os-image.bin`, which combines the raw bootsector block data and compiled kernel files.

2. **Boot the Operating System in QEMU:**
   ```bash
   make run
   ```

3. **Clean workspace artifacts:**
   ```bash
   make clean
   ```

## 🛠️ Where to go from here?
- **Keyboard Interrupts:** Setup an IDT (Interrupt Descriptor Table) to handle user inputs.
- **Memory Allocation:** Implement a physical memory manager and a basic heap allocation algorithm (`malloc`).
- **File System:** Write an implementation for a simple architecture file structure layout like FAT12.

## 🐳 Developing with Docker (Recommended for Mac/Windows)

If you don't want to install toolchains locally, build and run the project using Docker:

1. **Build the container image:**
   ```bash
   docker build -t buy-os-builder .
   ```

2. **Compile the OS using the container:**
   ```bash
   docker run --rm -v \$(pwd):/os-workspace buy-os-builder make
   ```
