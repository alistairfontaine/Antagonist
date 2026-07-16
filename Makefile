# Define our compiler tools and target architecture flags
AS = nasm
CC = g++
LD = ld

# Freestanding C++ target compilation flags matrix with local include pathways
ASFLAGS = -f elf32
CFLAGS  = -m32 -c -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -Iinclude -std=c++20
LDFLAGS = -m elf_i386 -T ../FontaineOS/linker.ld

# Define our destination pathways matching our strict folder tree rules
KERNEL_OBJS = ../FontaineOS/src/boot.o ../FontaineOS/src/gdt.o ../FontaineOS/src/idt.o ../FontaineOS/src/timer.o ../FontaineOS/src/keyboard.o ../FontaineOS/src/pmm.o ../FontaineOS/src/vmm.o ../FontaineOS/src/heap.o ../FontaineOS/src/task.o ../FontaineOS/src/ata.o
DISTRO_OBJS = src/kernel_sandbox.o src/main.o src/vbe.o src/buffer.o src/palette.o src/input.o src/math3d.o src/raycast.o src/collision.o src/spatial.o src/selection.o src/particle.o src/modification.o src/tensor.o src/weights.o src/vocab.o src/tokenizer.o src/threads.o src/font.o src/persistence.o


all: bin/antagonist.bin bin/disk.img

# Monolithic Linking Stage: Fuses your kernel core with your graphic and cognitive layers!
bin/antagonist.bin: $(DISTRO_OBJS)
	mkdir -p bin
	$(LD) $(LDFLAGS) -o bin/antagonist.bin $(KERNEL_OBJS) $(DISTRO_OBJS)

# Automatically generate a clean 10MB virtual hard drive image with a valid MBR signature
bin/disk.img:
	mkdir -p bin
	dd if=/dev/zero of=bin/disk.img bs=512 count=20480 2>/dev/null
	printf '\x55\xAA' | dd of=bin/disk.img bs=1 seek=510 conv=notrunc 2>/dev/null

src/kernel_sandbox.o: src/kernel_sandbox.cpp
	$(CC) $(CFLAGS) -I../FontaineOS/include src/kernel_sandbox.cpp -o src/kernel_sandbox.o

src/main.o: src/main.cpp
	$(CC) $(CFLAGS) src/main.cpp -o src/main.o


src/vbe.o: src/vbe.cpp
	$(CC) $(CFLAGS) src/vbe.cpp -o src/vbe.o

src/buffer.o: src/buffer.cpp
	$(CC) $(CFLAGS) src/buffer.cpp -o src/buffer.o

src/palette.o: src/palette.cpp
	$(CC) $(CFLAGS) src/palette.cpp -o src/palette.o

src/input.o: src/input.cpp
	$(CC) $(CFLAGS) src/input.cpp -o src/input.o

src/math3d.o: src/math3d.cpp
	$(CC) $(CFLAGS) src/math3d.cpp -o src/math3d.o

src/raycast.o: src/raycast.cpp
	$(CC) $(CFLAGS) src/raycast.cpp -o src/raycast.o

src/collision.o: src/collision.cpp
	$(CC) $(CFLAGS) src/collision.cpp -o src/collision.o

src/spatial.o: src/spatial.cpp
	$(CC) $(CFLAGS) src/spatial.cpp -o src/spatial.o

src/selection.o: src/selection.cpp
	$(CC) $(CFLAGS) src/selection.cpp -o src/selection.o

src/particle.o: src/particle.cpp
	$(CC) $(CFLAGS) src/particle.cpp -o src/particle.o

src/modification.o: src/modification.cpp
	$(CC) $(CFLAGS) src/modification.cpp -o src/modification.o

src/tensor.o: src/tensor.cpp
	$(CC) $(CFLAGS) src/tensor.cpp -o src/tensor.o

src/weights.o: src/weights.cpp
	$(CC) $(CFLAGS) src/weights.cpp -o src/weights.o

src/vocab.o: src/vocab.cpp
	$(CC) $(CFLAGS) src/vocab.cpp -o src/vocab.o

src/tokenizer.o: src/tokenizer.cpp
	$(CC) $(CFLAGS) src/tokenizer.cpp -o src/tokenizer.o

src/threads.o: src/threads.cpp
	$(CC) $(CFLAGS) src/threads.cpp -o src/threads.o

src/font.o: src/font.cpp
	$(CC) $(CFLAGS) src/font.cpp -o src/font.o

src/persistence.o: src/persistence.cpp
	$(CC) $(CFLAGS) src/persistence.cpp -o src/persistence.o

# Run Target: Forcefully wakes up an external high-resolution window interface frame!
run: bin/antagonist.bin bin/disk.img
	qemu-system-i386 -kernel bin/antagonist.bin -drive file=bin/disk.img,format=raw,index=0,media=disk -vga std -display sdl

clean:
	rm -f src/*.o bin/*.bin bin/disk.img
