#include "OpenniCamera.h"
#include <iostream>
using namespace std;

OpenniCamera::OpenniCamera(void) {
}

OpenniCamera::~OpenniCamera(void) {
}

int OpenniCamera::initialize() {
    // Initialize OpenNI2.
    Status status = OpenNI::initialize();

    // Use any available device.
    device.open(ANY_DEVICE);

    // Create a depth video stream from device.
    videoStreamDepth.create(device, SENSOR_DEPTH);

    // Start streaming.
    videoStreamDepth.start();

    // Take a first shot to get width and height data.
    videoStreamDepth.readFrame(&videoFrameRefDepth);

    // Get resolutions AFTER readFrame(). (BEFORE causes wrong.)
    widthFrame = videoFrameRefDepth.getWidth();
    heightFrame = videoFrameRefDepth.getHeight();

    return 0;
}

void OpenniCamera::shutdown() {
    videoStreamDepth.destroy();
    device.close();
    OpenNI::shutdown();
}

/* ------------------------------------------------
 * setSamplingMethod
 * Parameters:
 *  1. Use SamplingMethod to determine the method.
 *  2-3. Parameters to be sent to actual sampling methods.
 *      a. SAMPING_GRID
 *          arga: Number of columns of grid points.
 *          argb: Number of rows of grid points.
 *      b. SAMPING_GRID_RANDOM
 *          arga: Number of columns of grid points.
 *          argb: Number of rows of grid points.
 *      C. SAMPING_RANDOM
 *          arga: Number of points.
 *          argb: Not used.
 * ------------------------------------------------ */
void OpenniCamera::setSamplingMethod(SamplingMethod method, int arga, int argb) {
    samplingMethod = method;
    samplingArgA = arga;
    samplingArgB = argb;

    // Do what should be done before taking photos.
    switch (samplingMethod) {
    case SAMPLING_GRID:
        calcSamplingIndicesByGrid();
        break;
    case SAMPLING_GRID_RANDOM:
        break;
    case SAMPLING_RANDOM:
        break;
    default:
        break;
    }
}

void OpenniCamera::takeNewDepthPhoto(double xFront, double yFront, double zFront, 
                                     double xTop, double yTop, double zTop) {
    // Get a frame referenced by mVideoFrameRef.
    videoStreamDepth.readFrame(&videoFrameRefDepth);

    // Get the data array of the frame.
    depthPhoto = (DepthPixel *)videoFrameRefDepth.getData();

    cout << depthPhoto[15000] << endl;

    // Sample the photo and calculate sampling vectors.
    switch (samplingMethod) {
    case SAMPLING_GRID:
        break;
    case SAMPLING_GRID_RANDOM:
        break;
    case SAMPLING_RANDOM:
        break;
    default:
        break;
    }
}

int *OpenniCamera::getDepthPhoto() {
    return convertDepthPixelArrayToIntArray(depthPhoto, widthFrame * heightFrame);
}

/* --------------------------------
 * Private
 * -------------------------------- */

int *OpenniCamera::convertDepthPixelArrayToIntArray(const DepthPixel* depthPixelArray, int length) {
    int *intArray;
    if (length <= 0)
        intArray = NULL;
    else {
        intArray = new int[length];
        for (int i = 0; i < length; i++)
            intArray[i] = depthPixelArray[i];
    }
    return intArray;
}


/* ------------------------------------------------
 * calcSamplingIndicesByGrid
 * Sample points sit at the center of each rectangular.
 * ------------------------------------------------ */
void OpenniCamera::calcSamplingIndicesByGrid() {
    // Index of target array.
    int indexSample = 0;

    // Half width/height of grid.
    double halfWidthGrid  = ((double)widthFrame  / (double)samplingArgA) / 2.0;
    double halfHeightGrid = ((double)heightFrame / (double)samplingArgB) / 2.0;

    // Create int array samplingIndicesOfPhoto.
    samplingIndicesOfPhoto = new int[samplingArgA * samplingArgB];

    // Fill in the array.
    for (int ih = 0; ih < samplingArgB; ih++)
        for (int iw = 0; iw < samplingArgA; iw++) {
            int x = (1 + 2 * iw) * (int)halfWidthGrid;
            int y = (1 + 2 * ih) * (int)halfHeightGrid;
            int indexPhotoArray = convertIntXYToIndexOfPhotoArray(x, y);
            samplingIndicesOfPhoto[ indexSample ] = indexPhotoArray;
            cout << indexSample << '\t' << samplingIndicesOfPhoto[ indexSample ] << endl;
            indexSample++;
        }
}

/* ------------------------------------------------
 * calcSamplingVectorsByGrid
 * calcSamplingIndicesByGrid() should have been called,
 * because this method uses the array samplingIndicesOfPhoto.
 * ------------------------------------------------ */
void OpenniCamera::calcSamplingVectorsByGrid(double xFront, double yFront, double zFront, 
                                             double xTop, double yTop, double zTop) {

}

int OpenniCamera::convertIntXYToIndexOfPhotoArray(int x, int y) {
    return y * widthFrame + x;
}