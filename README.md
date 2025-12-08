# BlitzMail - Rural Mail Delivery Game

A 3D mail delivery game built with OpenGL and enhanced with **Assimp** for multi-format 3D model loading.

![BlitzMail](University%20logo.png)

## 🎮 Game Overview

Play as a mailman exploring a scenic rural countryside, collecting packages while experiencing dynamic day/night cycles and realistic 3D environments.

### Features
- ✅ **3D Model Loading** via Assimp (supports .blend, .obj, .3ds, .fbx, .ma/.mb, and more!)
- ✅ **Player Model**: Loaded from Blender file (`Player.blend`)
- ✅ **Dynamic Lighting**: Rotating sun with day/night cycle
- ✅ **First/Third Person Camera**: Toggle between viewing modes
- ✅ **Physics**: Gravity, jumping, and smooth movement
- ✅ **Collectibles**: 5 packages to find around the rural scene
- ✅ **Detailed Environment**: Trees, houses, fences, rocks, crops, and more

## 🚀 Quick Start

### Linux

```bash
# 1. Setup Assimp (one-time)
./setup_assimp.sh

# 2. Build the game
make

# 3. Test model loading (optional)
./test_assimp

# 4. Play!
./BlitzMail
```

### Windows

1. Follow the instructions in [ASSIMP_SETUP.md](ASSIMP_SETUP.md) to get Assimp
2. Open `OpenGL3DTemplate.sln` in Visual Studio
3. Build (F7) and Run (F5)

## 🎯 Controls

| Key/Input | Action |
|-----------|--------|
| **W** | Move Forward |
| **A** | Move Left |
| **S** | Move Backward |
| **D** | Move Right |
| **Space** | Jump |
| **C** | Crouch/Stand |
| **V** | Toggle Camera View |
| **Mouse** | Look Around |
| **ESC** | Exit Game |

## 📦 3D Model Support

BlitzMail uses **Assimp 5.4.3** to load 3D models in multiple formats:

### Currently Loaded Models
- **Player Model** (Player.blend) - 32,153 vertices
- **Street Lamps** (StreetLamp.blend) - Functional lighting
- **Fences** (cerca.blend) - Rural boundaries
- **Rocks** (Rock1.blend, RockSet.blend) - Environmental detail
- **Farmhouse** (farmhouse_obj.obj) - From Maya
- **Trees** (Tree1.3ds, Tree.obj) - Multiple formats
- **Crops** (Wheat, Carrots) - OBJ format

### Supported Formats
✅ Blender (.blend)  
✅ Wavefront OBJ (.obj)  
✅ 3D Studio (.3ds)  
✅ Autodesk FBX (.fbx)  
✅ Maya (.ma, .mb)  
✅ COLLADA (.dae)  
✅ And many more!

See [ASSIMP_INTEGRATION.md](ASSIMP_INTEGRATION.md) for details.

## 🛠️ Building from Source

### Prerequisites

#### Linux
```bash
# Install OpenGL and GLUT
sudo apt-get update
sudo apt-get install -y freeglut3-dev libgl1-mesa-dev libglu1-mesa-dev

# Assimp will be built automatically by setup_assimp.sh
```

#### Windows
- Visual Studio 2019 or later
- GLUT library (glut32.lib/dll)
- Assimp (see ASSIMP_SETUP.md)

#### macOS
```bash
brew install freeglut
# Then follow Linux build steps
```

### Build Steps

**Linux/macOS:**
```bash
./setup_assimp.sh  # First time only
make
./BlitzMail
```

**Windows:**
1. Run setup_assimp.sh in Git Bash or download Assimp manually
2. Open OpenGL3DTemplate.sln
3. Build and run from Visual Studio

## 📚 Documentation

- **[ASSIMP_INTEGRATION.md](ASSIMP_INTEGRATION.md)** - Complete Assimp integration guide
- **[ASSIMP_SETUP.md](ASSIMP_SETUP.md)** - Download and installation instructions
- **[QUICK_START.md](QUICK_START.md)** - Game controls and features
- **[IMPLEMENTATION_NOTES.md](IMPLEMENTATION_NOTES.md)** - Technical details
- **[SUMMARY.md](SUMMARY.md)** - Project overview

## 🧪 Testing

Verify that all models load correctly:

```bash
# Linux/macOS
./test_assimp

# Expected output:
# ===== Assimp Model Loading Test =====
# Testing: models/98-hikerbasemesh/Player.blend... SUCCESS
# Testing: models/.../cerca.blend... SUCCESS
# ...
# All tests PASSED! ✓
```

## 📁 Project Structure

```
BlitzMail/
├── OpenGL3DTemplate.cpp       # Main game code
├── ModelLoader.h              # Assimp-based model loader
├── Makefile                   # Linux build system
├── setup_assimp.sh           # Assimp installation script
├── test_assimp.cpp           # Model loading tests
├── models/                    # 3D model files
│   ├── 98-hikerbasemesh/     # Player.blend
│   ├── 6od9waw1za0w-fence/   # cerca.blend
│   └── ...                    # Other model directories
└── external/                  # External libraries (not in repo)
    └── assimp-install/       # Assimp installation
```

## 🎨 Adding New Models

1. Place your model in the `models/` directory (any supported format)
2. Declare a Model variable:
   ```cpp
   Model myModel;
   ```
3. Load in `loadAllModels()`:
   ```cpp
   if (loadModel("models/path/to/model.blend", myModel)) {
       myModel.scale = 1.0f;
   }
   ```
4. Render in `display()`:
   ```cpp
   glPushMatrix();
   glTranslatef(x, y, z);
   renderModel(myModel);
   glPopMatrix();
   ```

## 🔧 Troubleshooting

### "Cannot find Assimp"
- Run `./setup_assimp.sh` to download and build Assimp
- Or see [ASSIMP_SETUP.md](ASSIMP_SETUP.md) for manual installation

### "Models won't load"
- Run `./test_assimp` to diagnose issues
- Check file paths in OpenGL3DTemplate.cpp
- Verify model files exist in the models/ directory

### Build errors
- Ensure all prerequisites are installed
- Check that external/assimp-install exists
- Review compiler error messages

### Black screen
- Update graphics drivers
- Try toggling camera mode with 'V'
- Check OpenGL support

## 🎯 Game Objectives

- Collect all 5 packages scattered around the rural scene
- Explore the environment with houses, trees, fences, and crops
- Experience the day/night cycle
- Enjoy the peaceful countryside atmosphere

## 🚀 Performance

- **60+ FPS** on modern hardware
- Efficient model loading at startup
- Optimized rendering pipeline
- Texture caching to minimize memory usage

## 📝 Technical Highlights

- **Assimp Integration**: Professional-grade 3D model loading
- **Multi-format Support**: Blender, Maya, OBJ, 3DS, FBX, and more
- **Hybrid Rendering**: Models with primitive decorations
- **Automatic Fallback**: Primitives if models fail to load
- **Texture Support**: BMP texture loading with caching

## 🤝 Contributing

Contributions are welcome! Areas for improvement:
- Additional model formats
- Advanced texture support (PNG, JPG)
- Animation system using Assimp
- VBO/VAO for better performance
- Additional gameplay features

## 📜 License

See individual model directories for asset licenses.  
Assimp is licensed under the BSD 3-Clause License.

## 🙏 Credits

- **Assimp Library**: https://assimp.org/
- **OpenGL & GLUT**: Standard graphics libraries
- **3D Models**: Various contributors (see model directories)

## 📞 Support

- Check documentation in the ASSIMP_*.md files
- Review test output: `./test_assimp`
- Verify model paths and formats
- Ensure Assimp is properly installed

---

**Enjoy delivering mail in the countryside!** 📬🚶‍♂️🌳
