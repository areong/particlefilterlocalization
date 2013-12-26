#ifndef UTILITIES_VIEWMODELISTENER_H
#define UTILITIES_VIEWMODELISTENER_H

class IViewModeListener {
public:
    // Notes:
    // If use ViewMode, needs to include "WindowProjector.h",
    // than circular dependency occurs.
    // It can be solved by using forward declaration
    // but it is not applicable to typedef.
    // Thus use int here.
    // Ref: http://stackoverflow.com/questions/15715882/error-c2061-syntax-error-identifier
    virtual void onViewModeChanged(int viewMode) = 0;
};

#endif