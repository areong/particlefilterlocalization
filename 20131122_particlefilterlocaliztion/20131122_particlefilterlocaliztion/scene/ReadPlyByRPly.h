#ifndef SCENE_READPLYBYRPLY_H
#define SCENE_READPLYBYRPLY_H

#include "../rply/rply.h"

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
