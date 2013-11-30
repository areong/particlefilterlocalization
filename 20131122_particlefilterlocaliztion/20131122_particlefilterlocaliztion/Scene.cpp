#include "Scene.h"

Scene::Scene(void) {
	for (int i = 0; i < 6; i++) {
		dRangeOfScene[i] = 0;
	}
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

int Scene::printDPointsXYZ() {
	if (!dPointsXYZ) return 1;

	for (int i = 0; i < numPoints * 3; i += 3) {
		cout << dPointsXYZ[i] << '\t' << dPointsXYZ[i+1] << '\t' << dPointsXYZ[i+2] << endl;
	}

	return 0;
}

double **Scene::getTableCubes() {
	return tableCubes;
}

/*
 * Private
 */

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

int Scene::XYZtoIndexOfCube(double x, double y, double z) {
	int xInd = (x - dRangeOfScene[1]) / lengthCubeEdge;
	int yInd = (y - dRangeOfScene[3]) / lengthCubeEdge;
	int zInd = (z - dRangeOfScene[5]) / lengthCubeEdge;
	int index = zInd * xyNumCubes + yInd * xNumCubes + xInd;
	return index;
}