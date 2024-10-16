#!/bin/bash

# Define build directories
build_dirs=("build-windows" "build-linux" "build-macos")

# Clean up old build directories
for dir in "${build_dirs[@]}"; do
    if [ -d "$dir" ]; then
        rm -rf "$dir"
    fi
done

# Build for Windows (native)
echo "Building for Windows..."
cmake -B build-windows -DCMAKE_BUILD_TYPE=Release
cmake --build build-windows

# Build for Linux (cross-compile)
echo "Cross-compiling for Linux..."
cmake -B build-linux -DCROSS_COMPILE_LINUX=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build-linux

# Build for macOS (cross-compile)
echo "Cross-compiling for macOS..."
cmake -B build-macos -DCROSS_COMPILE_MACOS=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build-macos

echo "Build process completed for all platforms."
