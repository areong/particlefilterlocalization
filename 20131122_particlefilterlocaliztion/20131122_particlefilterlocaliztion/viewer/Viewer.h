/* ----------------------------------------
 * Learned from SimpleViewer of OpenNI2.
 * ---------------------------------------- */

#ifndef VIEWER_H
#define VIEWER_H

#include <windows.h>
#include <glut.h>

class Viewer {
public:
    Viewer(char* title);
    ~Viewer(void);
    int initialize(int argc, char** argv);
    void runMainLoop();
    void setCallbackInMainLoopBeforeDrawing(void (*cb)());
    void setCallbackKeyEsc(void (*cb)());

protected:
    virtual void display();
    virtual void onKey(unsigned char key, int x, int y);

    void initOpenGLCallbacks();

private:
    static Viewer* self;
    static void glutIdle();
    static void glutDisplay();
    static void glutKeyboard(unsigned char key, int x, int y);

    char *titleWindow;

    void (*callbackInMainLoopBeforeDrawing)();
    void (*callbackKeyEsc)();
};

#endif