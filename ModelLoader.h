#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#ifdef _WIN32
#include <glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

// Include Assimp for advanced model loading
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#ifdef _WIN32
// Windows doesn't have strcasecmp
#define strcasecmp _stricmp
#endif

// OpenGL constants that may not be defined in older headers
#ifndef GL_BGR
#define GL_BGR 0x80E0
#endif

// Helper functions for endian conversion (3DS and BMP use little-endian)
inline unsigned short readLittleEndianShort(FILE* file) {
    unsigned char bytes[2];
    fread(bytes, 1, 2, file);
    return bytes[0] | (bytes[1] << 8);
}

inline unsigned int readLittleEndianInt(FILE* file) {
    unsigned char bytes[4];
    fread(bytes, 1, 4, file);
    return bytes[0] | (bytes[1] << 8) | (bytes[2] << 16) | (bytes[3] << 24);
}

inline float readLittleEndianFloat(FILE* file) {
    union { float f; unsigned int i; } u;
    u.i = readLittleEndianInt(file);
    return u.f;
}

// Simple 3DS model structure
struct Vector3 {
    float x, y, z;
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
};

struct Vector2 {
    float u, v;
    Vector2() : u(0), v(0) {}
    Vector2(float _u, float _v) : u(_u), v(_v) {}
};

struct Face {
    int vertexIndices[3];
    int normalIndices[3];
    int texCoordIndices[3];
};

// Note: Mesh stores expanded vertex/normal/texcoord data for simplicity
// The faces vector is available for indexed rendering if needed in the future
struct Mesh {
    std::vector<Vector3> vertices;
    std::vector<Vector3> normals;
    std::vector<Vector2> texCoords;
    std::vector<Face> faces;  // Available for future indexed rendering
    GLuint textureID;
    std::string materialName;
};

struct Model {
    std::vector<Mesh> meshes;
    float scale;
    Vector3 offset;
    
    Model() : scale(1.0f), offset(0, 0, 0) {}
};

// Texture loading functions
GLuint loadBMPTexture(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Warning: Could not open BMP file: %s\n", filename);
        return 0;
    }
    
    // Read BMP header
    unsigned char header[54];
    if (fread(header, 1, 54, file) != 54) {
        fclose(file);
        return 0;
    }
    
    // Check BMP signature
    if (header[0] != 'B' || header[1] != 'M') {
        fclose(file);
        return 0;
    }
    
    // Get image info - BMP files use little-endian byte order
    unsigned int dataPos = header[0x0A] | (header[0x0B] << 8) | (header[0x0C] << 16) | (header[0x0D] << 24);
    unsigned int imageSize = header[0x22] | (header[0x23] << 8) | (header[0x24] << 16) | (header[0x25] << 24);
    unsigned int width = header[0x12] | (header[0x13] << 8) | (header[0x14] << 16) | (header[0x15] << 24);
    unsigned int height = header[0x16] | (header[0x17] << 8) | (header[0x18] << 16) | (header[0x19] << 24);
    
    if (imageSize == 0) imageSize = width * height * 3;
    if (dataPos == 0) dataPos = 54;
    
    // Read image data - use vector for automatic memory management
    std::vector<unsigned char> data(imageSize);
    fseek(file, dataPos, SEEK_SET);
    fread(&data[0], 1, imageSize, file);
    fclose(file);
    
    // Create OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    // Upload texture data (BMP is BGR format)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, &data[0]);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    printf("Loaded BMP texture: %s (%dx%d)\n", filename, width, height);
    return textureID;
}

GLuint loadTexture(const char* filename) {
    // Check file extension
    const char* ext = strrchr(filename, '.');
    if (ext && (strcasecmp(ext, ".bmp") == 0)) {
        return loadBMPTexture(filename);
    }
    
    // For other formats (JPG, PNG), would need additional libraries
    // For now, return 0 (no texture)
    return 0;
}

// Simple 3DS loader stub
// Note: Full 3DS parsing is complex. This provides the structure.
// For production, use lib3ds or similar library.
bool load3DSModel(const char* filename, Model& model) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Warning: Could not open 3DS file: %s\n", filename);
        return false;
    }
    
    // Read basic 3DS structure
    // 3DS files use chunk-based format with little-endian byte order
    // Main chunk ID: 0x4D4D
    // Object chunk: 0x4000
    // Triangle mesh: 0x4100
    // Vertices: 0x4110
    // Faces: 0x4120
    
    // Read main chunk
    unsigned short chunkID = readLittleEndianShort(file);
    unsigned int chunkLength = readLittleEndianInt(file);
    
    if (chunkID != 0x4D4D) {
        printf("Warning: Invalid 3DS file format: %s\n", filename);
        fclose(file);
        return false;
    }
    
    Mesh mesh;
    bool hasData = false;
    
    // Parse chunks
    long fileSize = chunkLength;
    long currentPos = 6; // Already read 6 bytes
    
    while (currentPos < fileSize) {
        chunkID = readLittleEndianShort(file);
        chunkLength = readLittleEndianInt(file);
        
        long nextChunk = currentPos + chunkLength;
        
        if (chunkID == 0x4110) { // Vertices list
            unsigned short numVertices = readLittleEndianShort(file);
            
            for (int i = 0; i < numVertices; i++) {
                Vector3 vertex;
                vertex.x = readLittleEndianFloat(file);
                vertex.y = readLittleEndianFloat(file);
                vertex.z = readLittleEndianFloat(file);
                mesh.vertices.push_back(vertex);
            }
            hasData = true;
        }
        else if (chunkID == 0x4120) { // Faces description
            unsigned short numFaces = readLittleEndianShort(file);
            
            // Store original vertices temporarily
            std::vector<Vector3> originalVerts = mesh.vertices;
            mesh.vertices.clear();
            
            for (int i = 0; i < numFaces; i++) {
                unsigned short a = readLittleEndianShort(file);
                unsigned short b = readLittleEndianShort(file);
                unsigned short c = readLittleEndianShort(file);
                unsigned short flags = readLittleEndianShort(file);
                
                if (a < originalVerts.size() &&
                    b < originalVerts.size() &&
                    c < originalVerts.size()) {
                    
                    Vector3 v1 = originalVerts[a];
                    Vector3 v2 = originalVerts[b];
                    Vector3 v3 = originalVerts[c];
                    
                    // Calculate face normal
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
                        normal.x /= len;
                        normal.y /= len;
                        normal.z /= len;
                    }
                    
                    // Add expanded vertices and normals
                    mesh.vertices.push_back(v1);
                    mesh.vertices.push_back(v2);
                    mesh.vertices.push_back(v3);
                    
                    mesh.normals.push_back(normal);
                    mesh.normals.push_back(normal);
                    mesh.normals.push_back(normal);
                }
            }
            
            hasData = true;
        }
        else if (chunkID == 0x4140) { // Texture coordinates
            unsigned short numCoords = readLittleEndianShort(file);
            
            for (int i = 0; i < numCoords; i++) {
                Vector2 texCoord;
                texCoord.u = readLittleEndianFloat(file);
                texCoord.v = readLittleEndianFloat(file);
                mesh.texCoords.push_back(texCoord);
            }
        }
        
        // Seek to next chunk
        fseek(file, nextChunk, SEEK_SET);
        currentPos = nextChunk;
    }
    
    fclose(file);
    
    if (hasData && mesh.vertices.size() > 0) {
        model.meshes.push_back(mesh);
        printf("Loaded 3DS model: %s (%d vertices)\n", filename, (int)mesh.vertices.size());
        return true;
    }
    
    // Create a simple placeholder mesh if parsing failed
    printf("Warning: 3DS file loaded but no geometry found: %s\n", filename);
    return false;
}

// Simple OBJ loader
bool loadOBJModel(const char* filename, Model& model) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Warning: Could not open OBJ file: %s\n", filename);
        return false;
    }
    
    std::vector<Vector3> tempVertices;
    std::vector<Vector3> tempNormals;
    std::vector<Vector2> tempTexCoords;
    
    Mesh mesh;
    
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "v ", 2) == 0) {
            // Vertex
            Vector3 vertex;
            sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
            tempVertices.push_back(vertex);
        }
        else if (strncmp(line, "vn ", 3) == 0) {
            // Normal
            Vector3 normal;
            sscanf(line, "vn %f %f %f", &normal.x, &normal.y, &normal.z);
            tempNormals.push_back(normal);
        }
        else if (strncmp(line, "vt ", 3) == 0) {
            // Texture coordinate
            Vector2 texCoord;
            sscanf(line, "vt %f %f", &texCoord.u, &texCoord.v);
            tempTexCoords.push_back(texCoord);
        }
        else if (strncmp(line, "f ", 2) == 0) {
            // Face
            Face face;
            int matches;
            
            // Try format: f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3
            matches = sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                &face.vertexIndices[0], &face.texCoordIndices[0], &face.normalIndices[0],
                &face.vertexIndices[1], &face.texCoordIndices[1], &face.normalIndices[1],
                &face.vertexIndices[2], &face.texCoordIndices[2], &face.normalIndices[2]);
            
            if (matches != 9) {
                // Try format: f v1//vn1 v2//vn2 v3//vn3
                matches = sscanf(line, "f %d//%d %d//%d %d//%d",
                    &face.vertexIndices[0], &face.normalIndices[0],
                    &face.vertexIndices[1], &face.normalIndices[1],
                    &face.vertexIndices[2], &face.normalIndices[2]);
                
                if (matches == 6) {
                    face.texCoordIndices[0] = face.texCoordIndices[1] = face.texCoordIndices[2] = 0;
                }
            }
            
            if (matches == 0) {
                // Try format: f v1 v2 v3
                matches = sscanf(line, "f %d %d %d",
                    &face.vertexIndices[0],
                    &face.vertexIndices[1],
                    &face.vertexIndices[2]);
                
                if (matches == 3) {
                    face.normalIndices[0] = face.normalIndices[1] = face.normalIndices[2] = 0;
                    face.texCoordIndices[0] = face.texCoordIndices[1] = face.texCoordIndices[2] = 0;
                }
            }
            
            // Convert to 0-based indexing
            for (int i = 0; i < 3; i++) {
                face.vertexIndices[i]--;
                face.normalIndices[i]--;
                face.texCoordIndices[i]--;
            }
            
            // Expand face into mesh
            for (int i = 0; i < 3; i++) {
                if (face.vertexIndices[i] >= 0 && face.vertexIndices[i] < (int)tempVertices.size()) {
                    mesh.vertices.push_back(tempVertices[face.vertexIndices[i]]);
                }
                if (face.normalIndices[i] >= 0 && face.normalIndices[i] < (int)tempNormals.size()) {
                    mesh.normals.push_back(tempNormals[face.normalIndices[i]]);
                } else {
                    mesh.normals.push_back(Vector3(0, 1, 0)); // Default normal
                }
                if (face.texCoordIndices[i] >= 0 && face.texCoordIndices[i] < (int)tempTexCoords.size()) {
                    mesh.texCoords.push_back(tempTexCoords[face.texCoordIndices[i]]);
                } else {
                    mesh.texCoords.push_back(Vector2(0, 0)); // Default texcoord
                }
            }
        }
    }
    
    fclose(file);
    
    if (mesh.vertices.size() > 0) {
        model.meshes.push_back(mesh);
        printf("Loaded OBJ model: %s (%d vertices)\n", filename, (int)mesh.vertices.size());
        return true;
    }
    
    return false;
}

// Render a loaded model
void renderModel(const Model& model) {
    glPushMatrix();
    glTranslatef(model.offset.x, model.offset.y, model.offset.z);
    glScalef(model.scale, model.scale, model.scale);
    
    for (size_t m = 0; m < model.meshes.size(); m++) {
        const Mesh& mesh = model.meshes[m];
        
        if (mesh.textureID != 0) {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, mesh.textureID);
        }
        
        glBegin(GL_TRIANGLES);
        for (size_t i = 0; i < mesh.vertices.size(); i++) {
            if (i < mesh.normals.size()) {
                glNormal3f(mesh.normals[i].x, mesh.normals[i].y, mesh.normals[i].z);
            }
            if (i < mesh.texCoords.size() && mesh.textureID != 0) {
                glTexCoord2f(mesh.texCoords[i].u, mesh.texCoords[i].v);
            }
            glVertex3f(mesh.vertices[i].x, mesh.vertices[i].y, mesh.vertices[i].z);
        }
        glEnd();
        
        if (mesh.textureID != 0) {
            glDisable(GL_TEXTURE_2D);
        }
    }
    
    glPopMatrix();
}

// Load model using Assimp library (supports .blend, .fbx, .dae, and many more formats)
bool loadAssimpModel(const char* filename, Model& model) {
    Assimp::Importer importer;
    
    // Load the model with post-processing
    const aiScene* scene = importer.ReadFile(filename, 
        aiProcess_Triangulate |           // Convert all polygons to triangles
        aiProcess_FlipUVs |               // Flip UV coordinates (OpenGL convention)
        aiProcess_GenNormals |            // Generate normals if not present
        aiProcess_JoinIdenticalVertices | // Optimize by joining identical vertices
        aiProcess_OptimizeMeshes |        // Reduce number of meshes
        aiProcess_ImproveCacheLocality |  // Improve vertex cache locality
        aiProcess_RemoveRedundantMaterials // Remove redundant materials
    );
    
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        printf("Warning: Assimp failed to load file: %s\n", filename);
        printf("  Error: %s\n", importer.GetErrorString());
        return false;
    }
    
    printf("Loading model with Assimp: %s\n", filename);
    printf("  Meshes: %d\n", scene->mNumMeshes);
    printf("  Materials: %d\n", scene->mNumMaterials);
    
    // Process all meshes in the scene
    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* aiMesh = scene->mMeshes[i];
        Mesh mesh;
        
        printf("  Processing mesh %d: %s (%d vertices, %d faces)\n", 
               i, aiMesh->mName.C_Str(), aiMesh->mNumVertices, aiMesh->mNumFaces);
        
        // Process vertices, normals, and texture coordinates
        for (unsigned int j = 0; j < aiMesh->mNumVertices; j++) {
            // Vertices
            Vector3 vertex;
            vertex.x = aiMesh->mVertices[j].x;
            vertex.y = aiMesh->mVertices[j].y;
            vertex.z = aiMesh->mVertices[j].z;
            mesh.vertices.push_back(vertex);
            
            // Normals
            if (aiMesh->HasNormals()) {
                Vector3 normal;
                normal.x = aiMesh->mNormals[j].x;
                normal.y = aiMesh->mNormals[j].y;
                normal.z = aiMesh->mNormals[j].z;
                mesh.normals.push_back(normal);
            } else {
                mesh.normals.push_back(Vector3(0, 1, 0)); // Default up normal
            }
            
            // Texture coordinates
            if (aiMesh->HasTextureCoords(0)) {
                Vector2 texCoord;
                texCoord.u = aiMesh->mTextureCoords[0][j].x;
                texCoord.v = aiMesh->mTextureCoords[0][j].y;
                mesh.texCoords.push_back(texCoord);
            } else {
                mesh.texCoords.push_back(Vector2(0, 0));
            }
        }
        
        // Process faces (all triangulated due to aiProcess_Triangulate)
        // Note: We're storing expanded vertices, not using indexed rendering
        // This is for simplicity and compatibility with the existing rendering code
        
        // Load material/texture if available
        if (aiMesh->mMaterialIndex >= 0 && aiMesh->mMaterialIndex < scene->mNumMaterials) {
            aiMaterial* material = scene->mMaterials[aiMesh->mMaterialIndex];
            
            // Get diffuse texture
            if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
                aiString texPath;
                material->GetTexture(aiTextureType_DIFFUSE, 0, &texPath);
                
                // Try to load the texture
                // First try relative to model file
                std::string fullPath = std::string(filename);
                size_t lastSlash = fullPath.find_last_of("/\\");
                if (lastSlash != std::string::npos) {
                    fullPath = fullPath.substr(0, lastSlash + 1) + std::string(texPath.C_Str());
                    mesh.textureID = loadTexture(fullPath.c_str());
                }
                
                // If that didn't work, try the texture path as-is
                if (mesh.textureID == 0) {
                    mesh.textureID = loadTexture(texPath.C_Str());
                }
                
                if (mesh.textureID != 0) {
                    printf("    Loaded texture: %s\n", texPath.C_Str());
                }
            }
            
            // Store material name
            aiString matName;
            material->Get(AI_MATKEY_NAME, matName);
            mesh.materialName = std::string(matName.C_Str());
        }
        
        model.meshes.push_back(mesh);
    }
    
    printf("Loaded model with Assimp: %s (%d meshes)\n", filename, (int)model.meshes.size());
    return true;
}

// Load model with automatic format detection
bool loadModel(const char* filename, Model& model) {
    const char* ext = strrchr(filename, '.');
    if (!ext) {
        printf("Warning: No file extension found: %s\n", filename);
        return false;
    }
    
    // Try Assimp for .blend, .fbx, .dae, and other advanced formats
    if (strcasecmp(ext, ".blend") == 0 || 
        strcasecmp(ext, ".fbx") == 0 || 
        strcasecmp(ext, ".dae") == 0 ||
        strcasecmp(ext, ".gltf") == 0 ||
        strcasecmp(ext, ".glb") == 0) {
        return loadAssimpModel(filename, model);
    }
    // For .obj and .3ds, try Assimp first, then fall back to custom loaders
    else if (strcasecmp(ext, ".obj") == 0) {
        // Try Assimp first for better compatibility
        if (loadAssimpModel(filename, model)) {
            return true;
        }
        // Fall back to custom OBJ loader
        printf("  Assimp failed, trying custom OBJ loader...\n");
        return loadOBJModel(filename, model);
    } 
    else if (strcasecmp(ext, ".3ds") == 0 || strcasecmp(ext, ".3DS") == 0) {
        // Try Assimp first for better compatibility
        if (loadAssimpModel(filename, model)) {
            return true;
        }
        // Fall back to custom 3DS loader
        printf("  Assimp failed, trying custom 3DS loader...\n");
        return load3DSModel(filename, model);
    }
    
    printf("Warning: Unsupported file format: %s\n", filename);
    return false;
}

#endif // MODEL_LOADER_H
