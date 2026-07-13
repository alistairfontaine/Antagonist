# Define our compiler tools and target architecture flags
AS = nasm
CC = g++
LD = ld

# Freestanding C++ target compilation flags matrix with local include pathways
ASFLAGS = -f elf32
CFLAGS  = -m32 -c -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -Iinclude -std=c++20
LDFLAGS = -m elf_i386 -T ../FontaineOS/linker.ld

# Define our destination pathways matching our strict folder tree rules
KERNEL_OBJS = ../FontaineOS/src/boot.o ../FontaineOS/src/kernel.o ../FontaineOS/src/gdt.o ../FontaineOS/src/idt.o ../FontaineOS/src/timer.o ../FontaineOS/src/keyboard.o ../FontaineOS/src/pmm.o ../FontaineOS/src/vmm.o ../FontaineOS/src/heap.o ../FontaineOS/src/task.o ../FontaineOS/src/ata.o
DISTRO_OBJS = src/main.o src/vbe.o src/buffer.o src/palette.o

all: bin/antagonist.bin bin/disk.img

# Monolithic Linking Stage: Fuses your kernel core with your graphic layers!
bin/antagonist.bin: $(DISTRO_OBJS)
	mkdir -p bin
	$(LD) $(LDFLAGS) -o bin/antagonist.bin $(KERNEL_OBJS) $(DISTRO_OBJS)

# Automatically generate a clean 10MB virtual hard drive image with a valid MBR signature
bin/disk.img:
	mkdir -p bin
	dd if=/dev/zero of=bin/disk.img bs=512 count=20480 2>/dev/null
	printf '\x55\xAA' | dd of=bin/disk.img bs=1 seek=510 conv=notrunc 2>/dev/null

src/main.o: src/main.cpp
	$(CC) $(CFLAGS) src/main.cpp -o src/main.o

src/vbe.o: src/vbe.cpp
	$(CC) $(CFLAGS) src/vbe.cpp -o src/vbe.o

src/buffer.o: src/buffer.cpp
	$(CC) $(CFLAGS) src/buffer.cpp -o src/buffer.o

src/palette.o: src/palette.cpp
	$(CC) $(CFLAGS) src/palette.cpp -o src/palette.o

# Run Target: Mounts the raw multi-drive image to QEMU's primary master IDE bus lines
run: bin/antagonist.bin bin/disk.img
	qemu-system-i386 -kernel bin/antagonist.bin -drive file=bin/disk.img,format=raw,index=0,media=disk

clean:
	rm -f src/*.o bin/*.bin bin/disk.img
