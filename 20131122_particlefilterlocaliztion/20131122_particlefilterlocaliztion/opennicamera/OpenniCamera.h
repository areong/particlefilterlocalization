#ifndef OPENNICAMERA_H
#define OPENNICAMERA_H

#include <iostream>
#include "OpenNI.h"
using namespace std;
using namespace openni;

class OpenniCamera {
public:
    OpenniCamera(void);
    ~OpenniCamera(void);
    int initialize();
    void setSamplingMethod();
    void takeNewDepthPhoto(double xFront, double yFront, double zFront, 
                           double xTop, double yTop, double zTop);
    int *getDepthPhoto();
    int *getDepthPhotoSampled();
    double *getSamplingVectorsCameraCoordinates();
    double *getSamplingVectorsRealWorldCoordinates();

private:
    Device device;
    VideoStream videoStreamDepth;
    VideoFrameRef videoFrameRefDepth;
    int widthFrame;
    int heightFrame;
    DepthPixel *depthPhoto;
    int *depthPhotoSampled;
    double *samplingVectorsCameraCoordinates;
    double *samplingVectorsRealWorldCoordinates;

    int *convertDepthPixelArrayToIntArray(const DepthPixel* depthPixelArray, int length);
};

typedef enum
{
	SAMPLING_GRID = 1,
	SAMPLING_GRID_RANDOM = 2,
	SAMPLING_RANDOM = 3,

} SamplingMethod;

#endif