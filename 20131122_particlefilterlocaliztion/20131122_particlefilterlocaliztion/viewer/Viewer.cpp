/* ----------------------------------------
 * Learned from SimpleViewer of OpenNI2.
 * ---------------------------------------- */

#include "Viewer.h"
#include <windows.h>
#include <glut.h>
#include "IDataDisplayer.h"

Viewer::Viewer(char* title) {
    self = this;
    titleWindow = title;
    iDataDisplayers = NULL;
    numIDataDisplayers = 0;
}

Viewer::~Viewer(void) {
    self = NULL;
}

int Viewer::initialize(int argc, char** argv) {
    glutInit(&argc, argv);
    //glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(500, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(titleWindow);

    initOpenGLCallbacks();

    //glDisable(GL_DEPTH_TEST);
    //glEnable(GL_TEXTURE_2D);

    // Set the clear color. It will not be changed in the main loop.
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque

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

void Viewer::addDataDisplayer(IDataDisplayer* iDataDisplayer) {
    // If it is the first adding.
    if (iDataDisplayers == NULL) {
        iDataDisplayers = new IDataDisplayer *[1];
        iDataDisplayers[0] = iDataDisplayer;
        numIDataDisplayers = 1;
    }
    // If it is not the first adding.
    else {
        IDataDisplayer** iDataDisplayersTemp = new IDataDisplayer *[numIDataDisplayers + 1];

        // Copy from the old array.
        for (int i = 0; i < numIDataDisplayers; i++)
            iDataDisplayersTemp[i] = iDataDisplayers[i];

        // Add the new element.
        numIDataDisplayers += 1;
        iDataDisplayersTemp[numIDataDisplayers - 1] = iDataDisplayer;
        
        // Make member array point to this array.
        iDataDisplayers = iDataDisplayersTemp;
    }
}

void Viewer::removeAllDataDisplayers() {
    iDataDisplayers = NULL;
}

/* ----------------------------------------
 * Protected:
 * ---------------------------------------- */

void Viewer::display() {
    // Call the callback before drawing.
    callbackInMainLoopBeforeDrawing();

    glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer (background)

    // Display by every IDataDisplay.
    for (int i = 0; i < numIDataDisplayers; i++)
        iDataDisplayers[i]->display();
 
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