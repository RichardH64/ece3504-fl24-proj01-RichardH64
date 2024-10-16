# build_all.ps1
# Define build directories
$buildDirs = @{
    Windows = "build-windows"
    Linux = "build-linux"
    macOS = "build-macos"
}

# Clean up old build directories
foreach ($dir in $buildDirs.Values) {
    if (Test-Path $dir) {
        Remove-Item -Recurse -Force $dir
    }
}

# Build for Windows (native)
Write-Host "Building for Windows..."
cmake -B $buildDirs.Windows -DCMAKE_BUILD_TYPE=Release
cmake --build $buildDirs.Windows

# Build for Linux (cross-compile)
Write-Host "Cross-compiling for Linux..."
cmake -B $buildDirs.Linux -DCROSS_COMPILE_LINUX=ON -DCMAKE_BUILD_TYPE=Release
cmake --build $buildDirs.Linux

# Build for macOS (cross-compile)
Write-Host "Cross-compiling for macOS..."
cmake -B $buildDirs.macOS -DCROSS_COMPILE_MACOS=ON -DCMAKE_BUILD_TYPE=Release
cmake --build $buildDirs.macOS

Write-Host "Build process completed for all platforms."