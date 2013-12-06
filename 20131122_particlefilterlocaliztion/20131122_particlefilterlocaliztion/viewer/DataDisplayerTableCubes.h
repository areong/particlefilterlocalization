#ifndef VIEWER_DATADISPLAYERTABLECUBES_H
#define VIEWER_DATADISPLAYERTABLECUBES_H

#include "IDataDisplayer.h"

// Forward declarations.
class Scene;

/* -----------------------------
 * DataDisplayerTableCubes
 * Display tableCubes generated from Scene object.
 * ----------------------------- */
class DataDisplayerTableCubes : public IDataDisplayer {
public:
    DataDisplayerTableCubes(void);
    ~DataDisplayerTableCubes(void);
    void display();

    void setScene(Scene *sceneIn);

private:
    Scene *scene;
    double *rangeOfScene;
    double **tableCubes;
    int *numCubes;
    int *numPointsEachCube;
    
    double maxZToDisplay;
    double minZToDisplay;
    int indexCubeStart;
    int indexCubeEnd;
};

#endif
