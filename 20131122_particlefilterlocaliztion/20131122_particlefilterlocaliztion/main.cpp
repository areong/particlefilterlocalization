#include <stdlib.h>
#include <iostream>
#include "scene/Scene.h"

int main() {
    // Create Scene object.
    Scene sceneMain;

    // Initialize.
    // Parameters:
    //  1. File path of the ply file.
    //  2. Length of edge of each cube, used in cutting the scene into cubes. Unit is meter.
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
    double distance = sceneMain.calcDistanceFromPointAlongLine(-1, 0, 0, -0.5, 0.5, 1);
    cout << distance << endl;
    system("pause");
    return 0;
}