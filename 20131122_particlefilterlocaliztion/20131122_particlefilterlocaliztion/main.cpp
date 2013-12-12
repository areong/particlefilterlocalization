#include <stdlib.h>
#include <iostream>
#include "cellphonefunc/CellphoneFunc.h"
#include "opennicamera/OpenniCamera.h"
#include "particlefilter/ParticleFilter.h"
#include "scene/Scene.h"
#include "utilities/VectorRotator.h"
#include "viewer/DataDisplayerParticles.h"
#include "viewer/DataDisplayerTableCubes.h"
#include "viewer/Viewer.h"
using namespace std;

void callbackInViewerMainLoopBeforeDrawing();
void callbackAfterViewerMainLoop();
double callbackParticleEvaluation(double x, double y, double z);

// Create Scene object.
Scene sceneMain;

// Create OpenniCamera object.
OpenniCamera camera;
int numSamplingPointsW = 1;
int numSamplingPointsH = 1;
double *samplingVectorsCamera;
double samplingVectorsWorld[3];
double *depthPhotoSampled;

ParticleFilter particleFilter;

CellphoneFunc cpf;

VectorRotator vectorRotator;
const double vectorX[3] = {1, 0, 0};
const double vectorY[3] = {0, 1, 0};
const double vectorZ[3] = {0, 0, 1};

// X to East, Y to North, Z to Up.
double directionPhoneX[3];
double directionPhoneY[3];
double directionPhoneZ[3];

int main(int argc, char** argv) {
    
    // Connect to an Android device.
    cpf.connectAndReceiveData();

    // Initialize.
    // Parameters:
    //  1. File path of the ply file.
    //  2. Length of edge of cubes, used in cutting the scene into cubes. Unit is meter.
    sceneMain.initialize("res/captured.ply", 1);

    // Set the distance defining whether a point touches a line,
    // used in calculating distances later.
    // Parameters:
    //  1. The distance. Unit is meter.
    sceneMain.setDistanceDefiningPointTouchesLine(0.1);

    // Calculate the distance from a point to the scene along a line.
    // Parameters:
    //  1-3: Point's x,y,z coordinates.
    //  4-6: Line's direction vector. Not being a unit vector is okay.
    // Return:
    //  The distance. Unit is meter.
    //  Returns -1 if point out of range.
    double distance = sceneMain.calcDistanceFromPointAlongLine(0, 0, 0, 1, 0, 0);
    cout << distance << endl;
    distance = sceneMain.calcDistanceFromPointAlongLine(0, 0, 0, 0, 1, 0);
    cout << distance << endl;

    // Get the range of scene.
    // Return:
    //  A double array, which is [xMax, xMin, yMax, yMin, zMax, zMin].
    //  These are integral multiples of the edge of cubes, which is set previously.
    // CAUTION: DO NOT CHANGE THE CONTENT OF IT!
    double *rangeOfScene = sceneMain.getRangeOfScene();
    for (int i = 0; i < 3; i++)
        cout << i << ": "<< '\t' << rangeOfScene[i*2] << '\t' << rangeOfScene[i*2+1] << endl;

    // Get the table of cubes.
    // Return:
    //  A double two-dimensional array.
    //  First dimension is the array of cubes.
    //  Second dimension is an array of points' coordinates, i.e., [p1x, p1y, p1z, p2x, p2y, p2z,...]
    // CAUTION: DO NOT CHANGE THE CONTENT OF IT!
    double **tableCubes = sceneMain.getTableCubes();

    int indexCube = sceneMain.XYZtoIndexOfCube(-3,-1,4);
    cout << indexCube << endl;

    // Get number of points of each cube.
    // Return:
    //  A integer array. The index is the same as the cube.
    // CAUTION: DO NOT CHANGE THE CONTENT OF IT!
    int *numPointsEachCube = sceneMain.getNumPointsEachCube();
    //int indexCube = 0;
    if (numPointsEachCube[indexCube] > 0) {
        cout << numPointsEachCube[indexCube] << endl;
        cout << tableCubes[indexCube][0] << '\t'<< tableCubes[indexCube][1] << '\t' << tableCubes[indexCube][2] << endl;
    }
    else
        cout << "No point in cube no. " << indexCube << endl;

    // VectorRotator
    // Set offset 90 degrees, which makes no need to rotate model manually.
    vectorRotator.setPhoneAngleZOffset(-90);

    // Camera
    camera.initialize();
    camera.setSamplingMethod(SAMPLING_GRID, numSamplingPointsW, numSamplingPointsH);
    camera.takeNewDepthPhoto();
    //int *photo = camera.getDepthPhoto();
    samplingVectorsCamera = camera.getSamplingVectors();
    //for (int i = 0; i < numSamplingPointsW * numSamplingPointsH; i++) {
    //    cout << samplingVectorsCamera[i * 3    ] << ",\t" <<
    //            samplingVectorsCamera[i * 3 + 1] << ",\t" <<
    //            samplingVectorsCamera[i * 3 + 2] << endl;
    //}

    // Particle filter
    particleFilter.initialize(&sceneMain);
    // Temporary
    particleFilter.setCallbackParticleEvaluation(&callbackParticleEvaluation);
    
    DataDisplayerTableCubes dataDisplayerTableCubes;
    dataDisplayerTableCubes.setScene(&sceneMain);

    DataDisplayerParticles dataDisplayerParticles;
    dataDisplayerParticles.setParticleFilter(&particleFilter);

    Viewer viewerMain("Particle Filter Localization");
    viewerMain.initialize(argc, argv);
    viewerMain.setCallbackInMainLoopBeforeDrawing(&callbackInViewerMainLoopBeforeDrawing);
    viewerMain.setCallbackKeyEsc(&callbackAfterViewerMainLoop);
    
    viewerMain.addDataDisplayer(&dataDisplayerTableCubes);
    viewerMain.addDataDisplayer(&dataDisplayerParticles);

    viewerMain.runMainLoop();

    

    system("pause");
    return 0;
}

void callbackInViewerMainLoopBeforeDrawing() {
    //cout << "Azimuth: " << cpf.getAzimuth() << endl
    //     << "  Pitch: " << cpf.getPitch() << endl
    //     << "   Roll: " << cpf.getRoll() << endl;

    vectorRotator.setPhoneAngles(cpf.getPitch(), cpf.getRoll(), cpf.getAzimuth());
    
    //vectorRotator.fromPhoneToWorld(vectorX, directionPhoneX, 1);
    //vectorRotator.fromPhoneToWorld(vectorY, directionPhoneY, 1);
    //vectorRotator.fromPhoneToWorld(vectorZ, directionPhoneZ, 1);
    vectorRotator.fromCameraToWorld(samplingVectorsCamera, samplingVectorsWorld, 1);

    //cpf.getDirectionX(directionPhoneX);
    //cpf.getDirectionY(directionPhoneY);
    //cpf.getDirectionZ(directionPhoneZ);

    //cout << "x = ( " << directionPhoneX[0] << "\t, "
    //                 << directionPhoneX[1] << "\t, "
    //                 << directionPhoneX[2] << " )" << endl;
    //cout << "y = ( " << directionPhoneY[0] << "\t, "
    //                 << directionPhoneY[1] << "\t, "
    //                 << directionPhoneY[2] << " )" << endl;
    //cout << "z = ( " << directionPhoneZ[0] << "\t, "
    //                 << directionPhoneZ[1] << "\t, "
    //                 << directionPhoneZ[2] << " )" << endl;
    cout << samplingVectorsWorld[0] << "\t, " <<
            samplingVectorsWorld[1] << "\t, " <<
            samplingVectorsWorld[2] << endl;

    camera.takeNewDepthPhoto();
    depthPhotoSampled = camera.getDepthPhotoSampled();
    //cout << depthPhotoSampled[0] << endl;
    
    //double result;
    //result = sceneMain.takeAShotAndEvaluate(0, 0, 0,
    //                                        samplingVectorsWorld,
    //                                        numSamplingPointsW * numSamplingPointsH,
    //                                        depthPhotoSampled);
    //
    //cout << result << endl;

    //cout << "particleFilter.update();" << endl;
    particleFilter.update();
}

void callbackAfterViewerMainLoop() {
    cout << "After glutMainLoop()." << endl;
    //camera.shutdown();
}

double callbackParticleEvaluation(double x, double y, double z) {
    return sceneMain.takeAShotAndEvaluate(x, y, z,
                                          samplingVectorsWorld,
                                          numSamplingPointsW * numSamplingPointsH,
                                          depthPhotoSampled);
}