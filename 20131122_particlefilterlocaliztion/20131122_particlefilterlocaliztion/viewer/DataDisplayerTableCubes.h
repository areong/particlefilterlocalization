#ifndef VIEWER_DATADISPLAYERTABLECUBES_H
#define VIEWER_DATADISPLAYERTABLECUBES_H

#include "IDataDisplayer.h"
#include "../utilities/WindowProjector.h"
#include "../utilities/IViewModeListener.h"

// Forward declarations.
class Scene;

/* -----------------------------
 * DataDisplayerTableCubes
 * Display tableCubes generated from Scene object.
 * ----------------------------- */
class DataDisplayerTableCubes : public IDataDisplayer, public IViewModeListener {
public:
    DataDisplayerTableCubes(void);
    ~DataDisplayerTableCubes(void);
    
    void setViewer(Viewer *viewer);
    void display();
    void onKeyPressed(int key);

    void setScene(Scene *sceneIn);

    /* ------------------------------
       Override member function in IViewModeListener.
     * ------------------------------ */
    void onViewModeChanged(int viewMode);

private:
    Viewer *viewer;
    Scene *scene;
    double *rangeOfScene;
    double **tableCubes;
    int numCubes;
    int xNumCubes;
    int yNumCubes;
    int zNumCubes;
    int *numPointsEachCube;
    
    double maxZToDisplay;
    double minZToDisplay;
    int indexCubeStart;
    int indexCubeEnd;

    // Used in different viewing mode, to display cross view of scene.
    ViewMode viewMode;
    int indexZOfYXCrossView;
    int indexYOfZXCrossView;
    int indexXOfZYCrossView;
    bool whetherToDisplayACube(int indexCube);
    void increaseIndexOfCrossView();
    void decreaseIndexOfCrossView();
};

#endif
