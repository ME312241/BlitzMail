# BlitzMail - Build Instructions

This document provides detailed instructions for building BlitzMail on Windows, Linux, and macOS.

## Prerequisites

BlitzMail requires the following dependencies:

- **C++ Compiler**: GCC 4.8+, Clang 3.3+, or Visual Studio 2015+
- **OpenGL**: Version 1.1 or higher (usually provided by graphics drivers)
- **GLUT/FreeGLUT**: For window management and input handling
- **Assimp 5.x**: For 3D model loading (.blend, .obj, .3ds, .fbx, etc.)
- **CMake** (optional): Version 3.10+ for cross-platform builds

---

## Building on Linux

### Ubuntu/Debian

```bash
# Install dependencies
sudo apt-get update
sudo apt-get install -y build-essential freeglut3-dev libassimp-dev pkg-config

# Clone repository (if not already cloned)
git clone https://github.com/ME312241/BlitzMail.git
cd BlitzMail

# Option 1: Build with Make
make clean
make
./BlitzMail

# Option 2: Build with CMake
mkdir -p build && cd build
cmake ..
cmake --build .
./BlitzMail
```

### Fedora/RHEL/CentOS

```bash
# Install dependencies
sudo dnf install -y gcc-c++ freeglut-devel assimp-devel mesa-libGL-devel mesa-libGLU-devel

# Build with Make
make clean
make
./BlitzMail

# Or build with CMake
mkdir -p build && cd build
cmake ..
cmake --build .
./BlitzMail
```

### Arch Linux

```bash
# Install dependencies
sudo pacman -S base-devel freeglut assimp mesa

# Build
make
./BlitzMail
```

---

## Building on Windows

### Option 1: Visual Studio with vcpkg (Recommended)

vcpkg is a package manager for C++ that simplifies dependency management on Windows.

```cmd
# Install vcpkg (one-time setup)
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg integrate install

# Install dependencies
.\vcpkg install assimp:x86-windows freeglut:x86-windows

# Return to BlitzMail directory
cd ..\BlitzMail

# Open OpenGL3DTemplate.sln in Visual Studio 2022
# Select Debug or Release configuration
# Press F5 to build and run
```

### Option 2: Visual Studio with Manual Setup

1. **Download Assimp**
   - Download Assimp 5.x binaries from: https://github.com/assimp/assimp/releases
   - Extract to `C:\Program Files\Assimp` or `external\assimp-install` in project directory

2. **Download FreeGLUT**
   - Download from: https://www.transmissionzero.co.uk/software/freeglut-devel/
   - Extract `glut32.lib` and `glut32.dll` to project directory

3. **Configure Visual Studio Project**
   - Open `OpenGL3DTemplate.sln` in Visual Studio
   - Right-click project → Properties
   - Configuration Properties → C/C++ → General → Additional Include Directories:
     - Add: `external\assimp-install\include`
   - Configuration Properties → Linker → General → Additional Library Directories:
     - Add: `external\assimp-install\lib`
   - Configuration Properties → Linker → Input → Additional Dependencies:
     - Add: `assimp-vc143-mt.lib;glut32.lib`

4. **Copy DLLs**
   - Copy `assimp-vc143-mt.dll` from Assimp's bin directory to project root
   - Copy `glut32.dll` to project root

5. **Build**
   - Press F7 to build
   - Press F5 to run

### Option 3: CMake on Windows

```cmd
# Install dependencies with vcpkg first
vcpkg install assimp:x64-windows freeglut:x64-windows

# Build with CMake
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Release

# Run
.\Release\BlitzMail.exe
```

---

## Building on macOS

### Using Homebrew

```bash
# Install dependencies
brew install freeglut assimp

# Build with Make
make clean
make
./BlitzMail

# Or build with CMake
mkdir -p build && cd build
cmake ..
cmake --build .
./BlitzMail
```

### Using MacPorts

```bash
# Install dependencies
sudo port install freeglut assimp

# Build
make
./BlitzMail
```

---

## Verifying the Build

After building, verify the executable was created correctly:

```bash
# Linux/macOS
ls -lh BlitzMail
file BlitzMail
ldd BlitzMail  # Check linked libraries

# Windows
dir BlitzMail.exe
dumpbin /dependents BlitzMail.exe
```

Expected output (Linux):
```
BlitzMail: ELF 64-bit LSB executable
libGL.so.1
libGLU.so.1
libglut.so.3
libassimp.so.5
```

---

## Common Build Issues

### Issue: "assimp: No such file or directory"

**Linux:**
```bash
sudo apt-get install libassimp-dev
```

**Windows:**
Install Assimp via vcpkg or manually download from GitHub releases.

### Issue: "GL/glut.h: No such file or directory"

**Linux:**
```bash
sudo apt-get install freeglut3-dev
```

**Windows:**
Download FreeGLUT and copy headers to include directory.

### Issue: "Cannot open include file: 'assimp/Importer.hpp'"

**Visual Studio:**
- Check Additional Include Directories includes Assimp include path
- Verify Assimp is installed in expected location
- Use absolute paths if needed

### Issue: "LNK1104: cannot open file 'assimp-vc143-mt.lib'"

**Visual Studio:**
- Check Additional Library Directories includes Assimp lib path
- Verify the library name matches your Visual Studio version:
  - `assimp-vc143-mt.lib` for VS 2022
  - `assimp-vc142-mt.lib` for VS 2019
  - `assimp-vc141-mt.lib` for VS 2017

### Issue: "The program can't start because assimp-vc143-mt.dll is missing"

**Windows:**
- Copy `assimp-vc143-mt.dll` from Assimp's bin directory to:
  - Project root directory (same as .exe)
  - Or add Assimp bin directory to PATH

### Issue: CMake cannot find Assimp

```bash
# Linux: Install pkg-config
sudo apt-get install pkg-config

# Or specify Assimp location manually
cmake .. -DASSIMP_DIR=/path/to/assimp
```

---

## Running the Application

### Linux/macOS

```bash
./BlitzMail
```

**Note:** Requires X11 display server. If running on a server:
```bash
export DISPLAY=:0
./BlitzMail
```

### Windows

```cmd
# From project directory
Debug\BlitzMail.exe

# Or Release build
Release\BlitzMail.exe

# Or double-click BlitzMail.exe in File Explorer
```

---

## Clean Build

### Make

```bash
make clean
```

### CMake

```bash
cd build
cmake --build . --target clean
# Or remove build directory
cd ..
rm -rf build
```

### Visual Studio

- Build → Clean Solution

---

## Advanced Build Options

### Debug Build

```bash
# Make
make CXXFLAGS="-g -O0"

# CMake
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build .
```

### Release Build (Optimized)

```bash
# Make
make CXXFLAGS="-O3 -DNDEBUG"

# CMake
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

### Static Linking (Linux)

```bash
g++ -std=c++11 OpenGL3DTemplate.cpp -o BlitzMail -static -lGL -lGLU -lglut -lm -lassimp
```

---

## Build System Files

- **Makefile**: Linux/Unix/macOS build file using Make
- **CMakeLists.txt**: Cross-platform build file using CMake
- **OpenGL3DTemplate.vcxproj**: Visual Studio project file
- **OpenGL3DTemplate.sln**: Visual Studio solution file

---

## Dependencies Version Information

| Dependency | Minimum Version | Recommended Version | Notes |
|------------|----------------|---------------------|-------|
| GCC/G++    | 4.8            | 7.0+                | C++11 support required |
| Clang      | 3.3            | 10.0+               | C++11 support required |
| MSVC       | VS 2015        | VS 2022             | v140+ toolset |
| CMake      | 3.10           | 3.20+               | Optional |
| Assimp     | 5.0            | 5.3+                | Earlier versions not tested |
| FreeGLUT   | 3.0            | 3.4+                | GLUT 3.7+ also works |
| OpenGL     | 1.1            | 2.0+                | Higher versions provide better performance |

---

## Additional Resources

- [README.md](README.md) - Project overview and features
- [MODEL_LOADING_GUIDE.md](MODEL_LOADING_GUIDE.md) - How to load 3D models
- [VISUAL_STUDIO_ASSIMP_SETUP.md](VISUAL_STUDIO_ASSIMP_SETUP.md) - Detailed Windows setup
- [ASSIMP_INTEGRATION.md](ASSIMP_INTEGRATION.md) - Assimp integration details

---

## Getting Help

If you encounter build issues:

1. Check this document for common issues
2. Verify all dependencies are installed
3. Check compiler/library versions match minimum requirements
4. Review console output for specific error messages
5. Check the GitHub issues page for similar problems

---

**Last Updated**: December 2024
**Tested Platforms**: Ubuntu 20.04/22.04/24.04, Windows 10/11, macOS 12+
