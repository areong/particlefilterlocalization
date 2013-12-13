/* ----------------------------------------
 * Learned from SimpleViewer of OpenNI2.
 * ---------------------------------------- */

#ifndef VIEWER_VIEWER_H
#define VIEWER_VIEWER_H

// Foward declarations
class IDataDisplayer;
class WindowProjector;

class Viewer {
public:
    Viewer(char* title);
    ~Viewer(void);
    int initialize(int argc, char** argv);
    void runMainLoop();
    void setCallbackInMainLoopBeforeDrawing(void (*cb)());
    void setCallbackKeyEsc(void (*cb)());

    void addDataDisplayer(IDataDisplayer* iDataDisplayer);
    void removeAllDataDisplayers();

    void setWindowProjector(WindowProjector *windowProjector);
    
    /**
    Project a point in world coordinates to window coordinates.
    Viewer::setWindowProjector(...) should be called previously.
    In window coordinates, the visible region is
        -1 < xWindow < 1
        -1 < yWindow < 1
        (<= or < doesn't matter(?)).
    Unit is half of width/height of window, which changes as window's size changes.
    It is OpenGL's unit.
    */
    void projectAPointToWindow(double xWorld, double yWorld, double zWorld,
                               double xWindowMax, double xWindowMin,
                               double yWindowMax, double yWindowMin,
                               double *xWindow, double *yWindow);

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

    // An array to store IDataDisplayer pointers.
    IDataDisplayer** iDataDisplayers;
    int numIDataDisplayers;

    WindowProjector *windowProjector;
};

#endif