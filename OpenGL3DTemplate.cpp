#include <glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

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

// Forward declarations
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

// Simple OBJ loader placeholder (using primitives instead due to complexity)
void loadModel(const char* filename) {
    // In a full implementation, this would parse OBJ files
    // For this template, we'll use OpenGL primitives styled like models
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
    
    // Bag strap
    glColor3f(0.5f, 0.3f, 0.1f);
    glPushMatrix();
    glTranslatef(-0.2f, 0.3f, 0);
    glRotatef(45, 0, 0, 1);
    glScalef(0.05f, 0.8f, 0.05f);
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
    
    glPopMatrix();
}

void drawTree(float x, float z, float height) {
    glPushMatrix();
    glTranslatef(x, 0, z);
    
    // Trunk
    glColor3f(0.4f, 0.25f, 0.1f); // Brown
    glPushMatrix();
    glTranslatef(0, height * 0.3f, 0);
    glRotatef(-90, 1, 0, 0);
    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, 0.3f, 0.5f, height * 0.6f, 10, 10);
    gluDeleteQuadric(quad);
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
    glColor3f(0.5f, 0.5f, 0.5f); // Gray
    
    // Irregular rock shape
    glPushMatrix();
    glScalef(size, size * 0.6f, size * 0.8f);
    glutSolidSphere(1.0f, 8, 8);
    glPopMatrix();
    
    glPopMatrix();
}

void drawCrop(float x, float z) {
    glPushMatrix();
    glTranslatef(x, 0, z);
    
    // Wheat/carrot stalks
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
    
    glPopMatrix();
}

void drawGrassBlock(float x, float z) {
    glPushMatrix();
    glTranslatef(x, 0.5f, z);
    
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
    
    glPopMatrix();
}

void drawStreetLamp(float x, float z) {
    glPushMatrix();
    glTranslatef(x, 0, z);
    
    // Lamp post
    glColor3f(0.2f, 0.2f, 0.2f); // Dark gray metal
    glPushMatrix();
    glTranslatef(0, 2.5f, 0);
    glRotatef(-90, 1, 0, 0);
    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, 0.1f, 0.15f, 5.0f, 12, 12);
    gluDeleteQuadric(quad);
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
    float sunX = cos(sunAngle * 3.14159f / 180.0f) * 50.0f;
    float sunY = sin(sunAngle * 3.14159f / 180.0f) * 50.0f;
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
        camX = playerX - sin(cameraYaw * 3.14159f / 180.0f) * distance;
        camY = playerY + 2.5f;
        camZ = playerZ - cos(cameraYaw * 3.14159f / 180.0f) * distance;
        lookX = playerX;
        lookY = playerY + 1.0f;
        lookZ = playerZ;
    } else {
        // First-person camera
        float eyeHeight = isCrouching ? 0.8f : 1.5f;
        camX = playerX;
        camY = playerY + eyeHeight;
        camZ = playerZ;
        lookX = playerX + sin(cameraYaw * 3.14159f / 180.0f);
        lookY = playerY + eyeHeight + tan(cameraPitch * 3.14159f / 180.0f);
        lookZ = playerZ + cos(cameraYaw * 3.14159f / 180.0f);
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
        moveDirX += sin(cameraYaw * 3.14159f / 180.0f);
        moveDirZ += cos(cameraYaw * 3.14159f / 180.0f);
    }
    if (keyS) {
        moveDirX -= sin(cameraYaw * 3.14159f / 180.0f);
        moveDirZ -= cos(cameraYaw * 3.14159f / 180.0f);
    }
    if (keyA) {
        moveDirX -= cos(cameraYaw * 3.14159f / 180.0f);
        moveDirZ += sin(cameraYaw * 3.14159f / 180.0f);
    }
    if (keyD) {
        moveDirX += cos(cameraYaw * 3.14159f / 180.0f);
        moveDirZ -= sin(cameraYaw * 3.14159f / 180.0f);
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
    if (cameraPitch > 89.0f) cameraPitch = 89.0f;
    if (cameraPitch < -89.0f) cameraPitch = -89.0f;
}

void Reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)width / (float)height, 0.1f, 300.0f);
    glMatrixMode(GL_MODELVIEW);
}

void main(int argc, char** argv) {
    glutInit(&argc, argv);
    
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GL_DEPTH);
    
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
}
