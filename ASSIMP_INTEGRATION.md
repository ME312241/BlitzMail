# Assimp Integration Guide for BlitzMail

## Overview
BlitzMail now uses the Assimp (Open Asset Import Library) version 5.x for loading 3D models. This provides support for a wide variety of 3D file formats including:

- **.blend** (Blender)
- **.3ds** (3D Studio)
- **.obj** (Wavefront)
- **.fbx** (Autodesk FBX)
- **.dae** (Collada)
- **.gltf/.glb** (GL Transmission Format)
- And 50+ other formats

## What Changed

### Player Model
- **New**: Player.blend is now loaded from `models/98-hikerbasemesh/Player.blend`
- The mailman character uses this Blender model with primitives for the mail bag and postal items

### Rock Models
- **Updated**: Now uses `.blend` files:
  - Rock1: `models/1elmla01hh-Rock1_BYTyroSmith/Rock1/Rock1.blend`
  - RockSet: `models/xvs3wxwo2o-RockSet_MadeByTyroSmith/RockSet/RockSet.blend`

### Street Lamp
- **Updated**: Now uses `.blend` file:
  - `models/s3duldjjt9fk-StreetLampByTyroSmith/Street Lamp/StreetLamp.blend`

### Fence
- **New**: Fence model from `.blend`:
  - `models/6od9waw1za0w-fence/fence/cerca.blend`

## Building the Project

### Linux/Unix

#### Install Dependencies
```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install -y build-essential freeglut3-dev libassimp-dev

# Fedora/RHEL
sudo dnf install gcc-c++ freeglut-devel assimp-devel

# Arch Linux
sudo pacman -S base-devel freeglut assimp
```

#### Build with Make
```bash
make
./BlitzMail
```

Or use the Makefile targets:
```bash
make install-deps  # Install dependencies (Ubuntu/Debian only)
make               # Build
make run           # Build and run
make clean         # Clean build artifacts
```

#### Build with CMake
```bash
mkdir build
cd build
cmake ..
cmake --build .
./BlitzMail
```

### Windows with Visual Studio

#### Install Assimp
You have several options:

**Option 1: vcpkg (Recommended)**
```cmd
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg integrate install
.\vcpkg install assimp:x86-windows
```

**Option 2: Pre-built Binaries**
1. Download Assimp 5.x from: https://github.com/assimp/assimp/releases
2. Extract to `C:\Program Files\Assimp\` or another location
3. Configure Visual Studio project:
   - Right-click project → Properties
   - C/C++ → General → Additional Include Directories: Add `C:\Program Files\Assimp\include`
   - Linker → General → Additional Library Directories: Add `C:\Program Files\Assimp\lib`
   - Linker → Input → Additional Dependencies: Add `assimp-vc142-mt.lib` (or appropriate version)
4. Copy `assimp-vc142-mt.dll` to your Debug/Release folder

#### Update Visual Studio Project
The project file needs these additions (if not using vcpkg):

**For Debug Configuration:**
```xml
<AdditionalIncludeDirectories>
  $(OutputPath)\..;
  C:\Program Files\Assimp\include;
  %(AdditionalIncludeDirectories)
</AdditionalIncludeDirectories>

<AdditionalLibraryDirectories>
  $(OutputPath)\..;
  C:\Program Files\Assimp\lib;
  %(AdditionalLibraryDirectories)
</AdditionalLibraryDirectories>

<AdditionalDependencies>
  glut32.lib;
  assimp-vc142-mt.lib;
  %(AdditionalDependencies)
</AdditionalDependencies>
```

#### Build with CMake (Windows)
```cmd
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A Win32
cmake --build . --config Debug
Debug\BlitzMail.exe
```

### macOS

#### Install Dependencies
```bash
# Using Homebrew
brew install freeglut assimp

# Using MacPorts
sudo port install freeglut assimp
```

#### Build
```bash
# With Make
make
./BlitzMail

# With CMake
mkdir build && cd build
cmake ..
make
./BlitzMail
```

## Code Architecture

### ModelLoader.h
Enhanced with Assimp support:

```cpp
// New function for Assimp-based loading
bool loadAssimpModel(const char* filename, Model& model);

// Updated loadModel() function with format detection
bool loadModel(const char* filename, Model& model) {
    // Tries Assimp first for .blend, .fbx, .dae, .gltf
    // Falls back to custom loaders for .obj and .3ds if Assimp fails
}
```

Features:
- Automatic format detection based on file extension
- Triangulation of all polygons
- Normal generation for models without normals
- UV coordinate flipping for OpenGL convention
- Mesh optimization
- Material and texture loading
- Backward compatibility with custom 3DS/OBJ loaders

### OpenGL3DTemplate.cpp
Updated model loading:

```cpp
// New model paths using .blend files
const char* MODEL_PATH_PLAYER = "models/98-hikerbasemesh/Player.blend";
const char* MODEL_PATH_ROCK1 = "models/.../Rock1.blend";
const char* MODEL_PATH_STREETLAMP = "models/.../StreetLamp.blend";
const char* MODEL_PATH_FENCE = "models/.../cerca.blend";

// Enhanced drawPlayer() function
void drawPlayer() {
    // Uses loaded Player.blend model if available
    // Falls back to primitives
    // Adds mail bag and postal items as primitives
}
```

## Supported Model Formats

### Primary Formats (via Assimp)
- **.blend** - Blender (up to Blender 2.93+)
- **.3ds** - 3D Studio Max
- **.obj** - Wavefront OBJ
- **.fbx** - Autodesk FBX
- **.dae** - Collada
- **.gltf/.glb** - GL Transmission Format

### Texture Formats
Currently supported:
- **.bmp** - Bitmap (built-in loader)

Can be extended with Assimp's embedded texture support or additional image libraries.

## Troubleshooting

### Model Not Loading
**Error**: "Assimp failed to load file"

**Solutions**:
1. Check file path is correct (case-sensitive on Linux)
2. Verify model file is not corrupted
3. Check console output for Assimp error messages
4. Try opening the model in Blender to verify it's valid
5. For .blend files, ensure Blender version compatibility

### Assimp Library Not Found
**Linux**: 
```bash
# Check if Assimp is installed
pkg-config --modversion assimp
ldconfig -p | grep assimp

# If not found, install it
sudo apt-get install libassimp-dev
```

**Windows**:
- Verify Assimp DLL is in the same directory as the executable
- Check that include paths are correct in Visual Studio
- Ensure you're using the correct architecture (x86/x64)

### Linking Errors
**Linux**:
```bash
# Verify pkg-config works
pkg-config --libs assimp

# May need to add library path
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
```

**Windows**:
- Verify .lib file matches your Visual Studio version (vc142, vc143, etc.)
- Check Debug/Release configuration matches the library variant
- Use `dumpbin /dependents BlitzMail.exe` to check dependencies

### Model Appears at Wrong Scale/Rotation
Different modeling software uses different coordinate systems and scales. Adjust in code:

```cpp
if (loadModel("path/to/model.blend", myModel)) {
    myModel.scale = 0.01f;  // Adjust as needed
    myModel.offset = Vector3(0, -1, 0);  // Adjust position
}
```

Common scale factors:
- Blender → OpenGL: 0.01 to 0.1
- 3DS Max → OpenGL: 0.01 to 0.05
- Maya → OpenGL: 0.01

### Performance Issues
If models load slowly or render poorly:

1. **Reduce vertex count**: Simplify models in modeling software
2. **Optimize meshes**: Assimp post-processing flags already optimize
3. **Use LOD**: Create lower-detail versions for distant objects
4. **Enable VBO**: Consider upgrading to VBO/VAO rendering (future enhancement)

## Model Preparation Guidelines

### For Blender Models
1. Apply all transformations (Ctrl+A → All Transforms)
2. Set proper scale (typically 1 unit = 1 meter)
3. Ensure normals are correct (Face → Normals → Recalculate Outside)
4. UV unwrap for textures
5. Export or save as .blend (Assimp can read .blend directly)

### For Textures
1. Keep textures in same directory as model or subdirectory
2. Use relative paths in model files
3. Use power-of-2 dimensions for best compatibility (256, 512, 1024, 2048)
4. Supported formats: BMP (built-in), or extend with image libraries

## Advanced Features

### Material Support
Assimp loads material properties including:
- Diffuse color and texture
- Ambient color
- Specular color and shininess
- Normal maps (future)
- Opacity/transparency

Currently, textures are loaded but material colors are defined in code. Future enhancement could use Assimp material data.

### Multiple Meshes
Models can contain multiple meshes. All meshes are loaded and rendered together as part of the Model structure.

### Animation
Assimp supports loading animation data. The current implementation does not use animations, but this is a potential future enhancement.

## Future Enhancements

1. **Material System**: Use Assimp material colors instead of hardcoded colors
2. **More Texture Formats**: Integrate stb_image or SOIL for JPG/PNG support
3. **Animation**: Implement skeletal animation support
4. **Instancing**: Efficient rendering of repeated models
5. **VBO/VAO**: Modern OpenGL rendering pipeline
6. **Shader Support**: GLSL shaders for advanced rendering effects

## References

- Assimp Documentation: https://assimp.org/
- Assimp GitHub: https://github.com/assimp/assimp
- Supported Formats: https://github.com/assimp/assimp/blob/master/doc/Fileformats.md
- OpenGL Tutorial: https://learnopengl.com/
- Blender Documentation: https://docs.blender.org/

## Getting Help

If you encounter issues:

1. Check console output for error messages
2. Verify all dependencies are installed
3. Test with simpler models first (.obj files)
4. Check Assimp version: `pkg-config --modversion assimp` (should be 5.x)
5. Refer to Assimp documentation for format-specific issues

---

**Last Updated**: December 2024
**Assimp Version**: 5.3.0+
**Supported Platforms**: Linux, Windows, macOS
