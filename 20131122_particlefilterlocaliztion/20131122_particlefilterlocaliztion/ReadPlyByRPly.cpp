#include "ReadPlyByRPly.h"

ReadPlyByRPly::ReadPlyByRPly(void) {
	indexVertex = 0;
}

ReadPlyByRPly::~ReadPlyByRPly(void) {
}

int ReadPlyByRPly::readVerticesXYZ(char *file, double **dPtr) {
	// Set filePly
	filePly = file;

	// Open.
    p_ply ply = ply_open(filePly, NULL, 0, NULL);
    if (!ply) return 1;
    if (!ply_read_header(ply)) return 1;

	// Set callbacks and get numVertices.
	this->numVertices = ply_set_read_cb(ply, "vertex", "x", callbackVertixSaveXYZIntoArray, this, 0);
    ply_set_read_cb(ply, "vertex", "y", callbackVertixSaveXYZIntoArray, this, 1);
    ply_set_read_cb(ply, "vertex", "z", callbackVertixSaveXYZIntoArray, this, 2);

	// Create an array to store vetices xyz. Set dVerticesXYZ.
    *dPtr = new double[numVertices * 3];
	dVerticesXYZ = *dPtr;
    
	// Read and calling callbacks.
    if (!ply_read(ply)) return 1;

	// Close.
    ply_close(ply);
    return 0;
}

int ReadPlyByRPly::getNumVertices() {
	return numVertices;
}

/*
 * Private
 */

int ReadPlyByRPly::callbackVertixSaveXYZIntoArray(p_ply_argument argument) {
	long axis;
	void *readerTemp;
	ply_get_argument_user_data(argument, &readerTemp, &axis);
	ReadPlyByRPly *reader = (ReadPlyByRPly *)readerTemp;
	double value = ply_get_argument_value(argument);
	switch (axis) {
	case 0:	// x
		reader->dVerticesXYZ[reader->indexVertex] = value;
		reader->indexVertex++;
		break;
	case 1:	// y
		reader->dVerticesXYZ[reader->indexVertex] = value;
		reader->indexVertex++;
		break;
	case 2:	// z
		reader->dVerticesXYZ[reader->indexVertex] = value;
		reader->indexVertex++;
		break;
	default:
		break;
	}
	return 1;
}