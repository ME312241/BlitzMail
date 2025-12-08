# BlitzMail - Rural Mail Delivery Game

A 3D OpenGL game featuring a rural mail delivery scenario with a mailman character navigating through a countryside environment.

## 🎮 Features

- **3D Rural Environment**: Houses, trees, rocks, fences, crops, and street lamps
- **Playable Mailman Character**: Loaded from Blender file (Player.blend)
- **Package Collection**: Collect packages scattered throughout the rural landscape
- **Dynamic Lighting**: Day/night cycle with sun and street lamp lighting
- **Camera Modes**: First-person and third-person view
- **Movement System**: WASD movement, jumping, and crouching
- **Multiple 3D Model Formats**: Support for .blend, .3ds, .obj, .fbx, and more via Assimp

## 🚀 Getting Started

### Prerequisites

- **C++ Compiler**: GCC, Clang, or Visual Studio
- **OpenGL**: Version 1.1 or higher
- **GLUT/FreeGLUT**: For window management and input
- **Assimp 5.x**: For 3D model loading

### Building on Linux

```bash
# Install dependencies (Ubuntu/Debian)
sudo apt-get update
sudo apt-get install -y build-essential freeglut3-dev libassimp-dev

# Build with Make
make
./BlitzMail

# Or build with CMake
mkdir build && cd build
cmake ..
cmake --build .
./BlitzMail
```

### Building on Windows

#### Option 1: Visual Studio with vcpkg
```cmd
# Install vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg integrate install

# Install dependencies
.\vcpkg install assimp:x86-windows freeglut:x86-windows

# Open OpenGL3DTemplate.sln in Visual Studio and build
```

#### Option 2: Manual Setup
See [VISUAL_STUDIO_ASSIMP_SETUP.md](VISUAL_STUDIO_ASSIMP_SETUP.md) for detailed instructions.

### Building on macOS

```bash
# Install dependencies
brew install freeglut assimp

# Build
make
./BlitzMail
```

## 🎯 Controls

- **W/A/S/D** - Move forward/left/backward/right
- **Mouse** - Look around
- **Space** - Jump
- **C** - Crouch
- **V** - Toggle camera (first-person/third-person)
- **ESC** - Exit

## 📁 Project Structure

```
BlitzMail/
├── OpenGL3DTemplate.cpp    # Main game code
├── ModelLoader.h            # 3D model loading (Assimp integration)
├── glut.h                   # GLUT header
├── Makefile                 # Linux/Unix build file
├── CMakeLists.txt           # Cross-platform CMake build
├── OpenGL3DTemplate.vcxproj # Visual Studio project
├── models/                  # 3D model files
│   ├── 98-hikerbasemesh/
│   │   └── Player.blend     # Mailman character
│   ├── 1elmla01hh-Rock1_BYTyroSmith/
│   │   └── Rock1/Rock1.blend
│   ├── s3duldjjt9fk-StreetLampByTyroSmith/
│   │   └── Street Lamp/StreetLamp.blend
│   └── ... (other models)
└── Documentation
    ├── ASSIMP_INTEGRATION.md
    ├── MODEL_LOADING_GUIDE.md
    └── VISUAL_STUDIO_ASSIMP_SETUP.md
```

## 🛠️ Technical Details

### 3D Model Loading with Assimp

The project uses **Assimp (Open Asset Import Library)** version 5.x to load 3D models. This provides support for:

- **.blend** - Blender files (primary format for characters and objects)
- **.3ds** - 3D Studio Max files
- **.obj** - Wavefront OBJ files
- **.fbx** - Autodesk FBX
- **.dae** - Collada
- And 50+ other formats

Key features:
- Automatic triangulation
- Normal generation
- Mesh optimization
- Material and texture loading
- Fallback to primitive rendering if models fail to load

### Models in Use

1. **Player** - Mailman character from Player.blend
2. **Rocks** - Various rock formations from .blend files
3. **Street Lamps** - Lighting objects from .blend files
4. **Fences** - Boundary fences from .blend files
5. **Houses** - Farmhouses from Maya .obj files
6. **Vegetation** - Trees, wheat, carrots
7. **Primitives** - Mail bag and postal items rendered with OpenGL primitives

### Rendering

- **OpenGL 1.1+** with immediate mode rendering
- **Lighting**: Dynamic day/night cycle, directional sun light, point lights for lamps
- **Materials**: Colored primitives and textured models
- **Camera**: Perspective projection with adjustable view

## 📚 Documentation

- [ASSIMP_INTEGRATION.md](ASSIMP_INTEGRATION.md) - Complete guide to Assimp integration
- [MODEL_LOADING_GUIDE.md](MODEL_LOADING_GUIDE.md) - How to load and use 3D models
- [VISUAL_STUDIO_ASSIMP_SETUP.md](VISUAL_STUDIO_ASSIMP_SETUP.md) - Windows/Visual Studio setup
- [LIB3DS_INTEGRATION.md](LIB3DS_INTEGRATION.md) - Legacy lib3ds integration (optional)

## 🎨 Game Objectives

1. Navigate through the rural environment
2. Collect all 5 packages scattered across the map
3. Explore houses, trees, rocks, and farmland
4. Experience the day/night cycle

## 🔧 Troubleshooting

### Model Not Loading
```
Warning: Assimp failed to load file: models/...
```
- Verify the model file exists
- Check file path (case-sensitive on Linux)
- Ensure Assimp supports the file format
- Check console for detailed error messages

### Build Errors

**Linux: "assimp: No such file or directory"**
```bash
sudo apt-get install libassimp-dev
```

**Windows: "Cannot open include file: 'assimp/Importer.hpp'"**
- Install Assimp via vcpkg or manually
- Configure include directories in Visual Studio
- See VISUAL_STUDIO_ASSIMP_SETUP.md

### Runtime Issues

**"Cannot find assimp-*.dll"** (Windows)
- Copy the Assimp DLL to your executable directory
- Or add Assimp bin directory to PATH

**"freeglut: failed to open display"** (Linux without X)
- Requires X11 display server
- Cannot run in headless environments

## 🚧 Known Limitations

1. Texture loading limited to BMP format (via custom loader)
2. Material colors are hardcoded (not using Assimp material data yet)
3. No animation playback (Assimp loads animation data but not used)
4. Immediate mode OpenGL (not using modern VBO/VAO)

## 🎯 Future Enhancements

- [ ] Apply Assimp material colors
- [ ] Support more texture formats (JPG, PNG via stb_image)
- [ ] Skeletal animation playback
- [ ] Modern OpenGL with shaders
- [ ] Sound effects
- [ ] More gameplay elements

## 📝 License

This is an educational project. Model files in the `models/` directory may have their own licenses.

## 🙏 Acknowledgments

- **Assimp** - Open Asset Import Library
- **FreeGLUT** - OpenGL Utility Toolkit
- Model creators from various sources (see model directories for credits)

## 📧 Contact

For questions or issues, please refer to the documentation or check the console output for error messages.

---

**Last Updated**: December 2024  
**Assimp Version**: 5.3.0+  
**Platform Support**: Linux, Windows, macOS
