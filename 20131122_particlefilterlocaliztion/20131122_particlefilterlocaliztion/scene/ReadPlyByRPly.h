#ifndef READPLYBYRPLY_H
#define READPLYBYRPLY_H

#include <iostream>
#include "../rply/rply.h"
using namespace std;

class ReadPlyByRPly {
public:
    ReadPlyByRPly(void);
    ~ReadPlyByRPly(void);
    int readVerticesXYZ(char *file, double **dPtr);
    int getNumVertices();

    char *filePly;
    int indexVertex; // Used in callback.
    double *dVerticesXYZ;
private:
    static int callbackVertixSaveXYZIntoArray(p_ply_argument argument);
    int numVertices;
};

#endif
