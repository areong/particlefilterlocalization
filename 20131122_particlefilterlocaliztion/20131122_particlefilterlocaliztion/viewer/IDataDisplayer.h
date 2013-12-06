#ifndef VIEWER_IDATADISPLAYER_H
#define VIEWER_IDATADISPLAYER_H

/* -----------------------------------------------------
 * IDataDisplayer
 *
 * An interface for displayer used to display data in Viewer
 * by polymorphism mechanism.
 * -----------------------------------------------------  */
class IDataDisplayer {
public:
    virtual void display() = 0; // pure virtual
};

#endif