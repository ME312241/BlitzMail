# lib3ds Integration Guide

## Overview
This document provides guidance for developers who want to integrate the full lib3ds library into the BlitzMail project for enhanced .3DS model support.

## Current Implementation vs lib3ds

### Current Implementation (ModelLoader.h)
The current implementation provides:
- ✅ Basic .3DS chunk parsing
- ✅ Vertex loading (chunk 0x4110)
- ✅ Face loading (chunk 0x4120)
- ✅ Texture coordinate loading (chunk 0x4140)
- ✅ Automatic normal calculation
- ✅ Cross-platform compatibility
- ✅ No external dependencies

Limitations:
- ❌ No material support
- ❌ Single object per file
- ❌ No animation data
- ❌ No hierarchy/transformations
- ❌ No camera/light data

### lib3ds Library
lib3ds provides:
- ✅ Complete .3DS format support
- ✅ Material definitions (ambient, diffuse, specular, texture maps)
- ✅ Multiple objects per file
- ✅ Hierarchical transformations
- ✅ Keyframe animation data
- ✅ Camera definitions
- ✅ Light definitions
- ✅ Scene graph structure

## Integration Steps

### For Linux Development

1. **Install lib3ds**
   ```bash
   sudo apt-get update
   sudo apt-get install lib3ds-dev
   ```

2. **Update Makefile or Build Command**
   ```bash
   g++ OpenGL3DTemplate.cpp -o BlitzMail \
       -I. -std=c++11 \
       -lGL -lGLU -lglut -l3ds -lm
   ```

3. **Update ModelLoader.h**
   ```cpp
   #include <lib3ds/file.h>
   #include <lib3ds/mesh.h>
   #include <lib3ds/material.h>
   
   bool load3DSModelWithLib3ds(const char* filename, Model& model) {
       Lib3dsFile *file = lib3ds_file_load(filename);
       if (!file) return false;
       
       // Iterate through meshes
       for (Lib3dsMesh *mesh = file->meshes; mesh != NULL; mesh = mesh->next) {
           Mesh ourMesh;
           
           // Copy vertices
           for (int i = 0; i < mesh->points; i++) {
               ourMesh.vertices.push_back(Vector3(
                   mesh->pointL[i].pos[0],
                   mesh->pointL[i].pos[1],
                   mesh->pointL[i].pos[2]
               ));
           }
           
           // Copy faces and calculate normals
           for (int i = 0; i < mesh->faces; i++) {
               Lib3dsFace *face = &mesh->faceL[i];
               // Process face indices...
           }
           
           model.meshes.push_back(ourMesh);
       }
       
       lib3ds_file_free(file);
       return true;
   }
   ```

### For Windows/Visual Studio Development

1. **Download lib3ds**
   - Download from: https://github.com/AlexMarlo/lib3ds
   - Or use vcpkg: `vcpkg install lib3ds`

2. **Add to Visual Studio Project**
   
   a. **Include Directories**
      - Right-click project → Properties → C/C++ → General
      - Add: `C:\path\to\lib3ds\include`
   
   b. **Library Directories**
      - Properties → Linker → General
      - Add: `C:\path\to\lib3ds\lib`
   
   c. **Linker Input**
      - Properties → Linker → Input
      - Add to Additional Dependencies: `3ds.lib`

3. **Copy DLL**
   - Copy `3ds.dll` to your Debug/Release output directory

4. **Update OpenGL3DTemplate.vcxproj**
   ```xml
   <AdditionalIncludeDirectories>
       $(OutputPath)\..;
       C:\path\to\lib3ds\include;
       %(AdditionalIncludeDirectories)
   </AdditionalIncludeDirectories>
   ```
   ```xml
   <AdditionalDependencies>
       glut32.lib;
       3ds.lib;
       %(AdditionalDependencies)
   </AdditionalDependencies>
   ```

## Example: Complete lib3ds Integration

### Updated ModelLoader.h (with lib3ds)

```cpp
#ifdef USE_LIB3DS
#include <lib3ds/file.h>
#include <lib3ds/mesh.h>
#include <lib3ds/material.h>

bool load3DSModelWithLib3ds(const char* filename, Model& model) {
    Lib3dsFile *file = lib3ds_file_load(filename);
    if (!file) {
        printf("Warning: Could not load 3DS file with lib3ds: %s\n", filename);
        return false;
    }
    
    printf("Loading 3DS file: %s\n", filename);
    printf("  Meshes: %d\n", file->nmeshes);
    printf("  Materials: %d\n", file->nmaterials);
    
    // Process each mesh
    for (Lib3dsMesh *mesh = file->meshes; mesh != NULL; mesh = mesh->next) {
        Mesh ourMesh;
        
        printf("  Processing mesh: %s (%d vertices, %d faces)\n", 
               mesh->name, mesh->points, mesh->faces);
        
        // Store original vertices
        std::vector<Vector3> vertices;
        for (int i = 0; i < mesh->points; i++) {
            vertices.push_back(Vector3(
                mesh->pointL[i].pos[0],
                mesh->pointL[i].pos[1],
                mesh->pointL[i].pos[2]
            ));
        }
        
        // Store texture coordinates
        std::vector<Vector2> texcoords;
        if (mesh->texels > 0) {
            for (int i = 0; i < mesh->texels; i++) {
                texcoords.push_back(Vector2(
                    mesh->texelL[i][0],
                    mesh->texelL[i][1]
                ));
            }
        }
        
        // Process faces
        for (int i = 0; i < mesh->faces; i++) {
            Lib3dsFace *face = &mesh->faceL[i];
            
            // Get vertices
            Vector3 v1 = vertices[face->points[0]];
            Vector3 v2 = vertices[face->points[1]];
            Vector3 v3 = vertices[face->points[2]];
            
            // Calculate normal
            Vector3 edge1(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);
            Vector3 edge2(v3.x - v1.x, v3.y - v1.y, v3.z - v1.z);
            Vector3 normal(
                edge1.y * edge2.z - edge1.z * edge2.y,
                edge1.z * edge2.x - edge1.x * edge2.z,
                edge1.x * edge2.y - edge1.y * edge2.x
            );
            
            // Normalize
            float len = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
            if (len > 0.0001f) {
                normal.x /= len; normal.y /= len; normal.z /= len;
            }
            
            // Add vertices and normals
            ourMesh.vertices.push_back(v1);
            ourMesh.vertices.push_back(v2);
            ourMesh.vertices.push_back(v3);
            
            ourMesh.normals.push_back(normal);
            ourMesh.normals.push_back(normal);
            ourMesh.normals.push_back(normal);
            
            // Add texture coordinates if available
            if (texcoords.size() > 0) {
                for (int j = 0; j < 3; j++) {
                    int idx = face->points[j];
                    if (idx < texcoords.size()) {
                        ourMesh.texCoords.push_back(texcoords[idx]);
                    } else {
                        ourMesh.texCoords.push_back(Vector2(0, 0));
                    }
                }
            }
        }
        
        // Load material/texture
        if (mesh->faceL[0].material >= 0 && mesh->faceL[0].material < file->nmaterials) {
            Lib3dsMaterial *material = file->materials[mesh->faceL[0].material];
            if (material && material->texture1_map.name[0]) {
                char texPath[256];
                sprintf(texPath, "models/%s", material->texture1_map.name);
                ourMesh.textureID = loadTexture(texPath);
            }
        }
        
        ourMesh.materialName = mesh->name;
        model.meshes.push_back(ourMesh);
    }
    
    lib3ds_file_free(file);
    printf("Loaded 3DS model with lib3ds: %s (%d meshes)\n", 
           filename, (int)model.meshes.size());
    return true;
}

// In load3DSModel function, add option to use lib3ds:
bool load3DSModel(const char* filename, Model& model) {
#ifdef USE_LIB3DS
    return load3DSModelWithLib3ds(filename, model);
#else
    // Use the built-in basic parser
    // ... existing code ...
#endif
}
#endif // USE_LIB3DS
```

### Enabling lib3ds in Build

**Linux:**
```bash
g++ -DUSE_LIB3DS OpenGL3DTemplate.cpp -o BlitzMail \
    -I. -std=c++11 -lGL -lGLU -lglut -l3ds -lm
```

**Visual Studio:**
- Add `USE_LIB3DS` to Preprocessor Definitions

## Material and Texture Support

With lib3ds, you can access material properties:

```cpp
Lib3dsMaterial *mat = file->materials[materialIndex];

// Colors
GLfloat ambient[] = {mat->ambient[0], mat->ambient[1], mat->ambient[2], 1.0};
GLfloat diffuse[] = {mat->diffuse[0], mat->diffuse[1], mat->diffuse[2], 1.0};
GLfloat specular[] = {mat->specular[0], mat->specular[1], mat->specular[2], 1.0};

glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
glMaterialf(GL_FRONT, GL_SHININESS, mat->shininess);

// Texture
if (mat->texture1_map.name[0]) {
    GLuint texID = loadTexture(mat->texture1_map.name);
    glBindTexture(GL_TEXTURE_2D, texID);
}
```

## Animation Support

lib3ds also provides animation data:

```cpp
// Get object transformation at specific frame
Lib3dsNode *node = lib3ds_file_node_by_name(file, "ObjectName");
float matrix[4][4];
lib3ds_node_eval(node, frameNumber, matrix);

// Apply transformation in OpenGL
glPushMatrix();
glMultMatrixf((GLfloat*)matrix);
renderObject();
glPopMatrix();
```

## Benefits of lib3ds Integration

1. **Complete Format Support**: Access all .3DS features
2. **Material System**: Automatic material and texture loading
3. **Multiple Objects**: Handle complex scenes
4. **Animation**: Support for animated models
5. **Industry Standard**: Well-tested, widely used library

## Backward Compatibility

The current basic parser will remain as a fallback:
- Works without external dependencies
- Handles simple .3DS files
- Cross-platform with zero configuration
- Good for development and testing

Use lib3ds when:
- Models have complex materials
- Multiple objects per file
- Animation is needed
- Production/release builds

## Testing

After integration, test with:
```bash
# Verify lib3ds is working
./BlitzMail
# Check console for "Loaded 3DS model with lib3ds" messages

# Test with complex models
# Models with multiple objects
# Models with textures
# Models with transparency
```

## Troubleshooting

**Linux:**
- `cannot find -l3ds`: Install lib3ds-dev
- `lib3ds/file.h: No such file`: Check include paths

**Windows:**
- `unresolved external symbol`: Link 3ds.lib
- `cannot open file '3ds.dll'`: Copy DLL to output directory

## References

- lib3ds GitHub: https://github.com/AlexMarlo/lib3ds
- 3DS Format Specification: http://www.martinreddy.net/gfx/3d/3DS.spec
- lib3ds Documentation: https://lib3ds.sourceforge.net/

---

For questions about the basic built-in parser, see MODEL_LOADING_GUIDE.md
