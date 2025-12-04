# BlitzMail 3D Model Loading Guide

## Overview
The BlitzMail project now supports loading and rendering .3ds and .obj 3D models from the `models/` directory. This guide explains how the model loading system works and how to use it.

## Architecture

### ModelLoader.h
A header-only library that provides:
- **OBJ file loading**: Full support for Wavefront OBJ format with vertices, normals, and texture coordinates
- **.3DS file loading**: Basic support for Autodesk 3DS format with vertices, faces, and normals
- **Texture loading**: BMP texture loading support
- **Model rendering**: OpenGL rendering functions for loaded models

### Supported Formats

#### .OBJ Files
- Vertices (`v x y z`)
- Normals (`vn x y z`)
- Texture coordinates (`vt u v`)
- Faces with multiple formats:
  - `f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3`
  - `f v1//vn1 v2//vn2 v3//vn3`
  - `f v1 v2 v3`

#### .3DS Files
- Basic chunk-based parsing
- Vertices (chunk 0x4110)
- Faces (chunk 0x4120)
- Texture coordinates (chunk 0x4140)
- Automatic normal calculation for flat shading

#### Textures
- BMP format (24-bit RGB/BGR)
- Automatic format detection
- Linear filtering and repeat wrapping

## Model Integration

### Loaded Models
The following models are automatically loaded at startup:

1. **Tree** (`models/tree/tree1_3ds/Tree1.3ds`)
2. **Rock** (`models/1elmla01hh-Rock1_BYTyroSmith/Rock1/Rock1.3ds`)
3. **Rock Set** (`models/xvs3wxwo2o-RockSet_MadeByTyroSmith/RockSet/RockSet.3ds`)
4. **Farmhouse** (`models/4vd2sk31doow-farmhouse_maya16/.../farmhouse_obj.obj`)
5. **Street Lamp** (`models/s3duldjjt9fk-StreetLampByTyroSmith/Street Lamp/StreetLamp.3ds`)
6. **Wheat Field** (`models/10458_Wheat_Field_v1_L3.../10458_Wheat_Field_v1_max2010_it2.obj`)
7. **Carrot** (`models/Carrot_v01_l3.../10170_Carrot_v01_L3.obj`)
8. **Grass Block** (`models/grass-block/grass-block.3DS`)

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
- **Windows**: Uses `glut.h` and Visual Studio project
- **Linux/Unix**: Uses `GL/glut.h` and g++/make

### Build Requirements
- OpenGL 1.1+
- GLUT or FreeGLUT
- C++ compiler with C++11 support
- Standard libraries: stdio.h, stdlib.h, string.h, vector

### Visual Studio Integration
The `ModelLoader.h` file is included in the Visual Studio project (`OpenGL3DTemplate.vcxproj`) as a ClInclude item.

## Limitations and Future Enhancements

### Current Limitations
1. .3DS loader is basic - doesn't handle all chunk types
2. Texture loading is limited to BMP format
3. No material system (colors are handled in-code)
4. No animation support
5. No skeletal/bone systems

### Potential Enhancements
1. **lib3ds Integration**: Use full-featured lib3ds library for complete .3DS support
2. **Image Libraries**: Add stb_image or SOIL for JPG/PNG texture loading
3. **Material System**: Parse and apply MTL files for OBJ models
4. **Advanced Lighting**: Per-vertex lighting, specular maps
5. **Animation**: Keyframe or skeletal animation support
6. **LOD**: Level of detail for performance optimization

## Troubleshooting

### Model Not Loading
- Check file path is correct
- Verify file format (.obj or .3ds)
- Look for error messages in console output
- Ensure file is not corrupted

### Model Appears Wrong
- Adjust scale factor (try values between 0.001 and 10.0)
- Check offset values
- Verify normals are calculated correctly
- Try primitive fallback to confirm positioning

### Texture Not Appearing
- Ensure BMP is 24-bit RGB format
- Check file path
- Verify GL_TEXTURE_2D is enabled
- Confirm texture coordinates exist in model

## Examples

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

- Models are loaded once at startup (no per-frame loading)
- Vertex data is expanded (not indexed) for simplicity
- Immediate mode rendering (glBegin/glEnd) is used
- For production, consider VBOs/VAOs for better performance

## Additional Resources

- **3DS Format**: https://en.wikipedia.org/wiki/.3ds
- **OBJ Format**: http://paulbourke.net/dataformats/obj/
- **OpenGL Textures**: https://www.khronos.org/opengl/wiki/Texture
- **lib3ds**: https://github.com/AlexMarlo/lib3ds

---

For implementation questions or issues, refer to the source code in `ModelLoader.h` and `OpenGL3DTemplate.cpp`.
