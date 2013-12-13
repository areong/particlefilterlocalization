#include "DataDisplayerTableCubes.h"
#include <windows.h>
#include <glut.h>
#include "Viewer.h"
#include "../scene/Scene.h"

DataDisplayerTableCubes::DataDisplayerTableCubes(void) {
    scene = NULL;
    tableCubes = NULL;
    numPointsEachCube = NULL;
}

DataDisplayerTableCubes::~DataDisplayerTableCubes(void) {
}

void DataDisplayerTableCubes::setViewer(Viewer *viewerIn) {
    viewer = viewerIn;
}

void DataDisplayerTableCubes::display() {
    double sizeHalf = 0.005;
    double x;
    double y;

    for (int indexCube = indexCubeStart; indexCube < indexCubeEnd; indexCube++) {
        for (int i = 0; i < numPointsEachCube[indexCube]; i++) {
             // Calculate window position.
            viewer->projectAPointToWindow(tableCubes[indexCube][i*3  ],
                                          tableCubes[indexCube][i*3+1],
                                          tableCubes[indexCube][i*3+2],
                                          1, -1,
                                          1, -1,
                                          &x, &y);

            // Draw.
             glBegin(GL_QUADS);
                glColor3f(1.0f, 0.0f, 0.0f); // Red
                glVertex2f(x - sizeHalf, y - sizeHalf);
                glVertex2f(x + sizeHalf, y - sizeHalf);
                glVertex2f(x + sizeHalf, y + sizeHalf);
                glVertex2f(x - sizeHalf, y + sizeHalf);
             glEnd();
        }
    }
}

/* -------------------------------------------------------------
 * setScene
 * Set scene and store pointers to some necessary informations.
 * ----------------------------------------------- -------------- */
void DataDisplayerTableCubes::setScene(Scene *sceneIn) {
    scene = sceneIn;
    rangeOfScene = scene->getRangeOfScene();
    tableCubes = scene->getTableCubes();
    numCubes = scene->getNumCubes();
    numPointsEachCube = scene->getNumPointsEachCube();
    
    double xMin = rangeOfScene[1];
    double yMin = rangeOfScene[3];
    indexCubeStart = scene->XYZtoIndexOfCube(xMin + 0.01, yMin + 0.01, 0.01);
    indexCubeEnd = indexCubeStart + numCubes[0] * numCubes[1];
}