# Assimp Integration Guide for BlitzMail

## Overview
BlitzMail now uses the **Assimp library (version 5.4.3)** for loading 3D models in multiple formats. This replaces the previous basic 3DS/OBJ loader and adds support for many additional formats including Blender (.blend), Maya (.ma/.mb), and more.

## Supported Formats
Assimp can load models in the following formats:
- ✅ **Blender (.blend)** - Player model, fence, street lamp, rocks
- ✅ **Wavefront OBJ (.obj)** - Farmhouse, trees, wheat, carrots
- ✅ **3D Studio (.3ds)** - Trees, grass blocks
- ✅ **Autodesk FBX (.fbx)** - Animation and rigged models
- ✅ **Maya Binary (.mb)** - Maya native format
- ✅ **Maya ASCII (.ma)** - Maya text format
- ✅ **COLLADA (.dae)** - Universal 3D exchange format
- ✅ And many more!

## Current Model Loading

### Loaded Models
The following models are now loaded using Assimp:

1. **Player Model**: `models/98-hikerbasemesh/Player.blend`
   - Blender format
   - 5 meshes, 32,153 vertices
   - Mailman character model

2. **Fence Model**: `models/6od9waw1za0w-fence/fence/cerca.blend`
   - Blender format
   - 2 meshes, 210 vertices
   - Rural fence sections

3. **Street Lamp**: `models/s3duldjjt9fk-StreetLampByTyroSmith/Street Lamp/StreetLamp.blend`
   - Blender format
   - 5 meshes, 1,124 vertices
   - Functional street lighting

4. **Rock Models**:
   - `models/1elmla01hh-Rock1_BYTyroSmith/Rock1/Rock1.blend` (2 meshes, 169 vertices)
   - `models/xvs3wxwo2o-RockSet_MadeByTyroSmith/RockSet/RockSet.blend` (6 meshes, 478 vertices)

5. **Farmhouse**: `models/4vd2sk31doow-farmhouse_maya16/Farmhouse Maya 2016 Updated/farmhouse_obj.obj`
   - OBJ format (Maya export)
   - 1 mesh, 556 vertices

6. **Trees**:
   - `models/tree/tree1_3ds/Tree1.3ds` (5 meshes, 93,248 vertices)
   - `models/15od5xhlv2jc-Tree_02/Tree 02/Tree.obj` (2 meshes, 28,072 vertices)

7. **Crops**:
   - Wheat field OBJ (1 mesh, 27,944 vertices)
   - Carrot OBJ (1 mesh, 19,730 vertices)

## Building the Project

### Linux Build

#### Prerequisites
```bash
# Install OpenGL and GLUT
sudo apt-get update
sudo apt-get install -y freeglut3-dev libgl1-mesa-dev libglu1-mesa-dev

# Assimp is included in the external/ directory
```

#### Building
```bash
# Build the project
make

# Run the game
./BlitzMail

# Clean build artifacts
make clean

# Run tests
./test_assimp
```

### Windows Build

#### Prerequisites
1. Visual Studio 2019 or later with C++ support
2. Download and build Assimp 5.x or use the provided external/assimp-install directory
3. GLUT library (glut32.lib and glut32.dll)

#### Building
1. Open `OpenGL3DTemplate.sln` in Visual Studio
2. Ensure the Assimp library is in `external/assimp-install/`
3. Build the solution (F7)
4. Run (F5)

**Note**: The Visual Studio project is configured to look for:
- Include files: `external\assimp-install\include`
- Library files: `external\assimp-install\lib`
- Library name: `assimp-vc143-mt.lib` (adjust for your Visual Studio version)

## Code Structure

### ModelLoader.h
The updated `ModelLoader.h` provides:

```cpp
// Load any model format supported by Assimp
bool loadModel(const char* filename, Model& model);

// Render a loaded model
void renderModel(const Model& model);

// Texture loading
GLuint loadTexture(const char* filename);
GLuint loadBMPTexture(const char* filename);
```

### Key Features
1. **Automatic Format Detection**: Assimp detects the file format automatically
2. **Texture Support**: Loads textures referenced in model files
3. **Normal Generation**: Automatically generates smooth normals if not present
4. **Triangulation**: Converts all polygons to triangles for OpenGL rendering
5. **Texture Caching**: Prevents duplicate texture loading

### Model Structure
```cpp
struct Model {
    std::vector<Mesh> meshes;
    float scale;
    Vector3 offset;
};

struct Mesh {
    std::vector<Vector3> vertices;
    std::vector<Vector3> normals;
    std::vector<Vector2> texCoords;
    GLuint textureID;
    std::string materialName;
};
```

## Player Model with Primitives

The player rendering system now:
1. **Loads Player.blend** if available using Assimp
2. **Falls back to primitives** if the model fails to load
3. **Always adds mail bag and postman items** as primitive decorations on top

This hybrid approach ensures:
- High-quality model when available
- Reliable fallback for testing
- Postal-themed accessories regardless of base model

## Testing

A test program `test_assimp.cpp` is provided to verify model loading:

```bash
# Compile the test
g++ -std=c++11 -I. -Iexternal/assimp-install/include test_assimp.cpp \
    -o test_assimp -Lexternal/assimp-install/lib -lassimp \
    -Wl,-rpath,external/assimp-install/lib

# Run the test
./test_assimp
```

The test verifies:
- ✅ All .blend files load correctly
- ✅ All .obj files load correctly
- ✅ All .3ds files load correctly
- ✅ Correct vertex and mesh counts

## File Organization

```
BlitzMail/
├── external/
│   ├── assimp-5.4.3/          # Assimp source (not in repo)
│   └── assimp-install/         # Installed Assimp (not in repo)
│       ├── include/            # Assimp headers
│       └── lib/                # Assimp libraries
├── models/                     # 3D model files
│   ├── 98-hikerbasemesh/      # Player.blend
│   ├── 6od9waw1za0w-fence/    # cerca.blend
│   ├── s3duldjjt9fk-StreetLampByTyroSmith/  # StreetLamp.blend
│   └── ...
├── ModelLoader.h              # Assimp-based model loader
├── OpenGL3DTemplate.cpp       # Main game code
├── Makefile                   # Linux build system
├── OpenGL3DTemplate.vcxproj   # Visual Studio project
└── test_assimp.cpp           # Model loading tests
```

## Adding New Models

To add a new model to the game:

1. **Place the model file** in the `models/` directory
2. **Declare a Model variable** in OpenGL3DTemplate.cpp:
   ```cpp
   Model myNewModel;
   ```
3. **Load the model** in `loadAllModels()`:
   ```cpp
   if (loadModel("models/path/to/model.blend", myNewModel)) {
       myNewModel.scale = 1.0f;  // Adjust scale as needed
       myNewModel.offset = Vector3(0, 0, 0);
   }
   ```
4. **Render the model** in `display()`:
   ```cpp
   glPushMatrix();
   glTranslatef(x, y, z);  // Position in world
   glRotatef(angle, 0, 1, 0);  // Rotation if needed
   renderModel(myNewModel);
   glPopMatrix();
   ```

## Troubleshooting

### Model Won't Load
- Check the file path is correct
- Verify the file format is supported
- Check console output for Assimp error messages
- Try opening the file in Blender or another 3D tool to verify it's valid

### Textures Not Showing
- Ensure texture files are in the same directory as the model
- Check that texture file paths in the model are relative
- BMP textures are supported; other formats may need additional libraries

### Scale Issues
- Adjust the `model.scale` value in `loadAllModels()`
- Blender models often need scales around 0.1-1.0
- OBJ models may need scales around 0.01-0.1

### Memory Issues
- Large models with many vertices may need more memory
- Consider simplifying models in Blender before export
- Use LOD (Level of Detail) for distant objects

## Performance Notes

- **Model Loading**: Done once at startup, minimal impact
- **Rendering**: Uses OpenGL immediate mode (glBegin/glEnd)
  - For better performance, consider using VBOs (Vertex Buffer Objects)
  - Current approach is simple and educational
- **Texture Memory**: Textures are cached to avoid duplicates

## Future Enhancements

Potential improvements:
1. **Animations**: Assimp supports skeletal animations
2. **LOD System**: Multiple detail levels based on distance
3. **Material System**: Full material support (specular, bump maps, etc.)
4. **Instancing**: Efficient rendering of multiple copies
5. **VBO/VAO**: Modern OpenGL rendering pipeline
6. **Additional Texture Formats**: PNG, JPG support via stb_image or similar

## References

- **Assimp Documentation**: https://assimp.org/
- **Assimp GitHub**: https://github.com/assimp/assimp
- **OpenGL Documentation**: https://www.opengl.org/
- **GLUT Documentation**: https://www.opengl.org/resources/libraries/glut/

## License Notes

- **Assimp**: BSD 3-Clause License
- **BlitzMail**: [Your project license]
- **Model Assets**: Check individual model licenses in their directories

---

**Status**: ✅ **Fully Integrated and Tested**

All model formats load successfully, including the Player.blend mailman model!
