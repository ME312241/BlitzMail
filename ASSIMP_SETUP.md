# Assimp Download and Build Instructions

This document explains how to download and build Assimp 5.x for use with BlitzMail.

## Quick Setup (Linux)

Run this script to automatically download and build Assimp:

```bash
#!/bin/bash
# setup_assimp.sh - Download and build Assimp 5.4.3

cd "$(dirname "$0")"

# Create external directory
mkdir -p external
cd external

# Download Assimp 5.4.3
echo "Downloading Assimp 5.4.3..."
wget https://github.com/assimp/assimp/archive/refs/tags/v5.4.3.tar.gz -O assimp-5.4.3.tar.gz

# Extract
echo "Extracting..."
tar -xzf assimp-5.4.3.tar.gz

# Build
echo "Building Assimp..."
cd assimp-5.4.3
mkdir -p build
cd build

cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=../../assimp-install \
    -DASSIMP_BUILD_TESTS=OFF \
    -DASSIMP_BUILD_ASSIMP_TOOLS=OFF

make -j$(nproc)
make install

echo "Assimp installed to external/assimp-install"
echo "You can now run 'make' to build BlitzMail"
```

Save this as `setup_assimp.sh`, make it executable, and run:

```bash
chmod +x setup_assimp.sh
./setup_assimp.sh
```

## Manual Setup

### Linux

#### Prerequisites
```bash
sudo apt-get update
sudo apt-get install -y cmake build-essential wget
```

#### Download and Build
```bash
# Create external directory
mkdir -p external
cd external

# Download Assimp 5.4.3
wget https://github.com/assimp/assimp/archive/refs/tags/v5.4.3.tar.gz
tar -xzf v5.4.3.tar.gz
cd assimp-5.4.3

# Configure with CMake
mkdir build
cd build
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=../../assimp-install \
    -DASSIMP_BUILD_TESTS=OFF \
    -DASSIMP_BUILD_ASSIMP_TOOLS=OFF

# Build (use all CPU cores)
make -j$(nproc)

# Install to external/assimp-install
make install

# Return to project root
cd ../../..
```

### Windows

#### Using Pre-built Binaries

1. **Download** Assimp pre-built binaries:
   - Go to: https://github.com/assimp/assimp/releases
   - Download the Windows SDK for your Visual Studio version
   - Example: `assimp-sdk-5.4.3-win-vc143-x64.zip` for VS 2022

2. **Extract** to `external/assimp-install/`
   - The structure should be:
     ```
     external/
     └── assimp-install/
         ├── include/
         │   └── assimp/
         └── lib/
             └── assimp-vc143-mt.lib
     ```

3. **Copy DLLs** to your project directory:
   - Copy `assimp-vc143-mt.dll` from the bin folder to your project root

#### Building from Source (Windows)

1. **Prerequisites**:
   - Visual Studio 2019 or later
   - CMake (3.10 or later)
   - Git

2. **Download**:
   ```cmd
   mkdir external
   cd external
   git clone https://github.com/assimp/assimp.git -b v5.4.3 assimp-5.4.3
   cd assimp-5.4.3
   ```

3. **Configure**:
   ```cmd
   mkdir build
   cd build
   cmake .. -G "Visual Studio 16 2019" ^
       -DCMAKE_INSTALL_PREFIX=../../assimp-install ^
       -DASSIMP_BUILD_TESTS=OFF ^
       -DASSIMP_BUILD_ASSIMP_TOOLS=OFF
   ```

4. **Build**:
   ```cmd
   cmake --build . --config Release
   cmake --install . --config Release
   ```

### macOS

#### Using Homebrew
```bash
# Install Assimp via Homebrew
brew install assimp

# Update Makefile to use system Assimp
# Change: -Iexternal/assimp-install/include
# To:     -I/usr/local/include
# And:    -Lexternal/assimp-install/lib
# To:     -L/usr/local/lib
```

#### Building from Source
```bash
# Prerequisites
brew install cmake

# Download
mkdir -p external
cd external
curl -L https://github.com/assimp/assimp/archive/refs/tags/v5.4.3.tar.gz -o assimp-5.4.3.tar.gz
tar -xzf assimp-5.4.3.tar.gz
cd assimp-5.4.3

# Build
mkdir build
cd build
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=../../assimp-install \
    -DASSIMP_BUILD_TESTS=OFF \
    -DASSIMP_BUILD_ASSIMP_TOOLS=OFF

make -j$(sysctl -n hw.ncpu)
make install
cd ../../..
```

## Verification

After installation, verify the structure:

```
external/
└── assimp-install/
    ├── include/
    │   └── assimp/
    │       ├── Importer.hpp
    │       ├── scene.h
    │       ├── postprocess.h
    │       └── ... (other headers)
    └── lib/
        ├── libassimp.so (Linux)
        ├── libassimp.dylib (macOS)
        └── assimp-vc143-mt.lib (Windows)
```

Test the installation:
```bash
# Linux/macOS
./test_assimp

# Windows
test_assimp.exe
```

Expected output:
```
===== Assimp Model Loading Test =====

Testing: models/98-hikerbasemesh/Player.blend... SUCCESS
Testing: models/6od9waw1za0w-fence/fence/cerca.blend... SUCCESS
...
All tests PASSED! ✓
```

## Troubleshooting

### "Cannot find Assimp headers"
- Verify `external/assimp-install/include/assimp/` exists
- Check include paths in Makefile or Visual Studio project

### "Cannot find Assimp library"
- Verify library files in `external/assimp-install/lib/`
- On Linux: Check that `libassimp.so.5` exists
- On Windows: Check that `assimp-vc143-mt.lib` exists

### "DLL not found" (Windows)
- Copy the `.dll` file from the Assimp bin folder to the project directory
- Or add the Assimp bin folder to your system PATH

### Build takes too long
- Use `-j$(nproc)` on Linux to parallelize the build
- On a 4-core system, this should take 2-5 minutes

### CMake fails
- Ensure CMake version is 3.10 or later: `cmake --version`
- Install required development tools
- Check for error messages about missing dependencies

## Alternative: System Installation

### Linux (Debian/Ubuntu)
```bash
sudo apt-get install libassimp-dev
```
Then update the Makefile to use system paths.

### Linux (Fedora/CentOS)
```bash
sudo dnf install assimp-devel
```

### Limitations of System Installation
- May not be the latest version (5.4.3)
- Project won't be self-contained
- Different versions on different systems

**Recommendation**: Use the local installation in `external/assimp-install/` for consistency.

## Version Compatibility

BlitzMail has been tested with:
- ✅ Assimp 5.4.3 (recommended)
- ✅ Assimp 5.3.x (should work)
- ✅ Assimp 5.2.x (should work)
- ⚠️ Assimp 5.0.x (may have issues with some formats)
- ❌ Assimp 4.x (API changes, not supported)

## Support

For Assimp-specific issues:
- Assimp documentation: https://assimp.org/
- Assimp GitHub issues: https://github.com/assimp/assimp/issues

For BlitzMail integration issues:
- Check ASSIMP_INTEGRATION.md
- Review the test program output
- Verify model file paths are correct
