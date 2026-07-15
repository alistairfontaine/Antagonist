#!/bin/bash

# ==============================================================================
#            ANTAGONIST OS DISTRIBUTION AUTOMATED RELEASES PIPELINE
# ==============================================================================
# Execution Context: Orchestrates localized dual-directory cross-compilations.
# Fixed: Added -Iinclude flag matrix paths to protect relative compilation loops.
# ==============================================================================

# Exit immediately if any individual pipeline target fails
set -e

echo "========================================================================"
echo "🚀 INITIATING ANTAGONIST MONOLITHIC SYSTEMS INTEGRATION LOOPS..."
echo "========================================================================"

# 1. Clean out the local distro build cache to prevent stale object linking crashes
echo "🧹 Scrubbing intermediate distribution object tracks..."
make clean

# 2. Step into your adjacent core micro-kernel workspace and re-compile the silicon foundations
echo "📦 Transitioning workspace context to kernel project directory..."
cd ../FontaineOS

echo "🛠️ Rebuilding stable FontaineOS kernel modules under include-shielded flags..."
# Forcefully execute the kernel's compilation tools to verify object integrity
make clean || true
nasm -f elf32 src/boot.s -o src/boot.o
g++ -m32 -c -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -std=c++20 -Iinclude src/kernel.cpp -o src/kernel.o
g++ -m32 -c -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -std=c++20 -Iinclude src/gdt.cpp -o src/gdt.o
g++ -m32 -c -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -std=c++20 -Iinclude src/idt.cpp -o src/idt.o
g++ -m32 -c -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -std=c++20 -Iinclude src/timer.cpp -o src/timer.o
g++ -m32 -c -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -std=c++20 -Iinclude src/keyboard.cpp -o src/keyboard.o
g++ -m32 -c -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -std=c++20 -Iinclude src/pmm.cpp -o src/pmm.o
g++ -m32 -c -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -std=c++20 -Iinclude src/vmm.cpp -o src/vmm.o
g++ -m32 -c -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -std=c++20 -Iinclude src/heap.cpp -o src/heap.o
g++ -m32 -c -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -std=c++20 -Iinclude src/task.cpp -o src/task.o
g++ -m32 -c -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -std=c++20 -Iinclude src/ata.cpp -o src/ata.o

# 3. Return back to our distribution hub repository to link the complete package tree
echo "📂 Returning to Antagonist distribution master branch hub..."
cd ../Antagonist

echo "⛓️ Fusing hardware kernel blocks with user-space graphics and neural networks..."
make all

# 4. Git Version Control Logging Backup System Automation Pass
echo "📝 Synchronizing distribution repository tree state tracks with remote cloud..."
git add .
git commit -m "build: fix kernel include tracking paths inside deployment script pass Y" || true
git push origin main || true

echo "========================================================================"
echo "✅ ARCHITECTURE LINK COMPLETELY PROCESSED! IGNITING BARE-METAL HARNESS..."
echo "========================================================================"

# 5. Execute hardware virtualization loading loops across our fresh system disk media
make run
