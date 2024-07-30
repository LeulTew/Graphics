#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "stb-master/stb_image.h"

// Global variables for mouse interaction
bool isMouseDragging = false;
int lastMouseX = 0;
int lastMouseY = 0;

const int windowWidth = 800;
const int windowHeight = 600;

float angle = 0.0;

float zoom = -150.0f; // Initialize the zoom value
float rotationX = 0.0;
float rotationY = 0.0;

// Global Variables for Orbits
float mercuryOrbit = 0.0;
float venusOrbit = 0.0;
float earthOrbit = 0.0;
float moonOrbit = 0.0;
float marsOrbit = 0.0;
float jupiterOrbit = 0.0;
float saturnOrbit = 0.0;
float uranusOrbit = 0.0;
float neptuneOrbit = 0.0;

GLuint sunTexture, mercuryTexture, venusTexture, earthTexture, moonTexture, marsTexture, jupiterTexture, saturnTexture, uranusTexture, neptuneTexture;
GLuint saturnRingTexture, starsTexture;

GLuint loadTexture(const char *filename)
{
    int width, height, nrChannels;
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (!data)
    {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    if (nrChannels == 3)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    else if (nrChannels == 4)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
    else
    {
        std::cerr << "Unsupported number of channels: " << nrChannels << std::endl;
        stbi_image_free(data);
        return 0;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return textureID;
}

void loadTextures()
{
    sunTexture = loadTexture("Textures/sun.png");
    mercuryTexture = loadTexture("Textures/mercury.png");
    venusTexture = loadTexture("Textures/venus.png");
    earthTexture = loadTexture("Textures/earth.png");
    moonTexture = loadTexture("Textures/moon.png");
    marsTexture = loadTexture("Textures/mars.png");
    jupiterTexture = loadTexture("Textures/jupiter.png");
    saturnTexture = loadTexture("Textures/saturn.png");
    saturnRingTexture = loadTexture("Textures/saturn_rings.png");
    uranusTexture = loadTexture("Textures/uranus.png");
    neptuneTexture = loadTexture("Textures/neptune.png");
    starsTexture = loadTexture("Textures/stars.png");

    // Simple error checking: replace 0 with whatever your loadTexture function returns on failure
    if (!sunTexture || !mercuryTexture || !venusTexture || !earthTexture || !moonTexture || !marsTexture || !jupiterTexture || !saturnTexture || !saturnRingTexture || !uranusTexture || !neptuneTexture || !starsTexture)
    {
        std::cerr << "Failed to load one or more textures" << std::endl;
    }
}

void init()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    loadTextures();

    GLfloat lightPos[] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat lightAmb[] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat lightDiff[] = {0.9f, 0.9f, 0.9f, 1.0f};
    GLfloat lightSpec[] = {1.0f, 1.0f, 1.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiff);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpec);

    glEnable(GL_LIGHT1);
    GLfloat light1Pos[] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat light1Diff[] = {0.5f, 0.5f, 0.5f, 1.0f};
    glLightfv(GL_LIGHT1, GL_POSITION, light1Pos);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1Diff);
}

void drawSphere(GLuint textureID, float radius)
{
    if (textureID != 0)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glColor3f(1.0, 1.0, 1.0);
    }
    else
    {
        glDisable(GL_TEXTURE_2D);
        glColor3f(1.0, 1.0, 0.0); // Example default color
    }

    GLUquadric *quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    gluQuadricDrawStyle(quad, GLU_FILL);
    gluQuadricNormals(quad, GLU_SMOOTH);
    gluQuadricOrientation(quad, GLU_OUTSIDE);
    gluSphere(quad, radius, 50, 50);
    gluDeleteQuadric(quad);

    if (textureID != 0)
    {
        glDisable(GL_TEXTURE_2D);
    }
}

void drawSaturn(GLuint textureID, GLuint ringTextureID, float radius)
{
    drawSphere(textureID, radius);

    if (ringTextureID != 0)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, ringTextureID);
        glColor4f(1.0, 1.0, 1.0, 1.0);

        GLUquadric *ring = gluNewQuadric();
        gluQuadricTexture(ring, GL_TRUE);
        gluQuadricDrawStyle(ring, GLU_FILL);
        gluQuadricNormals(ring, GLU_SMOOTH);

        glPushMatrix();
        glRotatef(90.0, 1.0, 0.0, 0.0);
        gluDisk(ring, radius * 1.2, radius * 2.5, 50, 1); // Adjusted ring size
        glPopMatrix();

        gluDeleteQuadric(ring);
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);
    }

    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        std::cerr << "OpenGL error: " << gluErrorString(err) << std::endl;
    }
}

void drawSkybox()
{
    if (starsTexture != 0)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, starsTexture);
        glColor3f(1.0, 1.0, 1.0);

        float size = 400.0f; // Adjust the size of the skybox if needed

        glPushMatrix();
        glBegin(GL_QUADS);

        // Front Face
        glTexCoord2f(0.0, 0.0);
        glVertex3f(-size, -size, -size);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(size, -size, -size);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(size, size, -size);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(-size, size, -size);

        // Back Face
        glTexCoord2f(0.0, 0.0);
        glVertex3f(size, -size, size);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(-size, -size, size);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(-size, size, size);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(size, size, size);

        // Top Face
        glTexCoord2f(0.0, 0.0);
        glVertex3f(-size, size, -size);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(size, size, -size);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(size, size, size);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(-size, size, size);

        // Bottom Face
        glTexCoord2f(0.0, 0.0);
        glVertex3f(-size, -size, size);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(size, -size, size);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(size, -size, -size);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(-size, -size, -size);

        // Right face
        glTexCoord2f(0.0, 0.0);
        glVertex3f(size, -size, -size);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(size, -size, size);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(size, size, size);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(size, size, -size);

        // Left Face
        glTexCoord2f(0.0, 0.0);
        glVertex3f(-size, -size, size);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(-size, -size, -size);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(-size, size, -size);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(-size, size, size);

        glEnd();
        glPopMatrix();

        glDisable(GL_TEXTURE_2D);
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0, 0.0, zoom);
    glRotatef(rotationX, 1.0, 0.0, 0.0);
    glRotatef(rotationY, 0.0, 1.0, 0.0);

    // Draw surrounding stars using skybox
    drawSkybox();

    // Draw the Sun
    glPushMatrix();
    glRotatef(angle, 0.0, 1.0, 0.0);
    drawSphere(sunTexture, 20.0); // Adjusted sun size
    glPopMatrix();

    // Draw Mercury
    glPushMatrix();
    glRotatef(mercuryOrbit, 0.0, 1.0, 0.0); // Mercury's orbit around the Sun
    glTranslatef(30.0, 0.0, 0.0);           // Distance from the Sun
    drawSphere(mercuryTexture, 0.38);
    glPopMatrix();

    // Draw Venus
    glPushMatrix();
    glRotatef(venusOrbit, 0.0, 1.0, 0.0); // Venus's orbit around the Sun
    glTranslatef(50.0, 0.0, 0.0);         // Distance from the Sun
    drawSphere(venusTexture, 0.95);
    glPopMatrix();

    // Draw Earth and Moon
    glPushMatrix();
    glRotatef(earthOrbit, 0.0, 1.0, 0.0); // Earth's orbit around the Sun
    glTranslatef(70.0, 0.0, 0.0);         // Distance from the Sun
    drawSphere(earthTexture, 1.0);

    glPushMatrix();
    glRotatef(moonOrbit, 0.0, 1.0, 0.0); // Moon's orbit around the Earth
    glTranslatef(3.0, 0.0, 0.0);         // Distance from the Earth
    drawSphere(moonTexture, 0.27);
    glPopMatrix();

    glPopMatrix();

    // Draw Mars
    glPushMatrix();
    glRotatef(marsOrbit, 0.0, 1.0, 0.0); // Mars's orbit around the Sun
    glTranslatef(100.0, 0.0, 0.0);       // Distance from the Sun
    drawSphere(marsTexture, 0.53);
    glPopMatrix();

    // Draw Jupiter
    glPushMatrix();
    glRotatef(jupiterOrbit, 0.0, 1.0, 0.0); // Jupiter's orbit around the Sun
    glTranslatef(140.0, 0.0, 0.0);          // Distance from the Sun
    drawSphere(jupiterTexture, 11.0);
    glPopMatrix();

    // Draw Saturn
    glPushMatrix();
    glRotatef(saturnOrbit, 0.0, 1.0, 0.0); // Saturn's orbit around the Sun
    glTranslatef(190.0, 0.0, 0.0);         // Distance from the Sun
    drawSaturn(saturnTexture, saturnRingTexture, 9.0);
    glPopMatrix();

    // Draw Uranus
    glPushMatrix();
    glRotatef(uranusOrbit, 0.0, 1.0, 0.0); // Uranus's orbit around the Sun
    glTranslatef(250.0, 0.0, 0.0);         // Distance from the Sun
    drawSphere(uranusTexture, 4.0);
    glPopMatrix();

    // Draw Neptune
    glPushMatrix();
    glRotatef(neptuneOrbit, 0.0, 1.0, 0.0); // Neptune's orbit around the Sun
    glTranslatef(300.0, 0.0, 0.0);          // Distance from the Sun
    drawSphere(neptuneTexture, 3.9);
    glPopMatrix();

    glutSwapBuffers();
}

void update(int value)
{
    angle += 0.5;        // Increment Sun's rotation angle
    mercuryOrbit += 1.5; // Increment Mercury's orbit
    venusOrbit += 1.2;   // Increment Venus' orbit
    earthOrbit += 1.0;   // Increment Earth's orbit
    moonOrbit += 6.0;    // Increment Moon's orbit
    marsOrbit += 0.9;    // Increment Mars' orbit
    jupiterOrbit += 0.7; // Increment Jupiter's orbit
    saturnOrbit += 0.5;  // Increment Saturn's orbit
    uranusOrbit += 0.3;  // Increment Uranus' orbit
    neptuneOrbit += 0.2; // Increment Neptune's orbit

    // Keep orbit angles in the range of 0-360
    mercuryOrbit = fmod(mercuryOrbit, 360.0);
    venusOrbit = fmod(venusOrbit, 360.0);
    earthOrbit = fmod(earthOrbit, 360.0);
    moonOrbit = fmod(moonOrbit, 360.0);
    marsOrbit = fmod(marsOrbit, 360.0);
    jupiterOrbit = fmod(jupiterOrbit, 360.0);
    saturnOrbit = fmod(saturnOrbit, 360.0);
    uranusOrbit = fmod(uranusOrbit, 360.0);
    neptuneOrbit = fmod(neptuneOrbit, 360.0);

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // Update every 16ms (approx. 60 FPS)
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)w / (double)h, 1.0, 1000.0); // Adjust the far clipping plane if needed
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27: // Escape key
        exit(0);
        break;
    case 'w':
        rotationX -= 5.0;
        break;
    case 's':
        rotationX += 5.0;
        break;
    case 'a':
        rotationY -= 5.0;
        break;
    case 'd':
        rotationY += 5.0;
        break;
    case '=':
        zoom += 2.0;
        break;
    case '-':
        zoom -= 2.0;
        break;
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            isMouseDragging = true;
            lastMouseX = x;
            lastMouseY = y;
        }
        else if (state == GLUT_UP)
        {
            isMouseDragging = false;
        }
    }
}

void motion(int x, int y)
{
    if (isMouseDragging)
    {
        rotationY += (x - lastMouseX) * 0.5;
        rotationX += (y - lastMouseY) * 0.5;
        lastMouseX = x;
        lastMouseY = y;
        glutPostRedisplay();
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Solar System");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutTimerFunc(25, update, 0); // Set up the update function
    glutMainLoop();
    return 0;
}
