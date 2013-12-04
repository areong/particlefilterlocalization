#ifndef SCENE_H
#define SCENE_H

#include <cmath>
#include <iostream>
#include "ReadPlyByRPly.h"

class Scene {
public:
    Scene(void);
    ~Scene(void);
    int initialize(char *fileScene, double lengthCubeEdge);
    double takeAShot(double xPoint, double yPoint, double zPoint, 
                     double xFront, double yFront, double zFront,
                     double xTop,   double yTop,   double zTop);
    double calcDistanceFromPointAlongLine(double xStart, double yStart, double zStart, 
                                          double xLine, double yLine, double zLine);
    void setDistanceDefiningPointTouchesLine(double distance);
    int printDPointsXYZ();
    double *getRangeOfScene();
    double **getTableCubes();

private:
    double *dPointsXYZ;
    int numPoints;
    double dRangeOfScene[6]; // [xMax, xMin, yMax, yMin, zMax, zMin]
    double **tableCubes;
    double lengthCubeEdge;
    int xNumCubes;
    int yNumCubes;
    int zNumCubes;
    int xyNumCubes;
    int *numPointsPerCube;
    double distanceDefiningPointTouchesLine;

    void calcRangeOfScene();
    void createTableCubes();
    int XYZtoIndexOfCube(double x, double y, double z);
    double distanceToNearestPointTouchingTheLine(double xStart, double yStart, double zStart, 
                                                 double xLine, double yLine, double zLine);
    double distanceToNearestPointTouchingTheLineInACube(int indexCube,
                                                        double xStart, double yStart, double zStart,
                                                        double xLine, double yLine, double zLine,
                                                        double lengthLine);
};

#endif
