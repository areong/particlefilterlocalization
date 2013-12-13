#include "WindowProjector.h"

WindowProjector::WindowProjector(void) {
    rangeOfScene = new double[6];
    for (int i = 0; i < 6; i++)
        rangeOfScene[i] = 0;
    viewMode = TOPVIEW_YPOS_XPOS;
}

WindowProjector::~WindowProjector(void) {
}

void WindowProjector::setRangeOfScene(double *rangeOfSceneIn) {
    for (int i = 0; i < 6; i++)
        rangeOfScene[i] = rangeOfSceneIn[i];
    lengthXRangeOfScene = rangeOfScene[0] - rangeOfScene[1];
    lengthYRangeOfScene = rangeOfScene[2] - rangeOfScene[3];
    lengthZRangeOfScene = rangeOfScene[4] - rangeOfScene[5];
}

void WindowProjector::setMode(ViewMode viewModeIn) {
    viewMode = viewModeIn;
}

void WindowProjector::projectAPoint(double xWorld, double yWorld, double zWorld,
                                    double xWindowMax, double xWindowMin,
                                    double yWindowMax, double yWindowMin,
                                    double *xWindow, double *yWindow) {
    switch (viewMode) {
    case TOPVIEW_YPOS_XPOS:
        *yWindow = (yWorld - rangeOfScene[3]) / lengthYRangeOfScene * 2 - 1;
        *xWindow = (xWorld - rangeOfScene[1]) / lengthXRangeOfScene * 2 - 1;
        break;
    case FRONTVIEW_ZPOS_XPOS:
        *yWindow = (zWorld - rangeOfScene[5]) / lengthZRangeOfScene * 2 - 1;
        *xWindow = (xWorld - rangeOfScene[1]) / lengthXRangeOfScene * 2 - 1;
        break;
    case RIGHTVIEW_ZPOS_YPOS:
        *yWindow = (zWorld - rangeOfScene[5]) / lengthZRangeOfScene * 2 - 1;
        *xWindow = (yWorld - rangeOfScene[3]) / lengthYRangeOfScene * 2 - 1;
        break;
    default:
        // Use TOPVIEW_YPOS_XPOS
        *xWindow = (xWorld - rangeOfScene[1]) / lengthXRangeOfScene * 2 - 1;
        *yWindow = (yWorld - rangeOfScene[3]) / lengthYRangeOfScene * 2 - 1;
        break;
    }    
}