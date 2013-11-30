#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include "ReadPlyByRPly.h"

class Scene {
public:
	Scene(void);
	~Scene(void);
	int initialize(char *fileScene, double lengthCubeEdge);
	void setDistancePointTouchesLine(double distance);
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
	double distancePointTouchesLine;

	void calcRangeOfScene();
	void createTableCubes();
	int XYZtoIndexOfCube(double x, double y, double z);
	double distanceToNearestPointTouchingTheLine(double xStart, double yStart, double zStart, 
												 double xLine, double yLine, double zLine);
};

#endif