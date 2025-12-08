#!/bin/bash
# setup_assimp.sh - Download and build Assimp 5.4.3 for BlitzMail

set -e  # Exit on error

echo "===== Assimp Setup for BlitzMail ====="
echo ""

# Get script directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

# Create external directory
echo "Creating external directory..."
mkdir -p external
cd external

# Check if already installed
if [ -d "assimp-install/include/assimp" ] && [ -f "assimp-install/lib/libassimp.so" ]; then
    echo "Assimp appears to already be installed in external/assimp-install/"
    echo "To reinstall, remove the external directory and run this script again."
    exit 0
fi

# Download Assimp 5.4.3
if [ ! -f "assimp-5.4.3.tar.gz" ]; then
    echo "Downloading Assimp 5.4.3..."
    wget https://github.com/assimp/assimp/archive/refs/tags/v5.4.3.tar.gz -O assimp-5.4.3.tar.gz
else
    echo "Assimp 5.4.3 archive already downloaded."
fi

# Extract
if [ ! -d "assimp-5.4.3" ]; then
    echo "Extracting..."
    tar -xzf assimp-5.4.3.tar.gz
else
    echo "Assimp source already extracted."
fi

# Build
echo "Building Assimp (this may take a few minutes)..."
cd assimp-5.4.3
mkdir -p build
cd build

cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=../../assimp-install \
    -DASSIMP_BUILD_TESTS=OFF \
    -DASSIMP_BUILD_ASSIMP_TOOLS=OFF

echo "Compiling with $(nproc) cores..."
make -j$(nproc)

echo "Installing..."
make install

cd "$SCRIPT_DIR"

echo ""
echo "===== Setup Complete ====="
echo "Assimp has been installed to: external/assimp-install"
echo ""
echo "Next steps:"
echo "  1. Run 'make' to build BlitzMail"
echo "  2. Run './test_assimp' to verify model loading"
echo "  3. Run './BlitzMail' to play the game"
echo ""
