#ifndef UTILITIES_WINDOWPROJECTOR_H
#define UTILITIES_WINDOWPROJECTOR_H

#include "IViewModeListener.h"

typedef enum {
    TOPVIEW_YPOS_XPOS = 1,
    FRONTVIEW_ZPOS_XPOS = 2,
    RIGHTVIEW_ZPOS_YPOS = 3,
} ViewMode;

/**
Project points in world coordinates to the window on screen.
*/
class WindowProjector {
public:
    WindowProjector(void);
    ~WindowProjector(void);

    /**
    Set range of scene. 
    @param [in] rangeOfScene: Double array of length 6.
    [xMax, xMin, yMax, yMin, zMax, zMin].
    */
    void setRangeOfScene(double *rangeOfScene);

    /**
    Set viewing mode.
    @param [in] viewMode:
    There are 3 different viewing modes:
        TOPVIEW_YPOS_XPOS
        FRONTVIEW_ZPOS_XPOS
        RIGHTVIEW_ZPOS_YPOS
    In naming,
    "aPOS_bNEG" means the Y axis of window equals to positive 'a' axis of scene,
    and the X axis of window equals to negative 'b' axis of scene.
    'a' and 'b' can be X, Y or Z.
    */
    void setViewMode(ViewMode viewMode);
    
    /**
    Project a point in world coordinates to window coordinates.
    In window coordinates, the visible region is
        -1 < xWindow < 1
        -1 < yWindow < 1
        (<= or < doesn't matter(?)).
    Unit is half of width/height of window, which changes as window's size changes.
    It is OpenGL's unit.
    */
    void projectAPoint(double xWorld, double yWorld, double zWorld,
                       double xWindowMax, double xWindowMin,
                       double yWindowMax, double yWindowMin,
                       double *xWindow, double *yWindow);

    void registerViewModeListener(IViewModeListener *listener);
    //void unregisterViewModeListener(IViewModeListener *listener);
    void notifyViewModeListener();

private:
    double *rangeOfScene; // [xMax, xMin, yMax, yMin, zMax, zMin]
    double lengthXRangeOfScene;
    double lengthYRangeOfScene;
    double lengthZRangeOfScene;

    ViewMode viewMode;

    IViewModeListener** iViewModeListeners;
    int numIViewModeListener;
};

#endif