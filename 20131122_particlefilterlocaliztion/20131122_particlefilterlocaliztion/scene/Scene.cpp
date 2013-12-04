#include "Scene.h"

Scene::Scene(void) {
    for (int i = 0; i < 6; i++) {
        dRangeOfScene[i] = 0;
    }
    distanceDefiningPointTouchesLine = 0.1;
}

Scene::~Scene(void) {
}

int Scene::initialize(char *fileScene, double lengthCubeEdge) {
    // Set.
    if (lengthCubeEdge <= 0) lengthCubeEdge = 1;
    this->lengthCubeEdge = lengthCubeEdge;

    // Create reader to read file.
    ReadPlyByRPly reader;
    reader.readVerticesXYZ(fileScene, &dPointsXYZ);
    numPoints = reader.getNumVertices();

    // Calculate range of scene.
    calcRangeOfScene();

    // Create table of cubes.
    createTableCubes();

    return 0;
}

double Scene::takeAShot(double xPoint, double yPoint, double zPoint, 
                        double xFront, double yFront, double zFront,
                        double xTop,   double yTop,   double zTop) {
    return distanceToNearestPointTouchingTheLine(xPoint, yPoint, zPoint, xFront, yFront, zFront);
}

double Scene::calcDistanceFromPointAlongLine(double xPoint, double yPoint, double zPoint, 
                                             double xFront, double yFront, double zFront) {
    return distanceToNearestPointTouchingTheLine(xPoint, yPoint, zPoint, xFront, yFront, zFront);
}

void Scene::setDistanceDefiningPointTouchesLine(double distance) {
    distanceDefiningPointTouchesLine = distance;
}

int Scene::printDPointsXYZ() {
    // If array is not created yet.
    if (!dPointsXYZ) return 1;

    // Print
    for (int i = 0; i < numPoints * 3; i += 3) {
        cout << dPointsXYZ[i] << '\t' << dPointsXYZ[i+1] << '\t' << dPointsXYZ[i+2] << endl;
    }

    return 0;
}

double *Scene::getRangeOfScene() {
    return dRangeOfScene;
    // How to forbid changing the array when passed outside?
}

double **Scene::getTableCubes() {
    return tableCubes;
    // Again, how to forbid changing the array when passed outside?
}

int *Scene::getNumPointsEachCube() {
    return numPointsEachCube;
    // Again, how to forbid changing the array when passed outside?
}

/* -------------------------
 * Private
 * ------------------------- */

void Scene::calcRangeOfScene() {
    // Calculate real range.
    for (int i = 0; i < numPoints * 3; i += 3) {
        // x
        if      (dPointsXYZ[i  ] > dRangeOfScene[0]) dRangeOfScene[0] = dPointsXYZ[i  ];
        else if (dPointsXYZ[i  ] < dRangeOfScene[1]) dRangeOfScene[1] = dPointsXYZ[i  ];
        // y
        if      (dPointsXYZ[i+1] > dRangeOfScene[2]) dRangeOfScene[2] = dPointsXYZ[i+1];
        else if (dPointsXYZ[i+1] < dRangeOfScene[3]) dRangeOfScene[3] = dPointsXYZ[i+1];
        // z
        if      (dPointsXYZ[i+2] > dRangeOfScene[4]) dRangeOfScene[4] = dPointsXYZ[i+2];
        else if (dPointsXYZ[i+2] < dRangeOfScene[5]) dRangeOfScene[5] = dPointsXYZ[i+2];
    }

    // Round the range.
    for (int i = 0; i < 6; i += 2) {
        // Calculate rounded max.
        if ((int)(dRangeOfScene[i  ] / lengthCubeEdge) >= 0)
            dRangeOfScene[i  ] = ((int)(dRangeOfScene[i  ] / lengthCubeEdge) + 1) * lengthCubeEdge;
        else
            dRangeOfScene[i  ] =  (int)(dRangeOfScene[i  ] / lengthCubeEdge     ) * lengthCubeEdge;

        // Calculate rounded min.
        if ((int)(dRangeOfScene[i+1] / lengthCubeEdge) >= 0)
            dRangeOfScene[i+1] =  (int)(dRangeOfScene[i+1] / lengthCubeEdge     ) * lengthCubeEdge;
        else
            dRangeOfScene[i+1] = ((int)(dRangeOfScene[i+1] / lengthCubeEdge) - 1) * lengthCubeEdge;
    }
}

void Scene::createTableCubes() {
    // Count number of cubes.
    xNumCubes = (int)( (dRangeOfScene[0] - dRangeOfScene[1]) / lengthCubeEdge );
    yNumCubes = (int)( (dRangeOfScene[2] - dRangeOfScene[3]) / lengthCubeEdge );
    zNumCubes = (int)( (dRangeOfScene[4] - dRangeOfScene[5]) / lengthCubeEdge );
    xyNumCubes = xNumCubes * yNumCubes;
    int numCubes = xNumCubes * yNumCubes * zNumCubes;

    // Create tableCubes and numPointsEachCube.
    tableCubes = new double *[numCubes];
    numPointsEachCube = new int[numCubes];
    for (int i = 0; i < numCubes; i++)
        numPointsEachCube[i] = 0;

    // Count number of points per cube.
    for (int i = 0; i < numPoints * 3; i += 3) {
        numPointsEachCube[ XYZtoIndexOfCube(dPointsXYZ[i], dPointsXYZ[i+1], dPointsXYZ[i+2]) ]++;
    }

    // Create entries of tableCubes.
    for (int i = 0; i < numCubes; i++) {
        // No points in cube, ignore.
        if (numPointsEachCube == 0) continue;
        // There are points in cube, create entry.
        tableCubes[i] = new double[numPointsEachCube[i] * 3];
    }

    // Fill in points into entries of tableCubes.
    int *currentIndexEachCube = new int[numCubes];
    for (int i = 0; i < numCubes; i++) {
        currentIndexEachCube[i] = 0;
    }
    for (int i = 0; i < numPoints * 3; i += 3) {
        int indexCube = XYZtoIndexOfCube(dPointsXYZ[i], dPointsXYZ[i+1], dPointsXYZ[i+2]);
        tableCubes[indexCube][currentIndexEachCube[indexCube] * 3    ] = dPointsXYZ[i    ];
        tableCubes[indexCube][currentIndexEachCube[indexCube] * 3 + 1] = dPointsXYZ[i + 1];
        tableCubes[indexCube][currentIndexEachCube[indexCube] * 3 + 2] = dPointsXYZ[i + 2];
        currentIndexEachCube[indexCube]++;
    }
}

/* ---------------------------------------------------
 * XYZtoIndexOfCube
 * Return index of cube given an (x, y, z) coordinate.
 * If out of range of scene, return -1.
 * ---------------------------------------------------  */

int Scene::XYZtoIndexOfCube(double x, double y, double z) {
    // Check out of range.
    if (x > dRangeOfScene[0] || x < dRangeOfScene[1] ||
        y > dRangeOfScene[2] || y < dRangeOfScene[3] ||
        z > dRangeOfScene[4] || z < dRangeOfScene[5] )
        return -1;

    // Calculate index.
    int xInd = (int)( (x - dRangeOfScene[1]) / lengthCubeEdge );
    int yInd = (int)( (y - dRangeOfScene[3]) / lengthCubeEdge );
    int zInd = (int)( (z - dRangeOfScene[5]) / lengthCubeEdge );
    int index = zInd * xyNumCubes + yInd * xNumCubes + xInd;
    return index;
}

/* ------------------------------------------------------------------------------
 * distanceToNearestPointTouchingTheLine
 * Calcutate distance to the nearest point which 'touch' the line given.
 * 'Touch' means prependicular distance from the line lies within a range set previously.
 *
 * What the algorithm looks like in 2D space:
 * 
 *  +------+------+------+------+------+
 *  |      |      | /    |      |      |
 *  |      |7     |6     |8     |      |
 *  +------+------+------+------+------+
 *  |      |     /|      |      |      |
 *  |    4 |    3 |    5 |      |      |
 *  +------+------+------+------+------+
 *  |      | a/   |      |      |      |
 *  | 1    | 0    | 2    |      |      |
 *  +------+------+------+------+------+
 *
 *  0: Starting point
 *  0 to 9: Cubes where these points sit will be checked in order.
 *  a: If line angle is larger than 45 degrees, point 1 and 2 would rotate
 *     to another locations 90 degrees from these,
 *     i.e., be vertical rather than horizontal in this example.
 * 
 * Param xStart, yStart, zStart: Coords of point in interest.
 * Param xLine, yLine, zLine: Direction vector of the line.
 * Return: Distance calculated.
 * --------------------------------------------------- ---------------------------*/
double Scene::distanceToNearestPointTouchingTheLine(double xStart, double yStart, double zStart, 
                                                    double xLine, double yLine, double zLine) {
    // Check whether starting point is out of range.
    if (XYZtoIndexOfCube(xStart, yStart, zStart) < 0)
        return -1;

    // Normalize line vector.
    double lengthLine = sqrt(xLine*xLine + yLine*yLine + zLine*zLine);
    xLine = xLine / lengthLine;
    yLine = yLine / lengthLine;
    zLine = zLine / lengthLine;

    // Determine 2 vectors used in locating surrounding 8 points.
    // 6 cases. In 2D a square is cut into 4 pieces by 45 degrees lines:
    //  +-------+
    //  |\     /|
    //  | \   / |
    //  |   X   |
    //  | /   \ |
    //  |/     \| (It is a square.)
    //  +-------+
    // And a 3D box is cut into 6 regions by 45 degrees planes. Thus 6 cases here.
    // Then set the length of line vector. There are 3 cases for 3 axes.
    double vector1ToSurroundPoint[3]; // x, y, z components.
    double vector2ToSurroundPoint[3];
    if      (xLine >  0 && xLine    >= abs(yLine) && xLine    >= abs(zLine)) {
        // Use  y and  z vector with length of lengthCubeEdge.
        vector1ToSurroundPoint[0] = 0;
        vector1ToSurroundPoint[1] = lengthCubeEdge;
        vector1ToSurroundPoint[2] = 0;
        vector2ToSurroundPoint[0] = 0;
        vector2ToSurroundPoint[1] = 0;
        vector2ToSurroundPoint[2] = lengthCubeEdge;

        // Set new lengthLine.
        lengthLine = lengthCubeEdge / xLine;
    }
    else if (xLine <  0 && xLine*-1 >= abs(yLine) && xLine*-1 >= abs(zLine)) {
        // Use -y and  z vector with length of lengthCubeEdge.
        vector1ToSurroundPoint[0] = 0;
        vector1ToSurroundPoint[1] = -1 * lengthCubeEdge;
        vector1ToSurroundPoint[2] = 0;
        vector2ToSurroundPoint[0] = 0;
        vector2ToSurroundPoint[1] = 0;
        vector2ToSurroundPoint[2] = lengthCubeEdge;

        // Set new lengthLine.
        lengthLine = lengthCubeEdge / xLine*-1;
    }
    else if (yLine >  0 && yLine    >= abs(xLine) && yLine    >= abs(zLine)) {
        // Use -x and  z vector with length of lengthCubeEdge.
        vector1ToSurroundPoint[0] = -1 * lengthCubeEdge;
        vector1ToSurroundPoint[1] = 0;
        vector1ToSurroundPoint[2] = 0;
        vector2ToSurroundPoint[0] = 0;
        vector2ToSurroundPoint[1] = 0;
        vector2ToSurroundPoint[2] = lengthCubeEdge;

        // Set new lengthLine.
        lengthLine = lengthCubeEdge / yLine;
    }
    else if (yLine <  0 && yLine*-1 >= abs(xLine) && yLine*-1 >= abs(zLine)) {
        // Use  x and  z vector with length of lengthCubeEdge.
        vector1ToSurroundPoint[0] = lengthCubeEdge;
        vector1ToSurroundPoint[1] = 0;
        vector1ToSurroundPoint[2] = 0;
        vector2ToSurroundPoint[0] = 0;
        vector2ToSurroundPoint[1] = 0;
        vector2ToSurroundPoint[2] = lengthCubeEdge;

        // Set new lengthLine.
        lengthLine = lengthCubeEdge / yLine*-1;
    }
    else if (xLine >  0 && xLine    >= abs(xLine) && xLine    >= abs(yLine)) {
        // Use  y and -x vector with length of lengthCubeEdge.
        vector1ToSurroundPoint[0] = 0;
        vector1ToSurroundPoint[1] = lengthCubeEdge;
        vector1ToSurroundPoint[2] = 0;
        vector2ToSurroundPoint[0] = -1 * lengthCubeEdge;
        vector2ToSurroundPoint[1] = 0;
        vector2ToSurroundPoint[2] = 0;

        // Set new lengthLine.
        lengthLine = lengthCubeEdge / zLine;
    }
    else if (xLine <  0 && xLine*-1 >= abs(xLine) && xLine*-1 >= abs(yLine)) {
        // Use  y and  x vector with length of lengthCubeEdge.
        vector1ToSurroundPoint[0] = 0;
        vector1ToSurroundPoint[1] = lengthCubeEdge;
        vector1ToSurroundPoint[2] = 0;
        vector2ToSurroundPoint[0] = lengthCubeEdge;
        vector2ToSurroundPoint[1] = 0;
        vector2ToSurroundPoint[2] = 0;

        // Set new lengthLine.
        lengthLine = lengthCubeEdge / zLine*-1;
    }

    // Set line vector to new length.
    xLine = xLine * lengthLine;
    yLine = yLine * lengthLine;
    zLine = zLine * lengthLine;

    // Create an array to store the 9 points, including the starting point.
    // Order:
    //     ^
    //  4  3  2
    //  5  0  1 >
    //  6  7  8
    //  0: The starting point.
    //  >: vector1ToSurroundPoint
    //  ^: vector2ToSurroundPoint
    double pointsOfNine[27];
    // point 0
    pointsOfNine[ 0] = xStart;
    pointsOfNine[ 1] = yStart;
    pointsOfNine[ 2] = zStart;
    // point 1
    pointsOfNine[ 3] = xStart + vector1ToSurroundPoint[0];
    pointsOfNine[ 4] = yStart + vector1ToSurroundPoint[1];
    pointsOfNine[ 5] = zStart + vector1ToSurroundPoint[2];
    // point 2
    pointsOfNine[ 6] = xStart + vector1ToSurroundPoint[0] + vector2ToSurroundPoint[0];
    pointsOfNine[ 7] = yStart + vector1ToSurroundPoint[1] + vector2ToSurroundPoint[1];
    pointsOfNine[ 8] = zStart + vector1ToSurroundPoint[2] + vector2ToSurroundPoint[2];
    // point 3
    pointsOfNine[ 9] = xStart                             + vector2ToSurroundPoint[0];
    pointsOfNine[10] = yStart                             + vector2ToSurroundPoint[1];
    pointsOfNine[11] = zStart                             + vector2ToSurroundPoint[2];
    // point 4
    pointsOfNine[12] = xStart - vector1ToSurroundPoint[0] + vector2ToSurroundPoint[0];
    pointsOfNine[13] = yStart - vector1ToSurroundPoint[1] + vector2ToSurroundPoint[1];
    pointsOfNine[14] = zStart - vector1ToSurroundPoint[2] + vector2ToSurroundPoint[2];
    // point 5
    pointsOfNine[15] = xStart - vector1ToSurroundPoint[0];
    pointsOfNine[16] = yStart - vector1ToSurroundPoint[1];
    pointsOfNine[17] = zStart - vector1ToSurroundPoint[2];
    // point 6
    pointsOfNine[18] = xStart - vector1ToSurroundPoint[0] - vector2ToSurroundPoint[0];
    pointsOfNine[19] = yStart - vector1ToSurroundPoint[1] - vector2ToSurroundPoint[1];
    pointsOfNine[20] = zStart - vector1ToSurroundPoint[2] - vector2ToSurroundPoint[2];
    // point 7
    pointsOfNine[21] = xStart                             - vector2ToSurroundPoint[0];
    pointsOfNine[22] = yStart                             - vector2ToSurroundPoint[1];
    pointsOfNine[23] = zStart                             - vector2ToSurroundPoint[2];
    // point 8
    pointsOfNine[24] = xStart + vector1ToSurroundPoint[0] - vector2ToSurroundPoint[0];
    pointsOfNine[25] = yStart + vector1ToSurroundPoint[1] - vector2ToSurroundPoint[1];
    pointsOfNine[26] = zStart + vector1ToSurroundPoint[2] - vector2ToSurroundPoint[2];

    // Do line search along the line until out of range.
    int numOfTimeWhileLoop = 0; // For output if no distance found.
    double distanceShortest = -1;
    int indicesNineCubes[9];
    bool allCubesOutOfRange = false;
    while (!allCubesOutOfRange) {
        // Get the 9 cubes' indices
        // and
        // calculate touching line shortest distances to each cube if exist.
        int countCubesOutOfRange = 0;
        for (int i = 0; i < 9; i++) {
            // Get index.
            indicesNineCubes[i] = XYZtoIndexOfCube(pointsOfNine[i * 3],
                                                   pointsOfNine[i * 3 + 1],
                                                   pointsOfNine[i * 3 + 2]);
            // Calculate distance.
            if (indicesNineCubes[i] >= 0) {
                double distance = distanceToNearestPointTouchingTheLineInACube(indicesNineCubes[i],
                                                                               xStart, yStart, zStart,
                                                                               xLine, yLine, zLine,
                                                                               lengthLine);
                // IF DISTANCE FOUND, BREAK THE LOOP.
                if (distance >= 0) {
                    distanceShortest = distance;
                    break;
                }
            }
            // Count out of range.
            else
                countCubesOutOfRange++;
        }

        // IF DISTANCE FOUND, BREAK THE LOOP.
        // It came from the break of inner loop.
        if (distanceShortest >= 0)
            break;

        // If all 'cubes' out of range, the break condition is satisfied.
        if (countCubesOutOfRange == 9)
            allCubesOutOfRange = true;

        // Move the 9 points to new position.
        else {
            for (int i = 0; i < 9; i++) {
                pointsOfNine[i * 3    ] += xLine;
                pointsOfNine[i * 3 + 1] += yLine;
                pointsOfNine[i * 3 + 2] += zLine;
            }
        }

        // Increment numOfTimeWhileLoop.
        numOfTimeWhileLoop++;
    }

    // If distance found, return it.
    if (distanceShortest >= 0)
        return distanceShortest;
    // If not found, return the distance to the last center point.
    else
        return lengthLine * (numOfTimeWhileLoop - 1);
}

/* ---------------------------------------------
 * distanceToNearestPointTouchingTheLineInACube
 * Return distance.
 * If no points touch the line, return -1.
 * ---------------------------------------------  */
double Scene::distanceToNearestPointTouchingTheLineInACube(int indexCube,
                                                           double xStart, double yStart, double zStart,
                                                           double xLine, double yLine, double zLine,
                                                           double lengthLine) {
    // Search every points in the cube.
    // If a point's distance to the line is closer than the distanceDefiningPointTouchesLine,
    // return the distance from thus stop searching.
    double distanceToReturn = -1;
    double distanceTemp;
    double xStartToPointi;
    double yStartToPointi;
    double zStartToPointi;
    double lengthStartToPointi;
    double cosTheta;
    double sinTheta;
    for (int i = 0; i < numPointsEachCube[indexCube]; i++) {
        // Calculate the vector from starting point to point i.
        xStartToPointi = tableCubes[indexCube][i * 3    ] - xStart;
        yStartToPointi = tableCubes[indexCube][i * 3 + 1] - yStart;
        zStartToPointi = tableCubes[indexCube][i * 3 + 2] - zStart;

        // If point i is at the opposite direction, ignore it.
        // ALTERNATIVE: Can be implemented at distanceToNearestPointTouchingTheLine
        //                by other ways, such as check only at starting cube.
        //                It may increase speed (really?).
        cosTheta = xLine * xStartToPointi +
                   yLine * yStartToPointi +
                   zLine * zStartToPointi;  // Currently it is a innor product.
        if (cosTheta < 0) {
            continue;
        }

        // If point i is close enough, it touches, thus get what we want.
        lengthStartToPointi = sqrt(xStartToPointi * xStartToPointi +
                                   yStartToPointi * yStartToPointi +
                                   zStartToPointi * zStartToPointi);
        if (lengthStartToPointi <= distanceDefiningPointTouchesLine) {
            distanceToReturn = lengthStartToPointi;
            break;
        }

        // Calculate distanceToReturn = lengthLine * sinTheta.
        cosTheta /= lengthStartToPointi * lengthLine;
        sinTheta = sqrt(1 - cosTheta * cosTheta);
        distanceTemp = lengthLine * sinTheta;

        // If distanceToReturn is small enough, it's what we want.
        if (distanceTemp <= distanceDefiningPointTouchesLine) {
            distanceToReturn = lengthStartToPointi;
            break;
        }
    }

    return distanceToReturn;
}
