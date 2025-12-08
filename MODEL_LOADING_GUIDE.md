# BlitzMail 3D Model Loading Guide

## Overview
The BlitzMail project now supports loading and rendering 3D models from the `models/` directory using **Assimp (Open Asset Import Library)** version 5.x. This provides support for many formats including .blend, .3ds, .obj, .fbx, .dae, and 50+ others. This guide explains how the model loading system works and how to use it.

## Architecture

### ModelLoader.h
A header-only library that provides:
- **Assimp Integration**: Primary model loader supporting 50+ formats including .blend (Blender), .fbx, .dae, .gltf
- **OBJ file loading**: Full support for Wavefront OBJ format (also via Assimp)
- **.3DS file loading**: Support via Assimp, with custom fallback loader
- **Texture loading**: BMP texture loading support (built-in), plus Assimp texture support
- **Model rendering**: OpenGL rendering functions for loaded models

### Supported Formats

#### Via Assimp (Primary)
- **.blend** - Blender files (NEW!)
- **.fbx** - Autodesk FBX
- **.dae** - Collada
- **.gltf/.glb** - GL Transmission Format
- **.3ds** - 3D Studio Max
- **.obj** - Wavefront OBJ
- And 50+ other formats

#### Direct Support (Fallback)
- **.OBJ Files**: Custom parser as fallback
  - Vertices (`v x y z`)
  - Normals (`vn x y z`)
  - Texture coordinates (`vt u v`)
  - Multiple face formats

- **.3DS Files**: Custom parser as fallback
  - Basic chunk-based parsing
  - Vertices (chunk 0x4110)
  - Faces (chunk 0x4120)
  - Texture coordinates (chunk 0x4140)
  - Automatic normal calculation

#### Textures
- BMP format (24-bit RGB/BGR)
- Automatic format detection
- Linear filtering and repeat wrapping

## Model Integration

### Loaded Models
The following models are automatically loaded at startup (updated with Assimp):

1. **Player/Mailman** (`models/98-hikerbasemesh/Player.blend`) - **NEW!**
2. **Tree** (`models/tree/tree1_3ds/Tree1.3ds`)
3. **Rock** (`models/1elmla01hh-Rock1_BYTyroSmith/Rock1/Rock1.blend`) - **UPDATED!**
4. **Rock Set** (`models/xvs3wxwo2o-RockSet_MadeByTyroSmith/RockSet/RockSet.blend`) - **UPDATED!**
5. **Farmhouse** (`models/4vd2sk31doow-farmhouse_maya16/.../farmhouse_obj.obj`)
6. **Street Lamp** (`models/s3duldjjt9fk-StreetLampByTyroSmith/Street Lamp/StreetLamp.blend`) - **UPDATED!**
7. **Fence** (`models/6od9waw1za0w-fence/fence/cerca.blend`) - **NEW!**
8. **Wheat Field** (`models/10458_Wheat_Field_v1_L3.../10458_Wheat_Field_v1_max2010_it2.obj`)
9. **Carrot** (`models/Carrot_v01_l3.../10170_Carrot_v01_L3.obj`)
10. **Grass Block** (`models/grass-block/grass-block.3DS`)

### Fallback Rendering
If a model fails to load or has no geometry, the system automatically falls back to OpenGL primitive rendering (cubes, spheres, cylinders) to ensure the scene always displays correctly.

## Usage

### Loading Models
```cpp
Model myModel;
if (loadModel("models/path/to/model.3ds", myModel)) {
    myModel.scale = 0.05f;  // Set scale
    myModel.offset = Vector3(0, 0, 0);  // Set offset
}
```

### Rendering Models
```cpp
void drawObject(float x, float z) {
    glPushMatrix();
    glTranslatef(x, 0, z);
    
    if (modelsLoaded && myModel.meshes.size() > 0) {
        renderModel(myModel);
    } else {
        // Fallback primitive rendering
        glColor3f(0.5f, 0.5f, 0.5f);
        glutSolidCube(1.0f);
    }
    
    glPopMatrix();
}
```

## Enhanced Mailman Character

The mailman character has been enhanced with additional OpenGL primitives:

### Mailman Accessories
- **Mail Bag**: Brown leather bag with flap and strap
- **Metal Buckle**: Silver buckle on the bag strap
- **Letters/Envelopes**: White paper envelopes sticking out of the bag
- **Postal Badge**: Gold badge on the uniform chest
- **Cap Visor**: Blue visor on the mailman's cap

These accessories are rendered using OpenGL primitives (cubes, spheres) to complement any loaded mailman model and represent postal carrier items.

## Technical Details

### Data Structures
```cpp
struct Vector3 { float x, y, z; };
struct Vector2 { float u, v; };
struct Face { int vertexIndices[3], normalIndices[3], texCoordIndices[3]; };
struct Mesh { 
    std::vector<Vector3> vertices;
    std::vector<Vector3> normals;
    std::vector<Vector2> texCoords;
    std::vector<Face> faces;
    GLuint textureID;
    std::string materialName;
};
struct Model { 
    std::vector<Mesh> meshes;
    float scale;
    Vector3 offset;
};
```

### Model Transformations
Each model can have:
- **Scale**: Uniform scaling factor
- **Offset**: Position offset for centering
- Applied in `renderModel()` function

### Normal Calculation
For .3DS files without normals, the loader automatically calculates flat-shaded normals using cross product of face edges.

## Compatibility

### Platform Support
- **Windows**: Uses `glut.h` and Visual Studio project (see VISUAL_STUDIO_ASSIMP_SETUP.md)
- **Linux/Unix**: Uses `GL/glut.h`, make, and CMake

### Build Requirements
- OpenGL 1.1+
- GLUT or FreeGLUT
- **Assimp 5.x** (NEW!)
- C++ compiler with C++11 support
- Standard libraries: stdio.h, stdlib.h, string.h, vector

### Building with Assimp
See [ASSIMP_INTEGRATION.md](ASSIMP_INTEGRATION.md) for complete build instructions.

**Quick Start (Linux):**
```bash
sudo apt-get install libassimp-dev freeglut3-dev
make
./BlitzMail
```

**Quick Start (Windows with vcpkg):**
```cmd
vcpkg install assimp:x86-windows
# Open in Visual Studio and build
```

### Visual Studio Integration
The `ModelLoader.h` file is included in the Visual Studio project. For Assimp configuration, see `VISUAL_STUDIO_ASSIMP_SETUP.md`.

## Limitations and Future Enhancements

### Current Capabilities (with Assimp)
1. ✅ Support for 50+ 3D file formats including .blend
2. ✅ Automatic triangulation and normal generation
3. ✅ Material and texture loading
4. ✅ Multiple meshes per model
5. ✅ Optimized mesh processing

### Current Limitations
1. Texture loading limited to BMP format (custom loader) - Assimp can load more
2. Material colors are hardcoded (Assimp materials not yet applied)
3. No animation playback (Assimp loads animation data)
4. No skeletal/bone systems (Assimp supports this)

### Potential Enhancements
1. **Apply Assimp Materials**: Use loaded material colors instead of hardcoded values
2. **Image Libraries**: Add stb_image or SOIL for JPG/PNG texture loading
3. **Animation Playback**: Implement skeletal/keyframe animation using Assimp data
4. **Advanced Lighting**: Per-vertex lighting, specular maps, normal maps
5. **LOD**: Level of detail for performance optimization
6. **VBO/VAO**: Modern OpenGL rendering pipeline

## Troubleshooting

### Model Not Loading
- Check file path is correct (case-sensitive on Linux)
- Verify file format is supported by Assimp
- Look for error messages in console output
- Ensure file is not corrupted
- For .blend files, check Blender version compatibility

### Assimp-specific Issues
See [ASSIMP_INTEGRATION.md](ASSIMP_INTEGRATION.md) for detailed troubleshooting including:
- Library not found errors
- Linking errors
- DLL/shared library issues
- Format-specific problems

### Model Appears Wrong
- Adjust scale factor (try values between 0.001 and 10.0)
- Check offset values
- Verify normals are calculated correctly
- Try primitive fallback to confirm positioning
- Different modeling software uses different coordinate systems

### Texture Not Appearing
- Ensure BMP is 24-bit RGB format
- Check file path (relative to model file)
- Verify GL_TEXTURE_2D is enabled
- Confirm texture coordinates exist in model

## Examples

### Loading a Blender Model (NEW!)
```cpp
// In initialization
Model playerModel;
if (loadModel("models/98-hikerbasemesh/Player.blend", playerModel)) {
    playerModel.scale = 0.01f;  // Adjust for Blender scale
    playerModel.offset = Vector3(0, 0, 0);
}

// In rendering
void drawPlayer() {
    glPushMatrix();
    if (modelsLoaded && playerModel.meshes.size() > 0) {
        glScalef(2.0f, 2.0f, 2.0f);
        renderModel(playerModel);
    } else {
        // Fallback to primitives
    }
    glPopMatrix();
}
```

### Complete Model Loading Example
```cpp
// In initialization
Model treeModel;
if (loadModel("models/tree/tree1_3ds/Tree1.3ds", treeModel)) {
    treeModel.scale = 0.02f;
    treeModel.offset = Vector3(0, 0, 0);
}

// In rendering
void drawTree(float x, float z, float height) {
    glPushMatrix();
    glTranslatef(x, 0, z);
    
    if (modelsLoaded && treeModel.meshes.size() > 0) {
        glPushMatrix();
        glScalef(height/4.0f, height/4.0f, height/4.0f);
        renderModel(treeModel);
        glPopMatrix();
    } else {
        // Fallback rendering with primitives
        drawTreePrimitive(height);
    }
    
    glPopMatrix();
}
```

## Performance Considerations

- Models are loaded once at startup using Assimp (no per-frame loading)
- Assimp post-processing optimizes meshes (JoinIdenticalVertices, OptimizeMeshes)
- Vertex data is expanded (not indexed) for simplicity
- Immediate mode rendering (glBegin/glEnd) is used
- For production, consider VBOs/VAOs for better performance
- Assimp caching improves load times for repeated models

## Additional Resources

- **Assimp Documentation**: https://assimp.org/
- **Assimp Supported Formats**: https://github.com/assimp/assimp/blob/master/doc/Fileformats.md
- **3DS Format**: https://en.wikipedia.org/wiki/.3ds
- **OBJ Format**: http://paulbourke.net/dataformats/obj/
- **Blender Documentation**: https://docs.blender.org/
- **OpenGL Textures**: https://www.khronos.org/opengl/wiki/Texture

## See Also

- [ASSIMP_INTEGRATION.md](ASSIMP_INTEGRATION.md) - Complete Assimp integration guide
- [VISUAL_STUDIO_ASSIMP_SETUP.md](VISUAL_STUDIO_ASSIMP_SETUP.md) - Visual Studio configuration
- [LIB3DS_INTEGRATION.md](LIB3DS_INTEGRATION.md) - Legacy lib3ds integration (optional)

---

For implementation questions or issues, refer to the source code in `ModelLoader.h` and `OpenGL3DTemplate.cpp`, or consult the Assimp documentation.
