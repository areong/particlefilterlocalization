#ifndef VIEWER_IDATADISPLAYER_H
#define VIEWER_IDATADISPLAYER_H

class Viewer;

/* -----------------------------------------------------
 * IDataDisplayer
 *
 * An interface for displayer used to display data in Viewer
 * by polymorphism mechanism.
 * -----------------------------------------------------  */
class IDataDisplayer {
public:
    virtual void display() = 0; // pure virtual

    /**
    Set viewer to access Viewer's methods.
    Called by Viewer when calling Viewer::addDataDisplayer.
    */
    virtual void setViewer(Viewer *viewer) = 0;

    virtual void onKeyPressed(int key) = 0;
};

#endif