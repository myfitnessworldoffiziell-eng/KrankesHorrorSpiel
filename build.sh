#!/bin/bash

# ECHOES Build Script

echo "=================================="
echo "  Building ECHOES.exe"
echo "=================================="
echo ""

# Create build directory
if [ ! -d "build" ]; then
    echo "Creating build directory..."
    mkdir build
fi

cd build

# Run CMake
echo "Running CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Release

if [ $? -ne 0 ]; then
    echo ""
    echo "ERROR: CMake configuration failed!"
    echo "Make sure you have all dependencies installed:"
    echo "  sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev"
    exit 1
fi

# Build
echo ""
echo "Building..."
make -j$(nproc)

if [ $? -ne 0 ]; then
    echo ""
    echo "ERROR: Build failed!"
    exit 1
fi

echo ""
echo "=================================="
echo "  Build successful!"
echo "=================================="
echo ""
echo "Run the game with:"
echo "  cd build && ./ECHOES"
echo ""
