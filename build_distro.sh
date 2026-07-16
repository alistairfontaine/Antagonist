#!/bin/bash
set -e

echo "========================================================================"
echo "🚀 INITIATING ANTAGONIST FILE-SYSTEM WORKSTATION LOOPS..."
echo "========================================================================"

# Clean and recompile our self-contained workspace object packages
make clean
make all

echo "========================================================================"
echo "✅ WORKSPACE SYSTEM TARGET COMPILED! LAUNCHING WORKSTATION HARNESS..."
echo "========================================================================"

# Launch our workstation directly using our stable, verified loader flag
make run
