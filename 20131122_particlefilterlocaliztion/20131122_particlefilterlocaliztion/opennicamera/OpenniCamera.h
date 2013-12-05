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
    int* depthPhoto;
    int *depthPhotoSampled;
    double *samplingVectorsCameraCoordinates;
    double *samplingVectorsRealWorldCoordinates;
};

#endif