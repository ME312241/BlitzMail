#ifdef _WIN32
#include <glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "ModelLoader.h"

// Constants
#define PI 3.14159265359f
#define MAX_PITCH 89.0f
#define MIN_PITCH -89.0f

// Camera and player state
float playerX = 0.0f, playerY = 1.5f, playerZ = 10.0f;
float cameraYaw = 0.0f, cameraPitch = 0.0f;
float playerVelY = 0.0f;
bool isJumping = false;
bool isCrouching = false;
bool thirdPerson = false;

// Movement keys state
bool keyW = false, keyA = false, keyS = false, keyD = false;

// Mouse state
int lastMouseX = 400, lastMouseY = 300;
bool firstMouse = true;

// Time and animation
float sunAngle = 0.0f;
float lampFlicker = 0.0f;
int frameCount = 0;

// Collected packages
int packagesCollected = 0;
const int TOTAL_PACKAGES = 5;

// Package positions
struct Package {
    float x, y, z;
    bool collected;
};

Package packages[5] = {
    {15.0f, 0.5f, 5.0f, false},
    {-20.0f, 0.5f, -10.0f, false},
    {10.0f, 0.5f, -25.0f, false},
    {-15.0f, 0.5f, 15.0f, false},
    {25.0f, 0.5f, -15.0f, false}
};

// 3D Models
Model mailmanModel;
Model treeModel;
Model fenceModel;
Model rockModel;
Model rockSetModel;
Model houseModel;
Model cottageModel;
Model streetLampModel;
Model wheatModel;
Model carrotModel;
Model grassBlockModel;

// Model loading flags
bool modelsLoaded = false;

// Model file paths
const char* MODEL_PATH_TREE = "models/tree/tree1_3ds/Tree1.3ds";
const char* MODEL_PATH_ROCK1 = "models/1elmla01hh-Rock1_BYTyroSmith/Rock1/Rock1.3ds";
const char* MODEL_PATH_ROCKSET = "models/xvs3wxwo2o-RockSet_MadeByTyroSmith/RockSet/RockSet.3ds";
const char* MODEL_PATH_FARMHOUSE = "models/4vd2sk31doow-farmhouse_maya16/Farmhouse Maya 2016 Updated/farmhouse_obj.obj";
const char* MODEL_PATH_STREETLAMP = "models/s3duldjjt9fk-StreetLampByTyroSmith/Street Lamp/StreetLamp.3ds";
const char* MODEL_PATH_WHEAT = "models/10458_Wheat_Field_v1_L3.123c5ecd0518-ae16-4fee-bf80-4177de196237/10458_Wheat_Field_v1_L3.123c5ecd0518-ae16-4fee-bf80-4177de196237/10458_Wheat_Field_v1_max2010_it2.obj";
const char* MODEL_PATH_CARROT = "models/Carrot_v01_l3.123c059c383a-f43b-48c0-b28a-bec318013e17/Carrot_v01_l3.123c059c383a-f43b-48c0-b28a-bec318013e17/10170_Carrot_v01_L3.obj";
const char* MODEL_PATH_GRASSBLOCK = "models/grass-block/grass-block.3DS";
const char* MODEL_PATH_TREE_ALT = "models/15od5xhlv2jc-Tree_02/Tree 02/Tree.obj";

// Forward declarations
void loadAllModels();
void drawPlayer();
void drawMailBag();
void drawTerrain();
void drawHouse(float x, float z, float scale);
void drawTree(float x, float z, float height);
void drawFence(float x, float z, float length, float rotation);
void drawRock(float x, float z, float size);
void drawCrop(float x, float z);
void drawGrassBlock(float x, float z);
void drawStreetLamp(float x, float z);
void drawPackage(float x, float y, float z);
void setupLighting();
void updateSunLight();
void updateLampLights();

// Load all 3DS models from the models directory
void loadAllModels() {
    printf("Loading 3D models...\n");
    
    // Seed random number generator for model variation
    srand((unsigned int)time(NULL));
    
    // Load mailman model (using .blend, but we'll use primitives with enhancements)
    // Note: The mailman model is Player.blend which needs conversion
    // For now, we'll enhance the primitive-based mailman
    
    // Load tree model
    if (loadModel(MODEL_PATH_TREE, treeModel)) {
        treeModel.scale = 0.02f;
        treeModel.offset = Vector3(0, 0, 0);
    }
    
    // Load rock models
    if (loadModel(MODEL_PATH_ROCK1, rockModel)) {
        rockModel.scale = 0.5f;
        rockModel.offset = Vector3(0, 0, 0);
    }
    
    if (loadModel(MODEL_PATH_ROCKSET, rockSetModel)) {
        rockSetModel.scale = 0.3f;
        rockSetModel.offset = Vector3(0, 0, 0);
    }
    
    // Load house model (using OBJ which is available)
    if (loadModel(MODEL_PATH_FARMHOUSE, houseModel)) {
        houseModel.scale = 0.01f;
        houseModel.offset = Vector3(0, 0, 0);
    }
    
    // Load street lamp model
    if (loadModel(MODEL_PATH_STREETLAMP, streetLampModel)) {
        streetLampModel.scale = 0.05f;
        streetLampModel.offset = Vector3(0, 0, 0);
    }
    
    // Load wheat model
    if (loadModel(MODEL_PATH_WHEAT, wheatModel)) {
        wheatModel.scale = 0.005f;
        wheatModel.offset = Vector3(0, 0, 0);
    }
    
    // Load carrot model
    if (loadModel(MODEL_PATH_CARROT, carrotModel)) {
        carrotModel.scale = 0.008f;
        carrotModel.offset = Vector3(0, 0, 0);
    }
    
    // Load grass block model
    if (loadModel(MODEL_PATH_GRASSBLOCK, grassBlockModel)) {
        grassBlockModel.scale = 0.5f;
        grassBlockModel.offset = Vector3(0, 0, 0);
    }
    
    // Load tree model (alternate)
    if (loadModel(MODEL_PATH_TREE_ALT, treeModel)) {
        treeModel.scale = 0.01f;
        treeModel.offset = Vector3(0, 0, 0);
    }
    
    modelsLoaded = true;
    printf("Models loaded successfully!\n");
}

void drawPlayer() {
    glPushMatrix();
    
    // Body (torso)
    glColor3f(0.2f, 0.3f, 0.8f); // Blue uniform
    glPushMatrix();
    glTranslatef(0, 0.8f, 0);
    glScalef(0.6f, 1.0f, 0.4f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Head
    glColor3f(0.9f, 0.7f, 0.6f); // Skin tone
    glPushMatrix();
    glTranslatef(0, 1.6f, 0);
    glutSolidSphere(0.3f, 20, 20);
    glPopMatrix();
    
    // Mail bag on back
    drawMailBag();
    
    // Arms
    glColor3f(0.2f, 0.3f, 0.8f);
    // Left arm
    glPushMatrix();
    glTranslatef(-0.4f, 0.7f, 0);
    glRotatef(20, 0, 0, 1);
    glScalef(0.15f, 0.8f, 0.15f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Right arm
    glPushMatrix();
    glTranslatef(0.4f, 0.7f, 0);
    glRotatef(-20, 0, 0, 1);
    glScalef(0.15f, 0.8f, 0.15f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Legs
    glColor3f(0.15f, 0.15f, 0.15f); // Dark pants
    // Left leg
    glPushMatrix();
    glTranslatef(-0.15f, 0.0f, 0);
    glScalef(0.2f, 0.8f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Right leg
    glPushMatrix();
    glTranslatef(0.15f, 0.0f, 0);
    glScalef(0.2f, 0.8f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Cap
    glColor3f(0.2f, 0.3f, 0.8f);
    glPushMatrix();
    glTranslatef(0, 1.85f, 0);
    glScalef(1.2f, 0.3f, 1.2f);
    glutSolidSphere(0.3f, 20, 20);
    glPopMatrix();
    
    // Cap visor
    glPushMatrix();
    glTranslatef(0, 1.75f, 0.3f);
    glScalef(0.35f, 0.05f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Postal badge on chest
    glColor3f(0.9f, 0.8f, 0.1f); // Gold badge
    glPushMatrix();
    glTranslatef(0, 1.0f, 0.21f);
    glScalef(0.15f, 0.15f, 0.02f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    glPopMatrix();
}

void drawMailBag() {
    glPushMatrix();
    glTranslatef(0, 0.9f, -0.3f);
    
    // Main bag body
    glColor3f(0.6f, 0.4f, 0.2f); // Brown leather
    glPushMatrix();
    glScalef(0.4f, 0.5f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Bag flap
    glColor3f(0.55f, 0.35f, 0.15f);
    glPushMatrix();
    glTranslatef(0, 0.26f, 0.05f);
    glRotatef(-10, 1, 0, 0);
    glScalef(0.42f, 0.08f, 0.22f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Bag strap
    glColor3f(0.5f, 0.3f, 0.1f);
    glPushMatrix();
    glTranslatef(-0.2f, 0.3f, 0);
    glRotatef(45, 0, 0, 1);
    glScalef(0.05f, 0.8f, 0.05f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Metal buckle on strap
    glColor3f(0.7f, 0.7f, 0.7f);
    glPushMatrix();
    glTranslatef(-0.1f, 0.6f, 0);
    glScalef(0.08f, 0.08f, 0.03f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Letters/envelopes sticking out of bag
    glColor3f(0.95f, 0.95f, 0.9f); // White paper
    // Envelope 1
    glPushMatrix();
    glTranslatef(-0.05f, 0.15f, 0.1f);
    glRotatef(15, 0, 0, 1);
    glScalef(0.15f, 0.2f, 0.02f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Envelope 2
    glPushMatrix();
    glTranslatef(0.05f, 0.18f, 0.12f);
    glRotatef(-10, 0, 0, 1);
    glScalef(0.12f, 0.18f, 0.02f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    glPopMatrix();
}

void drawTerrain() {
    // Ground plane
    glColor3f(0.4f, 0.6f, 0.3f); // Green grass
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glVertex3f(-100, 0, -100);
    glVertex3f(100, 0, -100);
    glVertex3f(100, 0, 100);
    glVertex3f(-100, 0, 100);
    glEnd();
    
    // Add some terrain variation with smaller patches
    glColor3f(0.45f, 0.65f, 0.35f);
    for (int i = -10; i < 10; i++) {
        for (int j = -10; j < 10; j++) {
            if ((i + j) % 3 == 0) {
                glBegin(GL_QUADS);
                float x = i * 10.0f;
                float z = j * 10.0f;
                glVertex3f(x, 0.01f, z);
                glVertex3f(x + 8, 0.01f, z);
                glVertex3f(x + 8, 0.01f, z + 8);
                glVertex3f(x, 0.01f, z + 8);
                glEnd();
            }
        }
    }
}

void drawHouse(float x, float z, float scale) {
    glPushMatrix();
    glTranslatef(x, 0, z);
    glScalef(scale, scale, scale);
    
    // Try to use loaded model
    if (modelsLoaded && houseModel.meshes.size() > 0) {
        glPushMatrix();
        glScalef(3.0f, 3.0f, 3.0f);
        renderModel(houseModel);
        glPopMatrix();
    } else {
        // Fallback to primitives
        // House base
        glColor3f(0.8f, 0.7f, 0.6f); // Beige walls
        glPushMatrix();
        glTranslatef(0, 2.5f, 0);
        glScalef(4.0f, 3.0f, 4.0f);
        glutSolidCube(1.0f);
        glPopMatrix();
        
        // Roof
        glColor3f(0.6f, 0.2f, 0.1f); // Red roof
        glPushMatrix();
        glTranslatef(0, 4.5f, 0);
        glRotatef(-90, 1, 0, 0);
        glutSolidCone(3.0f, 2.0f, 4, 10);
        glPopMatrix();
        
        // Door
        glColor3f(0.4f, 0.2f, 0.1f);
        glPushMatrix();
        glTranslatef(0, 1.0f, 2.01f);
        glScalef(0.8f, 1.5f, 0.1f);
        glutSolidCube(1.0f);
        glPopMatrix();
        
        // Windows
        glColor3f(0.6f, 0.8f, 1.0f); // Blue windows
        glPushMatrix();
        glTranslatef(-1.0f, 2.5f, 2.01f);
        glScalef(0.6f, 0.6f, 0.05f);
        glutSolidCube(1.0f);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(1.0f, 2.5f, 2.01f);
        glScalef(0.6f, 0.6f, 0.05f);
        glutSolidCube(1.0f);
        glPopMatrix();
    }
    
    glPopMatrix();
}

void drawTree(float x, float z, float height) {
    glPushMatrix();
    glTranslatef(x, 0, z);
    
    // Try to use loaded model
    if (modelsLoaded && treeModel.meshes.size() > 0) {
        glPushMatrix();
        glScalef(height / 4.0f, height / 4.0f, height / 4.0f);
        renderModel(treeModel);
        glPopMatrix();
    } else {
        // Fallback to primitives
        // Trunk
        glColor3f(0.4f, 0.25f, 0.1f); // Brown
        glPushMatrix();
        glTranslatef(0, height * 0.3f, 0);
        glRotatef(-90, 1, 0, 0);
        GLUquadric* quad = gluNewQuadric();
        if (quad) {
            gluCylinder(quad, 0.3f, 0.5f, height * 0.6f, 10, 10);
            gluDeleteQuadric(quad);
        }
        glPopMatrix();
        
        // Foliage
        glColor3f(0.1f, 0.5f, 0.1f); // Dark green
        glPushMatrix();
        glTranslatef(0, height * 0.7f, 0);
        glutSolidSphere(height * 0.4f, 15, 15);
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(0, height * 0.85f, 0);
        glutSolidSphere(height * 0.35f, 15, 15);
        glPopMatrix();
    }
    
    glPopMatrix();
}

void drawFence(float x, float z, float length, float rotation) {
    glPushMatrix();
    glTranslatef(x, 0, z);
    glRotatef(rotation, 0, 1, 0);
    
    glColor3f(0.5f, 0.35f, 0.2f); // Wood color
    
    // Fence posts
    for (float i = 0; i < length; i += 2.0f) {
        glPushMatrix();
        glTranslatef(i, 0.75f, 0);
        glScalef(0.15f, 1.5f, 0.15f);
        glutSolidCube(1.0f);
        glPopMatrix();
    }
    
    // Horizontal rails
    glPushMatrix();
    glTranslatef(length / 2, 1.0f, 0);
    glScalef(length, 0.1f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(length / 2, 0.5f, 0);
    glScalef(length, 0.1f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    glPopMatrix();
}

void drawRock(float x, float z, float size) {
    glPushMatrix();
    glTranslatef(x, size * 0.3f, z);
    
    // Try to use loaded model
    bool modelRendered = false;
    if (modelsLoaded) {
        // Select between rock models if both are available
        Model* selectedModel = NULL;
        if (rockModel.meshes.size() > 0 && rockSetModel.meshes.size() > 0) {
            // Use position-based deterministic selection for consistency
            selectedModel = ((int)(x + z) % 2 == 0) ? &rockModel : &rockSetModel;
        } else if (rockModel.meshes.size() > 0) {
            selectedModel = &rockModel;
        } else if (rockSetModel.meshes.size() > 0) {
            selectedModel = &rockSetModel;
        }
        
        if (selectedModel) {
            glPushMatrix();
            glScalef(size, size, size);
            renderModel(*selectedModel);
            glPopMatrix();
            modelRendered = true;
        }
    }
    
    if (!modelRendered) {
        // Fallback to primitive
        glColor3f(0.5f, 0.5f, 0.5f); // Gray
        glPushMatrix();
        glScalef(size, size * 0.6f, size * 0.8f);
        glutSolidSphere(1.0f, 8, 8);
        glPopMatrix();
    }
    
    glPopMatrix();
}

void drawCrop(float x, float z) {
    glPushMatrix();
    glTranslatef(x, 0, z);
    
    // Try to use loaded models (wheat or carrot)
    bool modelRendered = false;
    if (modelsLoaded) {
        // Select between wheat and carrot based on position for deterministic placement
        Model* selectedModel = NULL;
        if (wheatModel.meshes.size() > 0 && carrotModel.meshes.size() > 0) {
            selectedModel = ((int)(x + z) % 2 == 0) ? &wheatModel : &carrotModel;
        } else if (wheatModel.meshes.size() > 0) {
            selectedModel = &wheatModel;
        } else if (carrotModel.meshes.size() > 0) {
            selectedModel = &carrotModel;
        }
        
        if (selectedModel) {
            renderModel(*selectedModel);
            modelRendered = true;
        }
    }
    
    if (!modelRendered) {
        // Fallback to primitives - Wheat/carrot stalks
        glColor3f(0.8f, 0.7f, 0.2f); // Golden wheat
        for (int i = -2; i <= 2; i++) {
            for (int j = -2; j <= 2; j++) {
                glPushMatrix();
                glTranslatef(i * 0.3f, 0.3f, j * 0.3f);
                glScalef(0.05f, 0.6f, 0.05f);
                glutSolidCube(1.0f);
                glPopMatrix();
            }
        }
    }
    
    glPopMatrix();
}

void drawGrassBlock(float x, float z) {
    glPushMatrix();
    glTranslatef(x, 0.5f, z);
    
    // Try to use loaded model
    if (modelsLoaded && grassBlockModel.meshes.size() > 0) {
        renderModel(grassBlockModel);
    } else {
        // Fallback to primitives
        // Top (grass)
        glColor3f(0.3f, 0.7f, 0.3f);
        glBegin(GL_QUADS);
        glNormal3f(0, 1, 0);
        glVertex3f(-0.5f, 0.5f, -0.5f);
        glVertex3f(0.5f, 0.5f, -0.5f);
        glVertex3f(0.5f, 0.5f, 0.5f);
        glVertex3f(-0.5f, 0.5f, 0.5f);
        glEnd();
        
        // Sides (dirt)
        glColor3f(0.55f, 0.4f, 0.3f);
        glBegin(GL_QUADS);
        // Front
        glNormal3f(0, 0, 1);
        glVertex3f(-0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, 0.5f, 0.5f);
        glVertex3f(-0.5f, 0.5f, 0.5f);
        // Back
        glNormal3f(0, 0, -1);
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f, 0.5f, -0.5f);
        glVertex3f(0.5f, 0.5f, -0.5f);
        glVertex3f(0.5f, -0.5f, -0.5f);
        // Left
        glNormal3f(-1, 0, 0);
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f, -0.5f, 0.5f);
        glVertex3f(-0.5f, 0.5f, 0.5f);
        glVertex3f(-0.5f, 0.5f, -0.5f);
        // Right
        glNormal3f(1, 0, 0);
        glVertex3f(0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, 0.5f, -0.5f);
        glVertex3f(0.5f, 0.5f, 0.5f);
        glVertex3f(0.5f, -0.5f, 0.5f);
        glEnd();
    }
    
    glPopMatrix();
}

void drawStreetLamp(float x, float z) {
    glPushMatrix();
    glTranslatef(x, 0, z);
    
    // Try to use loaded model
    if (modelsLoaded && streetLampModel.meshes.size() > 0) {
        glPushMatrix();
        renderModel(streetLampModel);
        glPopMatrix();
    } else {
        // Fallback to primitives
        // Lamp post
        glColor3f(0.2f, 0.2f, 0.2f); // Dark gray metal
        glPushMatrix();
        glTranslatef(0, 2.5f, 0);
        glRotatef(-90, 1, 0, 0);
        GLUquadric* quad = gluNewQuadric();
        if (quad) {
            gluCylinder(quad, 0.1f, 0.15f, 5.0f, 12, 12);
            gluDeleteQuadric(quad);
        }
        glPopMatrix();
        
        // Lamp head
        glPushMatrix();
        glTranslatef(0, 5.0f, 0);
        
        // Lamp housing
        glColor3f(0.3f, 0.3f, 0.3f);
        glPushMatrix();
        glScalef(0.6f, 0.4f, 0.6f);
        glutSolidCube(1.0f);
        glPopMatrix();
        
        // Light bulb (glowing effect)
        if (sunAngle > 90 && sunAngle < 270) { // Night time
            glColor3f(1.0f, 1.0f, 0.9f + lampFlicker * 0.1f); // White with flicker
        } else {
            glColor3f(0.9f, 0.9f, 0.8f); // Dim during day
        }
        glPushMatrix();
        glTranslatef(0, -0.3f, 0);
        glutSolidSphere(0.2f, 12, 12);
        glPopMatrix();
        
        glPopMatrix();
    }
    
    glPopMatrix();
}

void drawPackage(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    
    // Rotate the package for visual effect
    glRotatef(frameCount * 0.5f, 0, 1, 0);
    
    // Box
    glColor3f(0.7f, 0.5f, 0.3f); // Cardboard color
    glutSolidCube(0.8f);
    
    // Tape cross
    glColor3f(0.8f, 0.7f, 0.5f);
    glPushMatrix();
    glTranslatef(0, 0.41f, 0);
    glScalef(0.85f, 0.01f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0, 0.41f, 0);
    glScalef(0.2f, 0.01f, 0.85f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    glPopMatrix();
}

void setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0); // Sun
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    
    // Enable additional lights for street lamps
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_LIGHT3);
    glEnable(GL_LIGHT4);
    
    // Set up material properties
    GLfloat mat_specular[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat mat_shininess[] = { 20.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void updateSunLight() {
    // Calculate sun position
    float sunX = cos(sunAngle * PI / 180.0f) * 50.0f;
    float sunY = sin(sunAngle * PI / 180.0f) * 50.0f;
    float sunZ = 0.0f;
    
    GLfloat light_position[] = { sunX, sunY, sunZ, 0.0f }; // Directional light
    
    // Change sun color based on angle
    GLfloat light_ambient[] = { 0.3f, 0.3f, 0.35f, 1.0f };
    GLfloat light_diffuse[4];
    
    if (sunAngle < 80 || sunAngle > 280) { // Day
        light_diffuse[0] = 1.0f; // Yellow
        light_diffuse[1] = 0.95f;
        light_diffuse[2] = 0.8f;
        light_diffuse[3] = 1.0f;
    } else if (sunAngle < 100 || sunAngle > 260) { // Sunset/sunrise
        light_diffuse[0] = 1.0f; // Orange
        light_diffuse[1] = 0.6f;
        light_diffuse[2] = 0.3f;
        light_diffuse[3] = 1.0f;
    } else { // Night
        light_ambient[0] = 0.1f;
        light_ambient[1] = 0.1f;
        light_ambient[2] = 0.15f;
        light_diffuse[0] = 0.2f;
        light_diffuse[1] = 0.2f;
        light_diffuse[2] = 0.3f;
        light_diffuse[3] = 1.0f;
    }
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
}

void updateLampLights() {
    // Only light up lamps at night
    if (sunAngle > 90 && sunAngle < 270) {
        // Lamp 1
        GLfloat lamp1_pos[] = { -10.0f, 5.0f, -10.0f, 1.0f };
        GLfloat lamp_diffuse[] = { 0.9f + lampFlicker * 0.1f, 0.9f + lampFlicker * 0.1f, 0.8f, 1.0f };
        GLfloat lamp_ambient[] = { 0.2f, 0.2f, 0.15f, 1.0f };
        glLightfv(GL_LIGHT1, GL_POSITION, lamp1_pos);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, lamp_diffuse);
        glLightfv(GL_LIGHT1, GL_AMBIENT, lamp_ambient);
        glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0f);
        glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.1f);
        
        // Lamp 2
        GLfloat lamp2_pos[] = { 10.0f, 5.0f, -10.0f, 1.0f };
        glLightfv(GL_LIGHT2, GL_POSITION, lamp2_pos);
        glLightfv(GL_LIGHT2, GL_DIFFUSE, lamp_diffuse);
        glLightfv(GL_LIGHT2, GL_AMBIENT, lamp_ambient);
        glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.0f);
        glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.1f);
        
        // Lamp 3
        GLfloat lamp3_pos[] = { -10.0f, 5.0f, 10.0f, 1.0f };
        glLightfv(GL_LIGHT3, GL_POSITION, lamp3_pos);
        glLightfv(GL_LIGHT3, GL_DIFFUSE, lamp_diffuse);
        glLightfv(GL_LIGHT3, GL_AMBIENT, lamp_ambient);
        glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, 1.0f);
        glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, 0.1f);
        
        // Lamp 4
        GLfloat lamp4_pos[] = { 10.0f, 5.0f, 10.0f, 1.0f };
        glLightfv(GL_LIGHT4, GL_POSITION, lamp4_pos);
        glLightfv(GL_LIGHT4, GL_DIFFUSE, lamp_diffuse);
        glLightfv(GL_LIGHT4, GL_AMBIENT, lamp_ambient);
        glLightf(GL_LIGHT4, GL_CONSTANT_ATTENUATION, 1.0f);
        glLightf(GL_LIGHT4, GL_LINEAR_ATTENUATION, 0.1f);
    } else {
        // Turn off lamps during day
        GLfloat off[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        glLightfv(GL_LIGHT1, GL_DIFFUSE, off);
        glLightfv(GL_LIGHT2, GL_DIFFUSE, off);
        glLightfv(GL_LIGHT3, GL_DIFFUSE, off);
        glLightfv(GL_LIGHT4, GL_DIFFUSE, off);
    }
}

void Display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    
    // Set up camera
    float camX, camY, camZ;
    float lookX, lookY, lookZ;
    
    if (thirdPerson) {
        // Third-person camera
        float distance = isCrouching ? 4.0f : 5.0f;
        camX = playerX - sin(cameraYaw * PI / 180.0f) * distance;
        camY = playerY + 2.5f;
        camZ = playerZ - cos(cameraYaw * PI / 180.0f) * distance;
        lookX = playerX;
        lookY = playerY + 1.0f;
        lookZ = playerZ;
    } else {
        // First-person camera
        float eyeHeight = isCrouching ? 0.8f : 1.5f;
        camX = playerX;
        camY = playerY + eyeHeight;
        camZ = playerZ;
        lookX = playerX + sin(cameraYaw * PI / 180.0f);
        lookY = playerY + eyeHeight + tan(cameraPitch * PI / 180.0f);
        lookZ = playerZ + cos(cameraYaw * PI / 180.0f);
    }
    
    gluLookAt(camX, camY, camZ, lookX, lookY, lookZ, 0.0f, 1.0f, 0.0f);
    
    // Update lighting
    updateSunLight();
    updateLampLights();
    
    // Draw terrain
    drawTerrain();
    
    // Draw player (only in third-person)
    if (thirdPerson) {
        glPushMatrix();
        glTranslatef(playerX, playerY, playerZ);
        glRotatef(-cameraYaw, 0, 1, 0);
        if (isCrouching) {
            glScalef(1.0f, 0.7f, 1.0f);
        }
        drawPlayer();
        glPopMatrix();
    }
    
    // Draw houses (farmhouses and cottages)
    drawHouse(-25.0f, -20.0f, 1.2f);
    drawHouse(20.0f, -25.0f, 1.0f);
    drawHouse(-15.0f, 25.0f, 1.1f);
    drawHouse(30.0f, 15.0f, 0.9f);
    
    // Draw trees
    drawTree(-5.0f, -15.0f, 4.0f);
    drawTree(8.0f, -8.0f, 3.5f);
    drawTree(-12.0f, 5.0f, 4.5f);
    drawTree(15.0f, 8.0f, 3.8f);
    drawTree(-20.0f, -5.0f, 4.2f);
    drawTree(25.0f, -18.0f, 3.9f);
    drawTree(-8.0f, 20.0f, 4.1f);
    drawTree(18.0f, 22.0f, 3.7f);
    
    // Draw fences
    drawFence(-15.0f, -25.0f, 10.0f, 0);
    drawFence(15.0f, -20.0f, 8.0f, 45);
    drawFence(-10.0f, 15.0f, 12.0f, 90);
    drawFence(20.0f, 10.0f, 10.0f, 0);
    
    // Draw rocks
    drawRock(5.0f, -5.0f, 0.8f);
    drawRock(-8.0f, -12.0f, 1.0f);
    drawRock(12.0f, 3.0f, 0.7f);
    drawRock(-18.0f, 8.0f, 0.9f);
    drawRock(22.0f, -8.0f, 1.1f);
    
    // Draw crops (wheat and carrots)
    for (int i = 0; i < 5; i++) {
        drawCrop(-5.0f + i * 3, -30.0f);
        drawCrop(10.0f + i * 3, -35.0f);
    }
    
    // Draw grass blocks
    for (int i = -3; i <= 3; i++) {
        for (int j = -3; j <= 3; j++) {
            if ((i + j) % 2 == 0) {
                drawGrassBlock(i * 5.0f, j * 5.0f);
            }
        }
    }
    
    // Draw street lamps
    drawStreetLamp(-10.0f, -10.0f);
    drawStreetLamp(10.0f, -10.0f);
    drawStreetLamp(-10.0f, 10.0f);
    drawStreetLamp(10.0f, 10.0f);
    
    // Draw packages (collectibles)
    for (int i = 0; i < TOTAL_PACKAGES; i++) {
        if (!packages[i].collected) {
            drawPackage(packages[i].x, packages[i].y, packages[i].z);
        }
    }
    
    // Draw sky color based on time of day
    if (sunAngle > 90 && sunAngle < 270) {
        glClearColor(0.05f, 0.05f, 0.15f, 1.0f); // Night sky
    } else if (sunAngle < 100 || sunAngle > 260) {
        glClearColor(1.0f, 0.5f, 0.3f, 1.0f); // Sunset/sunrise
    } else {
        glClearColor(0.6f, 0.8f, 1.0f, 1.0f); // Day sky
    }
    
    glutSwapBuffers();
}

void updatePlayer() {
    // Movement
    float moveSpeed = isCrouching ? 0.05f : 0.1f;
    float moveDirX = 0, moveDirZ = 0;
    
    if (keyW) {
        moveDirX += sin(cameraYaw * PI / 180.0f);
        moveDirZ += cos(cameraYaw * PI / 180.0f);
    }
    if (keyS) {
        moveDirX -= sin(cameraYaw * PI / 180.0f);
        moveDirZ -= cos(cameraYaw * PI / 180.0f);
    }
    if (keyA) {
        moveDirX -= cos(cameraYaw * PI / 180.0f);
        moveDirZ += sin(cameraYaw * PI / 180.0f);
    }
    if (keyD) {
        moveDirX += cos(cameraYaw * PI / 180.0f);
        moveDirZ -= sin(cameraYaw * PI / 180.0f);
    }
    
    // Normalize movement
    float len = sqrt(moveDirX * moveDirX + moveDirZ * moveDirZ);
    if (len > 0) {
        moveDirX = (moveDirX / len) * moveSpeed;
        moveDirZ = (moveDirZ / len) * moveSpeed;
    }
    
    playerX += moveDirX;
    playerZ += moveDirZ;
    
    // Jumping physics
    if (isJumping) {
        playerVelY -= 0.02f; // Gravity
        playerY += playerVelY;
        
        if (playerY <= 1.5f) {
            playerY = 1.5f;
            playerVelY = 0;
            isJumping = false;
        }
    }
    
    // Check package collection
    for (int i = 0; i < TOTAL_PACKAGES; i++) {
        if (!packages[i].collected) {
            float dx = playerX - packages[i].x;
            float dz = playerZ - packages[i].z;
            float dist = sqrt(dx * dx + dz * dz);
            
            if (dist < 2.0f) {
                packages[i].collected = true;
                packagesCollected++;
                printf("Package collected! %d/%d\n", packagesCollected, TOTAL_PACKAGES);
            }
        }
    }
}

void Anim() {
    frameCount++;
    
    // Update sun rotation
    sunAngle += 0.05f;
    if (sunAngle >= 360.0f) sunAngle = 0.0f;
    
    // Update lamp flicker
    lampFlicker = sin(frameCount * 0.1f) * 0.5f + 0.5f;
    
    // Update player
    updatePlayer();
    
    glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
        case 'W':
            keyW = true;
            break;
        case 's':
        case 'S':
            keyS = true;
            break;
        case 'a':
        case 'A':
            keyA = true;
            break;
        case 'd':
        case 'D':
            keyD = true;
            break;
        case ' ':
            if (!isJumping) {
                isJumping = true;
                playerVelY = 0.3f;
            }
            break;
        case 'c':
        case 'C':
            isCrouching = !isCrouching;
            break;
        case 'v':
        case 'V':
            thirdPerson = !thirdPerson;
            break;
        case 27: // ESC
            exit(0);
            break;
    }
}

void KeyboardUp(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
        case 'W':
            keyW = false;
            break;
        case 's':
        case 'S':
            keyS = false;
            break;
        case 'a':
        case 'A':
            keyA = false;
            break;
        case 'd':
        case 'D':
            keyD = false;
            break;
    }
}

void Mouse(int x, int y) {
    if (firstMouse) {
        lastMouseX = x;
        lastMouseY = y;
        firstMouse = false;
    }
    
    float xoffset = x - lastMouseX;
    float yoffset = lastMouseY - y; // Reversed
    lastMouseX = x;
    lastMouseY = y;
    
    float sensitivity = 0.3f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    
    cameraYaw += xoffset;
    cameraPitch += yoffset;
    
    // Constrain pitch
    if (cameraPitch > MAX_PITCH) cameraPitch = MAX_PITCH;
    if (cameraPitch < MIN_PITCH) cameraPitch = MIN_PITCH;
}

void Reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)width / (float)height, 0.1f, 300.0f);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    
    glutCreateWindow("BlitzMail - Rural Level Scene");
    
    // Set up callbacks
    glutDisplayFunc(Display);
    glutIdleFunc(Anim);
    glutKeyboardFunc(Keyboard);
    glutKeyboardUpFunc(KeyboardUp);
    glutPassiveMotionFunc(Mouse);
    glutMotionFunc(Mouse);
    glutReshapeFunc(Reshape);
    
    // OpenGL initialization
    glClearColor(0.6f, 0.8f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
    
    // Set up lighting
    setupLighting();
    
    // Load 3D models
    loadAllModels();
    
    printf("BlitzMail - Rural Level Scene\n");
    printf("Controls:\n");
    printf("  WASD - Move\n");
    printf("  Space - Jump\n");
    printf("  C - Crouch\n");
    printf("  V - Toggle camera (first/third person)\n");
    printf("  Mouse - Look around\n");
    printf("  ESC - Exit\n");
    printf("\nCollect all %d packages!\n", TOTAL_PACKAGES);
    
    glutMainLoop();
    
    return 0;
}
