#include "VectorRotator.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
using namespace std;

VectorRotator::VectorRotator(void) {
    angleX = 0;
    angleY = 0;
    angleZ = 0;
    R11 = 0;
    R21 = 0;
    R31 = 0;
    R12 = 0;
    R22 = 0;
    R32 = 0;
    R13 = 0;
    R23 = 0;
    R33 = 0;
}

VectorRotator::~VectorRotator(void) {
}

void VectorRotator::setPhoneAngles(double angleXIn, double angleYIn, double angleZIn) {
    angleX = degreeToRadian( angleXIn );
    angleY = degreeToRadian( angleYIn );
    angleZ = degreeToRadian( angleZIn );

    // Compute rotation matrix.
    // Ref: http://planning.cs.uiuc.edu/node102.html
    R11 = cos( angleZ ) * cos( angleY );
    R21 = sin( angleZ ) * cos( angleY );
    R31 = sin( angleY ) * -1;    

    R12 = cos( angleZ ) * sin( angleY ) * sin( angleX ) * 1
        - sin( angleZ ) * cos( angleX ) * 1;
    R22 = sin( angleZ ) * sin( angleY ) * sin( angleX ) * 1
        + cos( angleZ ) * cos( angleX ) * 1;
    R32 = cos( angleY ) * sin( angleX ) * 1;

    R13 = cos( angleZ ) * sin( angleY ) * cos( angleX ) * 1
        + sin( angleZ ) * sin( angleX ) * 1;
    R23 = sin( angleZ ) * sin( angleY ) * cos( angleX ) * 1
        + cos( angleZ ) * sin( angleX ) * 1;
    R33 = cos( angleY ) * cos( angleX ) * 1;
}

void VectorRotator::fromCameraToWorld(const double *vectorsIn, double *vectorsOut, int numVectors) {
     // Simple check validality.
    if (vectorsIn != 0 && vectorsOut != 0 && numVectors != 0) {
        double *vectorsTemp = new double[numVectors];
        fromCameraToPhone(vectorsIn, vectorsTemp, numVectors);
        //cout << vectorsTemp[0] << '\t' << vectorsTemp[1] << '\t' << vectorsTemp[2] << endl;
        fromPhoneToWorld(vectorsTemp, vectorsOut, numVectors);
        //cout << vectorsOut[0] << '\t' << vectorsOut[1] << '\t' << vectorsOut[2] << endl;
    }
}

/* ----------------------------------------------------------------------
 * VectorRotator::fromCameraToPhone
 * Camera coordinates:
 *      Right hand coordinates(mirroring enabled). Viewing along camera's front direction,
 *          X points to right, Y points to top, Z points to front.
 * Phone coordinates(Android):
 *      Right hand coordinates. Viewing at screen of phone,
 *          X points to left, Y points to top, Z points to back of screen.
 *
 * In current operation, the relations of these coordinates are:
 *      phoneX = cameraZ * -1
 *      phoneY = cameraX (mirroring enabled)
 *      phoneZ = cameraY * -1
 *
 *      Figure: Top view of putting phone on top of camera.
 * 
 *                  Right
 *           ------------------
 *          |                  |
 *      Top |       Phone      |
 *           ================== 
 *
 *                  Front
 *         ----------------------
 *        |                      |
 *        |        Camera        |  Right
 *         ======================
 *
 * Length of both array should equal to numVectors.
 * ----------------------------------------------------------------------  */
void VectorRotator::fromCameraToPhone(const double *vectorsIn, double *vectorsOut, int numVectors) {
    // Simple check validality.
    if (vectorsIn != 0 && vectorsOut != 0 && numVectors != 0) {
        // For each vector.
        double length;
        for (int i = 0; i < numVectors; i++) {
            // Normalize.
            length = calcLengthVector(vectorsIn[i*3    ],
                                      vectorsIn[i*3 + 1],
                                      vectorsIn[i*3 + 2]);
            vectorsOut[i*3    ] = vectorsIn[i*3 + 2] * -1 / length;
            vectorsOut[i*3 + 1] = vectorsIn[i*3    ] *  1 / length;
            vectorsOut[i*3 + 2] = vectorsIn[i*3 + 1] * -1 / length;
        }
    }
}

/* --------------------------------------------------
 * VectorRotator::fromPhoneToWorld
 * In Android's getOrientation(), the world coordinates are:
 *      X points to West, Y points to North, Z points to down.
 * However we want:
 *      X points to East, Y points to North, Z points to up.
 * Thus multiply -1 to X and Z.
 * --------------------------------------------------  */
void VectorRotator::fromPhoneToWorld(const double *vectorsIn, double *vectorsOut, int numVectors) {
    // Simple check validality.
    if (vectorsIn != 0 && vectorsOut != 0 && numVectors != 0) {
        // For each vector.
        double length;
        for (int i = 0; i < numVectors; i++) {
            // Normalize.
            length = calcLengthVector(vectorsIn[i*3    ],
                                      vectorsIn[i*3 + 1],
                                      vectorsIn[i*3 + 2]);
            vectorsOut[i*3    ] = (R11 * vectorsIn[i*3    ] +
                                   R12 * vectorsIn[i*3 + 1] +
                                   R13 * vectorsIn[i*3 + 2] ) / length * -1;
            vectorsOut[i*3 + 1] = (R21 * vectorsIn[i*3    ] +
                                   R22 * vectorsIn[i*3 + 1] +
                                   R23 * vectorsIn[i*3 + 2] ) / length;
            vectorsOut[i*3 + 2] = (R31 * vectorsIn[i*3    ] +
                                   R32 * vectorsIn[i*3 + 1] +
                                   R33 * vectorsIn[i*3 + 2] ) / length * -1;
        }
    }
}

/* -----------------------------------------------------------
 * Private:
 * -----------------------------------------------------------  */

double VectorRotator::degreeToRadian(double degree) {
    return degree * M_PI / 180.0;
}

double VectorRotator::calcLengthVector(double x, double y, double z) {
    return sqrt(x * x + y * y + z * z);
}