#ifndef OPENNICAMERA_OPENNICAMERA_H
#define OPENNICAMERA_OPENNICAMERA_H

#include "OpenNI.h"
using namespace openni;

typedef enum {
	SAMPLING_GRID = 1,
	SAMPLING_GRID_RANDOM = 2,
	SAMPLING_RANDOM = 3,
} SamplingMethod;

class OpenniCamera {
public:
    OpenniCamera(void);
    ~OpenniCamera(void);
    int initialize();
    void shutdown();
    void setSamplingMethod(SamplingMethod method, int arga, int argb);
    void takeNewDepthPhoto();
    int *getDepthPhoto();
    double *getDepthPhotoSampled();
    double *getSamplingVectors();

private:
    Device device;
    VideoStream videoStreamDepth;
    VideoFrameRef videoFrameRefDepth;
    int widthFrame;
    int heightFrame;
    DepthPixel *depthPhoto;
    double *depthPhotoSampled; // Unit is meter.

    int samplingMethod;
    int samplingArgA;
    int samplingArgB;
    int numSamplePoints;
    int *samplingIndicesOfPhoto;
    int *samplingPointsXYOnPhoto;
    double *samplingVectors;

    int *convertDepthPixelArrayToIntArray(const DepthPixel* depthPixelArray, int length);
    void calcSamplingIndicesByGrid();
    void calcSamplingVectors();
    int convertIntXYToIndexOfPhotoArray(int x, int y);
    void calcSamplingIndicesByRandom();
};

#endif