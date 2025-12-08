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
#include <map>

#ifdef _WIN32
// Windows doesn't have strcasecmp
#define strcasecmp _stricmp
#endif

// OpenGL constants that may not be defined in older headers
#ifndef GL_BGR
#define GL_BGR 0x80E0
#endif

// Simple 3D model structures
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

// Mesh stores expanded vertex/normal/texcoord data for rendering
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

// Texture cache to avoid loading the same texture multiple times
std::map<std::string, GLuint> textureCache;

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
    
    // Read image data
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
    // Check cache first
    if (textureCache.find(filename) != textureCache.end()) {
        return textureCache[filename];
    }
    
    // Check file extension
    const char* ext = strrchr(filename, '.');
    if (ext && (strcasecmp(ext, ".bmp") == 0)) {
        GLuint texID = loadBMPTexture(filename);
        if (texID != 0) {
            textureCache[filename] = texID;
        }
        return texID;
    }
    
    // For other formats (JPG, PNG), would need additional libraries
    // For now, return 0 (no texture)
    return 0;
}

// Helper function to extract directory from filepath
std::string getDirectory(const std::string& filepath) {
    size_t pos = filepath.find_last_of("/\\");
    if (pos != std::string::npos) {
        return filepath.substr(0, pos + 1);
    }
    return "";
}

// OBJ file parser - supports vertices, normals, texture coordinates, and faces
#define MAX_LINE_LENGTH 256
bool loadOBJ(const char* filename, Model& model) {
    printf("Loading OBJ model: %s\n", filename);
    
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open OBJ file: %s\n", filename);
        return false;
    }
    
    std::string modelDir = getDirectory(filename);
    
    // Temporary storage for indexed data
    std::vector<Vector3> temp_vertices;
    std::vector<Vector3> temp_normals;
    std::vector<Vector2> temp_texcoords;
    
    Mesh currentMesh;
    bool hasMesh = false;
    
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        // Parse vertices
        if (strncmp(line, "v ", 2) == 0) {
            Vector3 vertex;
            if (sscanf(line + 2, "%f %f %f", &vertex.x, &vertex.y, &vertex.z) == 3) {
                temp_vertices.push_back(vertex);
            }
        }
        // Parse normals
        else if (strncmp(line, "vn ", 3) == 0) {
            Vector3 normal;
            if (sscanf(line + 3, "%f %f %f", &normal.x, &normal.y, &normal.z) == 3) {
                temp_normals.push_back(normal);
            }
        }
        // Parse texture coordinates
        else if (strncmp(line, "vt ", 3) == 0) {
            Vector2 texcoord;
            if (sscanf(line + 3, "%f %f", &texcoord.u, &texcoord.v) == 2) {
                temp_texcoords.push_back(texcoord);
            }
        }
        // Parse faces
        else if (strncmp(line, "f ", 2) == 0) {
            if (!hasMesh) {
                hasMesh = true;
            }
            
            int v1, v2, v3, vt1, vt2, vt3, vn1, vn2, vn3;
            int matches = sscanf(line + 2, "%d/%d/%d %d/%d/%d %d/%d/%d",
                                &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);
            
            if (matches == 9) {
                // f v/vt/vn
                // OBJ indices are 1-based, convert to 0-based
                if (v1 > 0 && v1 <= (int)temp_vertices.size() &&
                    v2 > 0 && v2 <= (int)temp_vertices.size() &&
                    v3 > 0 && v3 <= (int)temp_vertices.size()) {
                    currentMesh.vertices.push_back(temp_vertices[v1 - 1]);
                    currentMesh.vertices.push_back(temp_vertices[v2 - 1]);
                    currentMesh.vertices.push_back(temp_vertices[v3 - 1]);
                }
                
                if (vn1 > 0 && vn1 <= (int)temp_normals.size() &&
                    vn2 > 0 && vn2 <= (int)temp_normals.size() &&
                    vn3 > 0 && vn3 <= (int)temp_normals.size()) {
                    currentMesh.normals.push_back(temp_normals[vn1 - 1]);
                    currentMesh.normals.push_back(temp_normals[vn2 - 1]);
                    currentMesh.normals.push_back(temp_normals[vn3 - 1]);
                }
                
                if (vt1 > 0 && vt1 <= (int)temp_texcoords.size() &&
                    vt2 > 0 && vt2 <= (int)temp_texcoords.size() &&
                    vt3 > 0 && vt3 <= (int)temp_texcoords.size()) {
                    currentMesh.texCoords.push_back(temp_texcoords[vt1 - 1]);
                    currentMesh.texCoords.push_back(temp_texcoords[vt2 - 1]);
                    currentMesh.texCoords.push_back(temp_texcoords[vt3 - 1]);
                }
            } else {
                // Try f v//vn (no texture coords)
                matches = sscanf(line + 2, "%d//%d %d//%d %d//%d",
                               &v1, &vn1, &v2, &vn2, &v3, &vn3);
                if (matches == 6) {
                    if (v1 > 0 && v1 <= (int)temp_vertices.size() &&
                        v2 > 0 && v2 <= (int)temp_vertices.size() &&
                        v3 > 0 && v3 <= (int)temp_vertices.size()) {
                        currentMesh.vertices.push_back(temp_vertices[v1 - 1]);
                        currentMesh.vertices.push_back(temp_vertices[v2 - 1]);
                        currentMesh.vertices.push_back(temp_vertices[v3 - 1]);
                    }
                    
                    if (vn1 > 0 && vn1 <= (int)temp_normals.size() &&
                        vn2 > 0 && vn2 <= (int)temp_normals.size() &&
                        vn3 > 0 && vn3 <= (int)temp_normals.size()) {
                        currentMesh.normals.push_back(temp_normals[vn1 - 1]);
                        currentMesh.normals.push_back(temp_normals[vn2 - 1]);
                        currentMesh.normals.push_back(temp_normals[vn3 - 1]);
                    }
                } else {
                    // Try f v (only vertices)
                    matches = sscanf(line + 2, "%d %d %d", &v1, &v2, &v3);
                    if (matches == 3 && v1 > 0 && v1 <= (int)temp_vertices.size() &&
                        v2 > 0 && v2 <= (int)temp_vertices.size() &&
                        v3 > 0 && v3 <= (int)temp_vertices.size()) {
                        currentMesh.vertices.push_back(temp_vertices[v1 - 1]);
                        currentMesh.vertices.push_back(temp_vertices[v2 - 1]);
                        currentMesh.vertices.push_back(temp_vertices[v3 - 1]);
                    }
                }
            }
        }
    }
    
    fclose(file);
    
    // Add the mesh if we parsed any data
    if (hasMesh && currentMesh.vertices.size() > 0) {
        // Fill in missing normals with default values
        while (currentMesh.normals.size() < currentMesh.vertices.size()) {
            currentMesh.normals.push_back(Vector3(0, 1, 0));
        }
        // Fill in missing texture coordinates
        while (currentMesh.texCoords.size() < currentMesh.vertices.size()) {
            currentMesh.texCoords.push_back(Vector2(0, 0));
        }
        
        model.meshes.push_back(currentMesh);
    }
    
    // Calculate total vertices across all meshes
    int totalVertices = 0;
    for (size_t i = 0; i < model.meshes.size(); i++) {
        totalVertices += model.meshes[i].vertices.size();
    }
    
    printf("Successfully loaded OBJ: %s (%d meshes, %d vertices)\n",
           filename, (int)model.meshes.size(), totalVertices);
    
    return model.meshes.size() > 0;
}

// Simple 3DS file parser - basic implementation for triangular meshes
#define MAX_3DS_VERTICES 1000000  // Maximum vertices per mesh (safety limit)
#define MAX_3DS_FACES 1000000     // Maximum faces per mesh (safety limit)
bool load3DS(const char* filename, Model& model) {
    printf("Loading 3DS model: %s\n", filename);
    
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Error: Could not open 3DS file: %s\n", filename);
        return false;
    }
    
    Mesh mesh;
    
    // 3DS file format uses chunks with IDs and lengths
    unsigned short chunkID;
    unsigned int chunkLength;
    
    while (fread(&chunkID, 2, 1, file) == 1) {
        if (fread(&chunkLength, 4, 1, file) != 1) break;
        
        // Main chunk
        if (chunkID == 0x4D4D) {
            continue;
        }
        // 3D Editor chunk
        else if (chunkID == 0x3D3D) {
            continue;
        }
        // Object block
        else if (chunkID == 0x4000) {
            // Skip object name (max 255 chars + null terminator)
            char name[257];
            int i = 0;
            while (i < 256) {
                if (fread(&name[i], 1, 1, file) != 1) break;
                if (name[i] == 0) break;
                i++;
            }
            name[256] = 0;  // Ensure null termination
        }
        // Triangular mesh
        else if (chunkID == 0x4100) {
            continue;
        }
        // Vertices list
        else if (chunkID == 0x4110) {
            unsigned short numVertices;
            if (fread(&numVertices, 2, 1, file) == 1 && numVertices <= MAX_3DS_VERTICES) {
                for (int i = 0; i < numVertices; i++) {
                    Vector3 vertex;
                    if (fread(&vertex.x, 4, 1, file) != 1) break;
                    if (fread(&vertex.y, 4, 1, file) != 1) break;
                    if (fread(&vertex.z, 4, 1, file) != 1) break;
                    mesh.vertices.push_back(vertex);
                }
            }
        }
        // Faces description
        else if (chunkID == 0x4120) {
            unsigned short numFaces;
            if (fread(&numFaces, 2, 1, file) == 1 && numFaces <= MAX_3DS_FACES) {
                std::vector<Vector3> originalVertices = mesh.vertices;
                mesh.vertices.clear();
                
                for (int i = 0; i < numFaces; i++) {
                    unsigned short v1, v2, v3, flags;
                    if (fread(&v1, 2, 1, file) != 1) break;
                    if (fread(&v2, 2, 1, file) != 1) break;
                    if (fread(&v3, 2, 1, file) != 1) break;
                    if (fread(&flags, 2, 1, file) != 1) break;
                    
                    if (v1 < originalVertices.size() &&
                        v2 < originalVertices.size() &&
                        v3 < originalVertices.size()) {
                        mesh.vertices.push_back(originalVertices[v1]);
                        mesh.vertices.push_back(originalVertices[v2]);
                        mesh.vertices.push_back(originalVertices[v3]);
                    }
                }
            }
        }
        // Mapping coordinates
        else if (chunkID == 0x4140) {
            unsigned short numCoords;
            if (fread(&numCoords, 2, 1, file) == 1 && numCoords <= MAX_3DS_VERTICES) {
                for (int i = 0; i < numCoords; i++) {
                    Vector2 texcoord;
                    if (fread(&texcoord.u, 4, 1, file) != 1) break;
                    if (fread(&texcoord.v, 4, 1, file) != 1) break;
                    mesh.texCoords.push_back(texcoord);
                }
            }
        }
        else {
            // Skip unknown chunks - check for underflow
            if (chunkLength > 6) {
                long skipBytes = chunkLength - 6;
                fseek(file, skipBytes, SEEK_CUR);
            }
        }
    }
    
    fclose(file);
    
    // Fill in missing data
    while (mesh.normals.size() < mesh.vertices.size()) {
        mesh.normals.push_back(Vector3(0, 1, 0));
    }
    while (mesh.texCoords.size() < mesh.vertices.size()) {
        mesh.texCoords.push_back(Vector2(0, 0));
    }
    
    if (mesh.vertices.size() > 0) {
        model.meshes.push_back(mesh);
    }
    
    printf("Successfully loaded 3DS: %s (%d meshes, %d vertices)\n",
           filename, (int)model.meshes.size(), (int)mesh.vertices.size());
    
    return model.meshes.size() > 0;
}

// Load model - detects format and uses appropriate parser
bool loadModel(const char* filename, Model& model) {
    // Check file extension
    const char* ext = strrchr(filename, '.');
    if (!ext) {
        printf("Error: No file extension found in: %s\n", filename);
        return false;
    }
    
    // Load based on extension
    if (strcasecmp(ext, ".obj") == 0) {
        return loadOBJ(filename, model);
    } else if (strcasecmp(ext, ".3ds") == 0 || strcasecmp(ext, ".3DS") == 0) {
        return load3DS(filename, model);
    } else {
        printf("Error: Unsupported file format: %s\n", ext);
        return false;
    }
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



#endif // MODEL_LOADER_H
