# Use a lightweight stable Linux distribution
FROM ubuntu:22.04

# Prevent interactive prompts during installation
ENV DEBIAN_FRONTEND=noninteractive

# Install compilation tools and dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    gcc-multilib \
    nasm \
    qemu-system-x86 \
    make \
    clean && \
    rm -rf /var/lib/apt/lists/*

# Set the working directory inside the container
WORKDIR /os-workspace

# Default command keeps the container alive or ready for make
CMD ["make"]
