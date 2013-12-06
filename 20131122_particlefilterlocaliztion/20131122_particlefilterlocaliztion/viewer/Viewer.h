/* ----------------------------------------
 * Learned from SimpleViewer of OpenNI2.
 * ---------------------------------------- */

#ifndef VIEWER_H
#define VIEWER_H

#include <windows.h>
#include <GL/gl.h>
#include <glut.h>

class Viewer {
public:
    Viewer(char* title);
    ~Viewer(void);
    int initialize(int argc, char** argv);
    void runMainLoop();
    void setCallbackKeyEsc(void (*cb)());

protected:
    virtual void display();
    virtual void displayPostDraw(){};   // Overload to draw over the screen image
    virtual void onKey(unsigned char key, int x, int y);

    void initOpenGLCallbacks();

private:
    static Viewer* self;
    static void glutIdle();
    static void glutDisplay();
    static void glutKeyboard(unsigned char key, int x, int y);

    char *titleWindow;

    void (*callbackKeyEsc) ();
};

#endif