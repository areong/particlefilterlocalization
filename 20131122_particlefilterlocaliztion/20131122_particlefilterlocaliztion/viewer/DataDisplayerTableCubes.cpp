#include "DataDisplayerTableCubes.h"
#include <windows.h>
#include <glut.h>
#include "Viewer.h"
#include "../scene/Scene.h"

#include <iostream>
using namespace std;

DataDisplayerTableCubes::DataDisplayerTableCubes(void) {
    scene = NULL;
    tableCubes = NULL;
    numPointsEachCube = NULL;

    indexZOfYXCrossView = 0;
    indexYOfZXCrossView = 0;
    indexXOfZYCrossView = 0;
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

    for (int indexCube = 0; indexCube < numCubes; indexCube++) {
        if (whetherToDisplayACube(indexCube) == false) {
            continue;
        }
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

void DataDisplayerTableCubes::onKeyPressed(int key) {
    switch (key) {
    // j
    case 106:
        decreaseIndexOfCrossView();
        break;
    // k
    case 107:
        increaseIndexOfCrossView();
        break;
    default:
        break;
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
    //indexCubeStart = scene->XYZtoIndexOfCube(xMin + 0.01, yMin + 0.01, 0.01);
    //indexCubeEnd = indexCubeStart + numCubes[0] * numCubes[1];

    xNumCubes = scene->getXNumCubes();
    yNumCubes = scene->getYNumCubes();
    zNumCubes = scene->getZNumCubes();

    indexZOfYXCrossView = xNumCubes / 3;
    indexYOfZXCrossView = yNumCubes / 2;
    indexXOfZYCrossView = zNumCubes / 2;
}

void DataDisplayerTableCubes::onViewModeChanged(int viewModeIn) {
    viewMode = (ViewMode)viewModeIn;
}

/* ---------------------------------------
 * Private:
 * --------------------------------------- */

bool DataDisplayerTableCubes::whetherToDisplayACube(int indexCube) {
    int indexZ, indexY, indexX;
    switch (viewMode) {
    case TOPVIEW_YPOS_XPOS:
        indexZ = scene->indexOfCubeToZComponent(indexCube);
        if (indexZ == indexZOfYXCrossView)
            return true;
        else
            return false;
        break;
    case FRONTVIEW_ZPOS_XPOS:
        indexY = scene->indexOfCubeToYComponent(indexCube);
        if (indexY == indexYOfZXCrossView)
            return true;
        else
            return false;
        break;
        break;
    case RIGHTVIEW_ZPOS_YPOS:
        indexX = scene->indexOfCubeToXComponent(indexCube);
        if (indexX == indexXOfZYCrossView)
            return true;
        else
            return false;
        break;
        break;
    default:
        return false;
        break;
    }
}

void DataDisplayerTableCubes::increaseIndexOfCrossView() {
    switch (viewMode) {
    case TOPVIEW_YPOS_XPOS:
        if (indexZOfYXCrossView <= zNumCubes - 2)
            indexZOfYXCrossView++;
        break;
    case FRONTVIEW_ZPOS_XPOS:
        if (indexYOfZXCrossView <= yNumCubes - 2)
            indexYOfZXCrossView++;
        break;
    case RIGHTVIEW_ZPOS_YPOS:
        if (indexXOfZYCrossView <= xNumCubes - 2)
            indexXOfZYCrossView++;
        break;
    default:
        break;
    }
}

void DataDisplayerTableCubes::decreaseIndexOfCrossView() {
    switch (viewMode) {
    case TOPVIEW_YPOS_XPOS:
        if (indexZOfYXCrossView >= 1)
            indexZOfYXCrossView--;
        break;
    case FRONTVIEW_ZPOS_XPOS:
        if (indexYOfZXCrossView >= 1)
            indexYOfZXCrossView--;
        break;
    case RIGHTVIEW_ZPOS_YPOS:
        if (indexXOfZYCrossView >= 1)
            indexXOfZYCrossView--;
        break;
    default:
        break;
    }
}