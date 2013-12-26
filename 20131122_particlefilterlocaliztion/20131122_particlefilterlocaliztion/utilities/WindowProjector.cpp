#include "WindowProjector.h"

WindowProjector::WindowProjector(void) {
    rangeOfScene = new double[6];
    for (int i = 0; i < 6; i++)
        rangeOfScene[i] = 0;
    viewMode = TOPVIEW_YPOS_XPOS;

    iViewModeListeners = 0;
    numIViewModeListener = 0;
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

void WindowProjector::setViewMode(ViewMode viewModeIn) {
    viewMode = viewModeIn;
    notifyViewModeListener();
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

void WindowProjector::registerViewModeListener(IViewModeListener *listener) {
    // If it is the first adding.
    if (iViewModeListeners == 0) {
        iViewModeListeners = new IViewModeListener *[1];
        iViewModeListeners[0] = listener;
        numIViewModeListener = 1;
    }
    // If it is not the first adding.
    else {
        IViewModeListener** iViewModeListenersTemp = new IViewModeListener *[numIViewModeListener + 1];

        // Copy from the old array.
        for (int i = 0; i < numIViewModeListener; i++)
            iViewModeListenersTemp[i] = iViewModeListeners[i];

        // Add the new element.
        numIViewModeListener += 1;
        iViewModeListenersTemp[numIViewModeListener - 1] = listener;
        
        // Make member array point to this array.
        iViewModeListeners = iViewModeListenersTemp;
    }
}

//void WindowProjector::unregisterViewModeListener(IViewModeListener *listener) {
//
//}

void WindowProjector::notifyViewModeListener() {
    for (int i = 0; i < numIViewModeListener; i++)
        iViewModeListeners[i]->onViewModeChanged(viewMode);
}
