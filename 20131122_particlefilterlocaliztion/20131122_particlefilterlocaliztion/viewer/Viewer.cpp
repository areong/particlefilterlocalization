/* ----------------------------------------
 * Learned from SimpleViewer of OpenNI2.
 * ---------------------------------------- */

#include "Viewer.h"

Viewer::Viewer(char* title) {
    self = this;
    titleWindow = title;
}

Viewer::~Viewer(void) {
    self = NULL;
}

int Viewer::initialize(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(500, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(titleWindow);

    initOpenGLCallbacks();

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    return 0;
}

void Viewer::runMainLoop() {
    glutMainLoop();
}

/* ------------------------------------------
 * setCallbackInMainLoopBeforeDrawing
 * Set the callback function to be called each time in main loop
 * before drawing.
 * ------------------------------------------ */
void Viewer::setCallbackInMainLoopBeforeDrawing(void (*cb)()) {
    callbackInMainLoopBeforeDrawing = cb;
}

/* ------------------------------------------
 * setCallbackKeyEsc
 * Set the callback function to be called when 'esc' is pressed, 
 * before exiting the whole program.
 *
 * If we call exit(1) at pressing 'esc', it jumps out of the glutMainLoop()
 * but never goes back to main() as we want. Thus using the callback function mechanism
 * makes us able to do something before the program is forced to shutdown by glut.
 * ------------------------------------------ */
void Viewer::setCallbackKeyEsc(void (*cb)()) {
    callbackKeyEsc = cb;
}

/* ----------------------------------------
 * Protected:
 * ---------------------------------------- */

void Viewer::display() {
    // Call the callback before drawing.
    callbackInMainLoopBeforeDrawing();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer (background)
 
    // Draw a Red 1x1 Square centered at origin
    glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
        glColor3f(1.0f, 0.0f, 0.0f); // Red
        glVertex2f(-0.5f, -0.5f);    // x, y
        glVertex2f( 0.5f, -0.5f);
        glVertex2f( 0.5f,  0.5f);
        glVertex2f(-0.5f,  0.5f);
    glEnd();
 
    glFlush();  // Render now
}

void Viewer::onKey(unsigned char key, int /*x*/, int /*y*/) {
    switch (key) {
    case 27:
        if (callbackKeyEsc != NULL)
            callbackKeyEsc();
        exit (1);
    }
}

void Viewer::initOpenGLCallbacks() {
    glutKeyboardFunc(glutKeyboard);
    glutDisplayFunc(glutDisplay);
    glutIdleFunc(glutIdle);
}

/* ----------------------------------------
 * Privated:
 * ---------------------------------------- */
Viewer* Viewer::self = NULL;

void Viewer::glutIdle() {
    glutPostRedisplay();
}

void Viewer::glutDisplay() {
    Viewer::self->display();
}

void Viewer::glutKeyboard(unsigned char key, int x, int y) {
    Viewer::self->onKey(key, x, y);
}