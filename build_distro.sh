#!/bin/bash
set -e

echo "========================================================================"
echo "💿 INITIATING ANTAGONIST PRODUCTION DISK GEN INTERFACE (PHASE B7)..."
echo "========================================================================"

# 1. Recompile our self-contained workspace modules and kernel objects cleanly
make clean
make all

echo "========================================================================"
echo "⚙️  PACKAGING STANDALONE BOOTLOADER DESCRIPTORS & CONFIGS..."
echo "========================================================================"

# 2. Establish a strict, compliant standalone GRUB directory tree footprint
mkdir -p iso/boot/grub

# 3. Securely copy your un-mangled kernel target binary into the packaging lane
cp bin/antagonist.bin iso/boot/antagonist.bin

# 4. Generate the explicit master GRUB hardware configuration script descriptor
cat << 'EOF' > iso/boot/grub/grub.cfg
set timeout=0
set default=0

menuentry "Antagonist Workstation OS Distribution Release" {
    multiboot /boot/antagonist.bin
    boot
}
EOF

echo "========================================================================"
echo "💿 BAKING MASTER PUBLIC WORKSTATION CD-ROM ISO DISK IMAGE..."
echo "========================================================================"

# 5. Pack your assets and kernel stubs securely into a completely portable live ISO image
grub-mkrescue -o bin/antagonist.iso iso 2>/dev/null

echo "========================================================================"
echo "🖥️  WAKING UP STANDALONE VIRTUAL RELEASE HARDWARE WORKSPACE NODE..."
echo "========================================================================"

# 6. Run QEMU as a freestanding hardware machine node, forcing it to read the ISO natively!
make run
