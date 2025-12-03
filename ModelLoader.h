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

#ifdef _WIN32
// Windows doesn't have strcasecmp
#define strcasecmp _stricmp
#endif

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
    
    Model() : scale(1.0f), offset(0, 0, 0) {}
};

// Texture loading functions
GLuint loadTexture(const char* filename) {
    // For now, return 0 (no texture)
    // In a full implementation, this would load BMP/JPG/PNG files
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
    // 3DS files use chunk-based format
    // For simplicity, we'll create a basic mesh
    fclose(file);
    
    // Create a simple placeholder mesh
    Mesh mesh;
    model.meshes.push_back(mesh);
    
    printf("Loaded 3DS model: %s (using placeholder geometry)\n", filename);
    return true;
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

// Load model with automatic format detection
bool loadModel(const char* filename, Model& model) {
    const char* ext = strrchr(filename, '.');
    if (ext) {
        if (strcasecmp(ext, ".obj") == 0) {
            return loadOBJModel(filename, model);
        } else if (strcasecmp(ext, ".3ds") == 0 || strcasecmp(ext, ".3DS") == 0) {
            return load3DSModel(filename, model);
        }
    }
    return false;
}

#endif // MODEL_LOADER_H
