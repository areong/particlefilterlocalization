#include "OpenniCamera.h"
#include <iostream>
#include <stdlib.h> // srand, rand.
#include <time.h>
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

    // Enable mirroring to match right hand coordinates.
    videoStreamDepth.setMirroringEnabled(true);

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
        numSamplePoints = samplingArgA * samplingArgB;
        calcSamplingIndicesByGrid();
        calcSamplingVectors();
        break;
    case SAMPLING_GRID_RANDOM:
        break;
    case SAMPLING_RANDOM:
        numSamplePoints = samplingArgA;
        calcSamplingIndicesByRandom();
        calcSamplingVectors();
        break;
    default:
        break;
    }
}

void OpenniCamera::takeNewDepthPhoto() {
    // Get a frame referenced by mVideoFrameRef.
    videoStreamDepth.readFrame(&videoFrameRefDepth);

    // Get the data array of the frame.
    depthPhoto = (DepthPixel *)videoFrameRefDepth.getData();

    // Sample the photo.
    depthPhotoSampled = new double[numSamplePoints];
    for (int i = 0; i < numSamplePoints; i++)
        depthPhotoSampled[i] = (double)depthPhoto[samplingIndicesOfPhoto[i]] / 1000;
}

int *OpenniCamera::getDepthPhoto() {
    return convertDepthPixelArrayToIntArray(depthPhoto, widthFrame * heightFrame);
}

double *OpenniCamera::getDepthPhotoSampled() {
    return depthPhotoSampled;
}

double *OpenniCamera::getSamplingVectors() {
    return samplingVectors;
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
 * Called by OpenniCamera::setSamplingMethod.
 * Sample points sit at the center of each rectangular.
 * ------------------------------------------------ */
void OpenniCamera::calcSamplingIndicesByGrid() {
    // Index of target array.
    int indexSample = 0;

    // Half width/height of grid.
    double halfWidthGrid  = ((double)widthFrame  / (double)samplingArgA) / 2.0;
    double halfHeightGrid = ((double)heightFrame / (double)samplingArgB) / 2.0;

    // Create int array samplingIndicesOfPhoto.
    samplingIndicesOfPhoto = new int[numSamplePoints];
    samplingPointsXYOnPhoto = new int[numSamplePoints * 2];

    // Fill in the array.
    for (int ih = 0; ih < samplingArgB; ih++)
        for (int iw = 0; iw < samplingArgA; iw++) {
            // Calculate index
            int x = (1 + 2 * iw) * (int)halfWidthGrid;
            int y = (1 + 2 * ih) * (int)halfHeightGrid;
            int indexPhotoArray = convertIntXYToIndexOfPhotoArray(x, y);
            samplingIndicesOfPhoto[ indexSample ] = indexPhotoArray;
            //cout << indexSample << '\t' << samplingIndicesOfPhoto[ indexSample ] << endl;

            // Save x and y.
            samplingPointsXYOnPhoto[indexSample*2    ] = x;
            samplingPointsXYOnPhoto[indexSample*2 + 1] = y;
            indexSample++;
        }
}

/* ------------------------------------------------
 * calcSamplingVectorsByGrid
 * Called by OpenniCamera::setSamplingMethod.
 * OpenniCamera::calcSamplingIndicesByGrid() should have been called,
 * because this method uses the array samplingIndicesOfPhoto.
 * ------------------------------------------------ */
void OpenniCamera::calcSamplingVectors() {
    float *vectorsTemp = new float[numSamplePoints * 3];
    samplingVectors = new double[numSamplePoints * 3];
    // Convert each sampling point.
    for (int i = 0; i < numSamplePoints; i++) {
        CoordinateConverter::convertDepthToWorld(videoStreamDepth,
                                                 samplingPointsXYOnPhoto[i * 2    ],
                                                 samplingPointsXYOnPhoto[i * 2 + 1],
                                                 100,
                                                 &vectorsTemp[i * 3    ],
                                                 &vectorsTemp[i * 3 + 1],
                                                 &vectorsTemp[i * 3 + 2]);
        // Copy to double array.
        samplingVectors[i * 3    ] = (double)vectorsTemp[i * 3    ];
        samplingVectors[i * 3 + 1] = (double)vectorsTemp[i * 3 + 1];
        samplingVectors[i * 3 + 2] = (double)vectorsTemp[i * 3 + 2];
    }
}

int OpenniCamera::convertIntXYToIndexOfPhotoArray(int x, int y) {
    return y * widthFrame + x;
}

/* ------------------------------------------------
 * calcSamplingIndicesByRandom
 * Called by OpenniCamera::setSamplingMethod.
 * Sample point positions choosed randomly.
 * ------------------------------------------------ */
void OpenniCamera::calcSamplingIndicesByRandom() {
    // Create int array samplingIndicesOfPhoto.
    samplingIndicesOfPhoto = new int[numSamplePoints];
    samplingPointsXYOnPhoto = new int[numSamplePoints * 2];

    // Fill in the array.
    for (int i = 0; i < numSamplePoints; i++) {
        // Random seed
        srand(time(NULL));

        // Calculate index
        int x = (int)( widthFrame * ( rand() / (float)(RAND_MAX+1) ) );
        int y = (int)( heightFrame * ( rand() / (float)(RAND_MAX+1) ) );
        int indexPhotoArray = convertIntXYToIndexOfPhotoArray(x, y);
        samplingIndicesOfPhoto[ i ] = indexPhotoArray;
        cout << i << '\t' << samplingIndicesOfPhoto[ i ] << endl;

        // Save x and y.
        samplingPointsXYOnPhoto[i*2    ] = x;
        samplingPointsXYOnPhoto[i*2 + 1] = y;
    }
}