#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#ifdef _WIN32
#include <glut.h>
#else
#include <GL/glut.h>
#endif

// Assimp includes
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>

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

// Process a single mesh from Assimp
void processMesh(aiMesh* assimpMesh, const aiScene* scene, Mesh& mesh, const std::string& modelDir) {
    // Process vertices, normals, and texture coordinates
    for (unsigned int i = 0; i < assimpMesh->mNumVertices; i++) {
        Vector3 vertex;
        vertex.x = assimpMesh->mVertices[i].x;
        vertex.y = assimpMesh->mVertices[i].y;
        vertex.z = assimpMesh->mVertices[i].z;
        mesh.vertices.push_back(vertex);
        
        if (assimpMesh->HasNormals()) {
            Vector3 normal;
            normal.x = assimpMesh->mNormals[i].x;
            normal.y = assimpMesh->mNormals[i].y;
            normal.z = assimpMesh->mNormals[i].z;
            mesh.normals.push_back(normal);
        } else {
            mesh.normals.push_back(Vector3(0, 1, 0)); // Default normal
        }
        
        if (assimpMesh->HasTextureCoords(0)) {
            Vector2 texCoord;
            texCoord.u = assimpMesh->mTextureCoords[0][i].x;
            texCoord.v = assimpMesh->mTextureCoords[0][i].y;
            mesh.texCoords.push_back(texCoord);
        } else {
            mesh.texCoords.push_back(Vector2(0, 0)); // Default tex coord
        }
    }
    
    // Process faces (Assimp already triangulates if we request it)
    // Note: We expand vertices per face for simplicity with OpenGL immediate mode
    std::vector<Vector3> expandedVertices;
    std::vector<Vector3> expandedNormals;
    std::vector<Vector2> expandedTexCoords;
    
    for (unsigned int i = 0; i < assimpMesh->mNumFaces; i++) {
        aiFace face = assimpMesh->mFaces[i];
        if (face.mNumIndices == 3) {
            for (unsigned int j = 0; j < 3; j++) {
                unsigned int index = face.mIndices[j];
                if (index < mesh.vertices.size()) {
                    expandedVertices.push_back(mesh.vertices[index]);
                    expandedNormals.push_back(mesh.normals[index]);
                    expandedTexCoords.push_back(mesh.texCoords[index]);
                }
            }
        }
    }
    
    // Replace with expanded data
    mesh.vertices = expandedVertices;
    mesh.normals = expandedNormals;
    mesh.texCoords = expandedTexCoords;
    
    // Process materials and textures
    if (assimpMesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[assimpMesh->mMaterialIndex];
        
        // Try to load diffuse texture
        if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString texPath;
            material->GetTexture(aiTextureType_DIFFUSE, 0, &texPath);
            
            // Try with model directory first
            std::string fullPath = modelDir + std::string(texPath.C_Str());
            mesh.textureID = loadTexture(fullPath.c_str());
            
            // Try without directory if not found
            if (mesh.textureID == 0) {
                mesh.textureID = loadTexture(texPath.C_Str());
            }
        }
    }
}

// Recursively process nodes in the scene
void processNode(aiNode* node, const aiScene* scene, Model& model, const std::string& modelDir) {
    // Process all meshes in this node
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* assimpMesh = scene->mMeshes[node->mMeshes[i]];
        Mesh mesh;
        processMesh(assimpMesh, scene, mesh, modelDir);
        model.meshes.push_back(mesh);
    }
    
    // Recursively process child nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene, model, modelDir);
    }
}

// Load model using Assimp - supports many formats including .blend, .obj, .3ds, .ma, .mb, .fbx, etc.
bool loadModel(const char* filename, Model& model) {
    printf("Loading model with Assimp: %s\n", filename);
    
    // Create Assimp importer
    Assimp::Importer importer;
    
    // Read the file with post-processing options
    // aiProcess_Triangulate: Convert all polygons to triangles
    // aiProcess_FlipUVs: Flip texture coordinates (some formats need this)
    // aiProcess_GenNormals: Generate normals if not present
    // aiProcess_JoinIdenticalVertices: Optimize the mesh
    const aiScene* scene = importer.ReadFile(filename,
        aiProcess_Triangulate |
        aiProcess_GenSmoothNormals |
        aiProcess_JoinIdenticalVertices |
        aiProcess_PreTransformVertices);
    
    // Check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        printf("ERROR::ASSIMP: %s\n", importer.GetErrorString());
        return false;
    }
    
    // Get the directory of the model file for texture loading
    std::string modelDir = getDirectory(filename);
    
    // Process the scene
    processNode(scene->mRootNode, scene, model, modelDir);
    
    // Calculate total vertices
    int totalVertices = 0;
    for (size_t i = 0; i < model.meshes.size(); i++) {
        totalVertices += model.meshes[i].vertices.size();
    }
    
    printf("Successfully loaded model: %s (%d meshes, %d vertices)\n", 
           filename, (int)model.meshes.size(), totalVertices);
    
    return true;
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
