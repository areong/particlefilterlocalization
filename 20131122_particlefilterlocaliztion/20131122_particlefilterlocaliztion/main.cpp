#include <stdlib.h>
#include <iostream>
#include "opennicamera/OpenniCamera.h"
#include "scene/Scene.h"
#include "viewer/DataDisplayerTableCubes.h"
#include "viewer/Viewer.h"
using namespace std;

void callbackInViewerMainLoopBeforeDrawing();
void callbackAfterViewerMainLoop();

// Create Scene object.
Scene sceneMain;

// Create OpenniCamera object.
//OpenniCamera camera;

int main(int argc, char** argv) {
     

    // Initialize.
    // Parameters:
    //  1. File path of the ply file.
    //  2. Length of edge of cubes, used in cutting the scene into cubes. Unit is meter.
    sceneMain.initialize("res/captured.ply", 1);

    // Set the distance defining whether a point touches a line,
    // used in calculating distances later.
    // Parameters:
    //  1. The distance. Unit is meter.
    sceneMain.setDistanceDefiningPointTouchesLine(0.1);

    // Calculate the distance from a point to the scene along a line.
    // Parameters:
    //  1-3: Point's x,y,z coordinates.
    //  4-6: Line's direction vector. Not being a unit vector is okay.
    // Return:
    //  The distance. Unit is meter.
    //  Returns -1 if point out of range.
    double distance = sceneMain.calcDistanceFromPointAlongLine(-1, 0, 0, -0.5, 0.5, 1);
    cout << distance << endl;

    // Get the range of scene.
    // Return:
    //  A double array, which is [xMax, xMin, yMax, yMin, zMax, zMin].
    //  These are integral multiples of the edge of cubes, which is set previously.
    // CAUTION: DO NOT CHANGE THE CONTENT OF IT!
    double *rangeOfScene = sceneMain.getRangeOfScene();
    for (int i = 0; i < 3; i++)
        cout << i << ": "<< '\t' << rangeOfScene[i*2] << '\t' << rangeOfScene[i*2+1] << endl;

    // Get the table of cubes.
    // Return:
    //  A double two-dimensional array.
    //  First dimension is the array of cubes.
    //  Second dimension is an array of points' coordinates, i.e., [p1x, p1y, p1z, p2x, p2y, p2z,...]
    // CAUTION: DO NOT CHANGE THE CONTENT OF IT!
    double **tableCubes = sceneMain.getTableCubes();

    // Get number of points of each cube.
    // Return:
    //  A integer array. The index is the same as the cube.
    // CAUTION: DO NOT CHANGE THE CONTENT OF IT!
    int *numPointsEachCube = sceneMain.getNumPointsEachCube();
    if (numPointsEachCube[0] > 0)
        cout << tableCubes[0][0] << '\t'<< tableCubes[0][1] << '\t' << tableCubes[0][2] << endl;
    else
        cout << "No point in cube no.0." << endl;

    
    //camera.initialize();
    //camera.setSamplingMethod(SAMPLING_GRID, 4, 3);
    //camera.takeNewDepthPhoto(-1, 0, 0, 0, 0, 1);
    //int *photo = camera.getDepthPhoto();
    
    DataDisplayerTableCubes dataDisplayerTableCubes;
    dataDisplayerTableCubes.setScene(&sceneMain);

    Viewer viewerMain("Particle Filter Localization");
    viewerMain.initialize(argc, argv);
    viewerMain.setCallbackInMainLoopBeforeDrawing(&callbackInViewerMainLoopBeforeDrawing);
    viewerMain.setCallbackKeyEsc(&callbackAfterViewerMainLoop);
    
    viewerMain.addDataDisplayer(&dataDisplayerTableCubes);

    viewerMain.runMainLoop();

    

    system("pause");
    return 0;
}

void callbackInViewerMainLoopBeforeDrawing() {
    //cout << "In glutMainLoop() before drawing." << endl;
}

void callbackAfterViewerMainLoop() {
    cout << "After glutMainLoop()." << endl;
    //camera.shutdown();
}