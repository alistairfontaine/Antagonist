#!/bin/bash
set -e

echo "========================================================================"
echo "🚀 INITIATING ANTAGONIST INDEPENDENT MONOLITHIC BUILD PIPELINE..."
echo "========================================================================"

# Clean local objects to avoid linking conflicts
make clean

# Compile the complete, self-contained local workspace package tree
make all

echo "========================================================================"
echo "✅ SELF-CONTAINED DISTRO COMPILED! LAUNCHING EMULATOR HARNESS..."
echo "========================================================================"

# Boot the platform inside QEMU standard UI panel windows
make run
