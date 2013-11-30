#include "Scene.h"

Scene::Scene(void) {
	for (int i = 0; i < 6; i++) {
		dRangeOfScene[i] = 0;
	}
	distancePointTouchesLine = 0.1;
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

void Scene::setDistancePointTouchesLine(double distance) {
	distancePointTouchesLine = distance;
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

/* -------------------------
 * Private
 * ------------------------- */

void Scene::calcRangeOfScene() {
	// Calculate real range.
	for (int i = 0; i < numPoints * 3; i += 3) {
		// x
		if (dPointsXYZ[i] > dRangeOfScene[0]) dRangeOfScene[0] = dPointsXYZ[i];
		else if (dPointsXYZ[i] < dRangeOfScene[1]) dRangeOfScene[1] = dPointsXYZ[i];
		// y
		if (dPointsXYZ[i+1] > dRangeOfScene[2]) dRangeOfScene[2] = dPointsXYZ[i+1];
		else if (dPointsXYZ[i+1] < dRangeOfScene[3]) dRangeOfScene[3] = dPointsXYZ[i+1];
		// z
		if (dPointsXYZ[i+2] > dRangeOfScene[4]) dRangeOfScene[4] = dPointsXYZ[i+2];
		else if (dPointsXYZ[i+2] < dRangeOfScene[5]) dRangeOfScene[5] = dPointsXYZ[i+2];
	}

	// Round the range.
	for (int i = 0; i < 6; i += 2) {
		cout << dRangeOfScene[i] << " ";
		// Calculate rounded max.
		if ((int)(dRangeOfScene[i] / lengthCubeEdge) >= 0)
			dRangeOfScene[i] = ((int)(dRangeOfScene[i] / lengthCubeEdge) + 1) * lengthCubeEdge;
		else
			dRangeOfScene[i] = (int)(dRangeOfScene[i] / lengthCubeEdge) * lengthCubeEdge;

		cout << dRangeOfScene[i+1] << endl;
		// Calculate rounded min.
		if ((int)(dRangeOfScene[i+1] / lengthCubeEdge) >= 0)
			dRangeOfScene[i+1] = (int)(dRangeOfScene[i+1] / lengthCubeEdge) * lengthCubeEdge;
		else
			dRangeOfScene[i+1] = ((int)(dRangeOfScene[i+1] / lengthCubeEdge) - 1) * lengthCubeEdge;
		cout <<  dRangeOfScene[i] << " " << dRangeOfScene[i+1] << endl;
	}
}

void Scene::createTableCubes() {
	// Count number of cubes.
	xNumCubes = (int)( (dRangeOfScene[0] - dRangeOfScene[1]) / lengthCubeEdge );
	yNumCubes = (int)( (dRangeOfScene[2] - dRangeOfScene[3]) / lengthCubeEdge );
	zNumCubes = (int)( (dRangeOfScene[4] - dRangeOfScene[5]) / lengthCubeEdge );
	xyNumCubes = xNumCubes * yNumCubes;
	int numCubes = xNumCubes * yNumCubes * zNumCubes;

	// Create tableCubes and numPointsPerCube.
	tableCubes = new double *[numCubes];
	numPointsPerCube = new int[numCubes];
	for (int i = 0; i < numCubes; i++)
		numPointsPerCube[i] = 0;

	// Count number of points per cube.
	for (int i = 0; i < numPoints * 3; i += 3) {
		numPointsPerCube[ XYZtoIndexOfCube(dPointsXYZ[i], dPointsXYZ[i+1], dPointsXYZ[i+2]) ]++;
	}

	// Create entries of tableCubes.
	for (int i = 0; i < numCubes; i++) {
		cout << numPointsPerCube[i] << endl;
		// No points in cube, ignore.
		if (numPointsPerCube == 0) continue;
		// There are points in cube, create entry.
		tableCubes[i] = new double[numPointsPerCube[i] * 3];
	}

	// Fill in points into entries of tableCubes.
	int *currentIndexEachCube = new int[numCubes];
	for (int i = 0; i < numCubes; i++) {
		currentIndexEachCube[i] = 0;
	}
	for (int i = 0; i < numPoints * 3; i += 3) {
		int indexCube = XYZtoIndexOfCube(dPointsXYZ[i], dPointsXYZ[i+1], dPointsXYZ[i+2]);
		tableCubes[indexCube][currentIndexEachCube[indexCube] * 3    ] = dPointsXYZ[0];
		tableCubes[indexCube][currentIndexEachCube[indexCube] * 3 + 1] = dPointsXYZ[1];
		tableCubes[indexCube][currentIndexEachCube[indexCube] * 3 + 2] = dPointsXYZ[2];
		currentIndexEachCube[indexCube]++;
	}
}

/* ------------------------
 * XYZtoIndexOfCube
 * Return index of cube given an (x, y, z) coordinate.
 * If out of range of scene, return -1.
 * ------------------------ */

int Scene::XYZtoIndexOfCube(double x, double y, double z) {
	// Check out of range.
	if (x > dRangeOfScene[0] || x < dRangeOfScene[1] ||
		y > dRangeOfScene[2] || y < dRangeOfScene[3] ||
		z > dRangeOfScene[4] || z < dRangeOfScene[5] )
		return -1;

	// Calculate index.
	int xInd = (x - dRangeOfScene[1]) / lengthCubeEdge;
	int yInd = (y - dRangeOfScene[3]) / lengthCubeEdge;
	int zInd = (z - dRangeOfScene[5]) / lengthCubeEdge;
	int index = zInd * xyNumCubes + yInd * xNumCubes + xInd;
	return index;
}

/* ------------------------
 * distanceToNearestPointTouchingTheLine
 * Calcutate distance to the nearest point which 'touch' the line given.
 * 'Touch' means prependicular distance from the line lies within a range set previously.
 *
 * What the algorithm looks like in 2D space:
 * 
 *  ¢q¢w¢w¢w¢w¢w¢w¢q¢w¢w¢w¢w¢w¢w¢q¢w¢w¢w¢w¢w¢w¢q¢w¢w¢w¢w¢w¢w¢q¢w¢w¢w¢w¢w¢w¢q
 *  ¢x      ¢x      ¢x /    ¢x      ¢x      ¢x
 *  ¢x      ¢x7     ¢x6     ¢x8     ¢x      ¢x
 *  ¢q¢w¢w¢w¢w¢w¢w¢q¢w¢w¢w¢w¢w¢w¢q¢w¢w¢w¢w¢w¢w¢q¢w¢w¢w¢w¢w¢w¢q¢w¢w¢w¢w¢w¢w¢q
 *  ¢x      ¢x     /¢x      ¢x      ¢x      ¢x
 *  ¢x    4 ¢x    3 ¢x    5 ¢x      ¢x      ¢x
 *  ¢q¢w¢w¢w¢w¢w¢w¢q¢w¢w¢w¢w¢w¢w¢q¢w¢w¢w¢w¢w¢w¢q¢w¢w¢w¢w¢w¢w¢q¢w¢w¢w¢w¢w¢w¢q
 *  ¢x      ¢x £c/   ¢x      ¢x      ¢x      ¢x
 *  ¢x 1    ¢x 0    ¢x 2    ¢x      ¢x      ¢x
 *  ¢q¢w¢w¢w¢w¢w¢w¢q¢w¢w¢w¢w¢w¢w¢q¢w¢w¢w¢w¢w¢w¢q¢w¢w¢w¢w¢w¢w¢q¢w¢w¢w¢w¢w¢w¢q
 *
 *  0: Starting point
 *  0 to 9: Cubes where these points sit will be checked in order.
 *  £c: If line angle is larger than 45 degrees, point 1 and 2 would rotate
 *     to another locations 90 degrees from these,
 *     i.e., be vertical rather than horizontal in this example.
 * 
 * Param xStart, yStart, zStart: Coords of point in interest.
 * Param xLine, yLine, zLine: Direction vector of the line.
 * Return: Distance calculated.
 * ------------------------ */
double Scene::distanceToNearestPointTouchingTheLine(double xStart, double yStart, double zStart, 
													double xLine, double yLine, double zLine) {
	// Normalize line vector.
	double lengthLine = sqrt(xLine*xLine + yLine*yLine + zLine*zLine);
	xLine = xLine / lengthLine;
	yLine = yLine / lengthLine;
	zLine = zLine / lengthLine;

	// Determine 2 vectors used in locating surrounding 8 points.
	// 6 cases. In 2D a square is cut into 4 pieces by 45 degrees lines:
	//  ¢z¢w¢w¢w¢w¢w¢w¢w¢{
	//  ¢x\     /¢x
	//  ¢x \   / ¢x
	//  ¢x   X   ¢x
	//  ¢x /   \ ¢x
	//  ¢x/     \¢x (It is a square.)
	//  ¢|¢w¢w¢w¢w¢w¢w¢w¢}
	// And a 3D box is cut into 6 regions by 45 degrees planes. Thus 6 cases here.
	double vector1ToSurroundPoint[3]; // x, y, z components.
	double vector2ToSurroundPoint[3];
	if      (xLine >= 0 && xLine    >= abs(yLine) && xLine    >= abs(zLine)) {
		// Use  y and  z vector with length of lengthCubeEdge.
		vector1ToSurroundPoint[0] = 0;
		vector1ToSurroundPoint[1] = lengthCubeEdge;
		vector1ToSurroundPoint[2] = 0;
		vector2ToSurroundPoint[0] = 0;
		vector2ToSurroundPoint[1] = 0;
		vector2ToSurroundPoint[2] = lengthCubeEdge;
	}
	else if (xLine <  0 && xLine*-1 >= abs(yLine) && xLine*-1 >= abs(zLine)) {
		// Use -y and  z vector with length of lengthCubeEdge.
		vector1ToSurroundPoint[0] = 0;
		vector1ToSurroundPoint[1] = -1 * lengthCubeEdge;
		vector1ToSurroundPoint[2] = 0;
		vector2ToSurroundPoint[0] = 0;
		vector2ToSurroundPoint[1] = 0;
		vector2ToSurroundPoint[2] = lengthCubeEdge;
	}
	else if (yLine >= 0 && yLine    >= abs(xLine) && yLine    >= abs(zLine)) {
		// Use -x and  z vector with length of lengthCubeEdge.
		vector1ToSurroundPoint[0] = -1 * lengthCubeEdge;
		vector1ToSurroundPoint[1] = 0;
		vector1ToSurroundPoint[2] = 0;
		vector2ToSurroundPoint[0] = 0;
		vector2ToSurroundPoint[1] = 0;
		vector2ToSurroundPoint[2] = lengthCubeEdge;
	}
	else if (yLine <  0 && yLine*-1 >= abs(xLine) && yLine*-1 >= abs(zLine)) {
		// Use  x and  z vector with length of lengthCubeEdge.
		vector1ToSurroundPoint[0] = lengthCubeEdge;
		vector1ToSurroundPoint[1] = 0;
		vector1ToSurroundPoint[2] = 0;
		vector2ToSurroundPoint[0] = 0;
		vector2ToSurroundPoint[1] = 0;
		vector2ToSurroundPoint[2] = lengthCubeEdge;
	}
	else if (xLine >= 0 && xLine    >= abs(xLine) && xLine    >= abs(yLine)) {
		// Use  y and -x vector with length of lengthCubeEdge.
		vector1ToSurroundPoint[0] = 0;
		vector1ToSurroundPoint[1] = lengthCubeEdge;
		vector1ToSurroundPoint[2] = 0;
		vector2ToSurroundPoint[0] = -1 * lengthCubeEdge;
		vector2ToSurroundPoint[1] = 0;
		vector2ToSurroundPoint[2] = 0;
	}
	else if (xLine <  0 && xLine*-1 >= abs(xLine) && xLine*-1 >= abs(yLine)) {
		// Use  y and  x vector with length of lengthCubeEdge.
		vector1ToSurroundPoint[0] = 0;
		vector1ToSurroundPoint[1] = lengthCubeEdge;
		vector1ToSurroundPoint[2] = 0;
		vector2ToSurroundPoint[0] = lengthCubeEdge;
		vector2ToSurroundPoint[1] = 0;
		vector2ToSurroundPoint[2] = 0;
	}

	// 
}

double Scene::distanceToNearestPointTouchingTheLineInACube(int indexCube,
														   double xLine, double yLine, double zLine) {

}