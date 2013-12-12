#include "VectorRotator.h"

#define _USE_MATH_DEFINES
#include <cmath>

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
    R11 = cos( angleZ ) * cos( angleX );
    R21 = sin( angleZ ) * cos( angleX );
    R31 = sin( angleX ) * -1;

    R12 = cos( angleZ ) * sin( angleX ) * sin( angleY )
        - sin( angleZ ) * cos( angleY );
    R22 = sin( angleZ ) * sin( angleX ) * sin( angleY )
        + cos( angleZ ) * cos( angleY );
    R32 = cos( angleX ) * sin( angleY );

    R13 = cos( angleZ ) * sin( angleX ) * cos( angleY )
        + sin( angleZ ) * sin( angleY );
    R23 = sin( angleZ ) * sin( angleX ) * cos( angleY )
        + cos( angleZ ) * sin( angleY );
    R33 = cos( angleX ) * cos( angleY );
}

void VectorRotator::fromCameraToWorld(const double *vectorsIn, double *vectorsOut, int numVectors) {
     // Simple check validality.
    if (vectorsIn != 0 && vectorsOut != 0 && numVectors != 0) {
        double *vectorsTemp = new double[numVectors];
        fromCameraToPhone(vectorsIn, vectorsTemp, numVectors);
        fromPhoneToWorld(vectorsTemp, vectorsOut, numVectors);
    }
}

/* ----------------------------------------------------------------------
 * VectorRotator::fromCameraToPhone
 * Camera coordinates:
 *      Left hand coordinates. Viewing along camera's front direction,
 *          X points to right, Y points to top, Z points to front.
 * Phone coordinates(Android):
 *     Right hand coordinates. Viewing at screen of phone,
 *          X points to right, Y points to top, Z points to you.
 *
 * In current operation, the relations of these coordinates are:
 *      phoneX = cameraZ
 *      phoneY = cameraX * -1
 *      phoneZ = cameraY
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
        for (int i = 0; i < numVectors; i++) {
            vectorsOut[i*3    ] = vectorsIn[i*3 + 2];
            vectorsOut[i*3 + 1] = vectorsIn[i*3    ] * -1;
            vectorsOut[i*3 + 2] = vectorsIn[i*3 + 1];
        }
    }
}

void VectorRotator::fromPhoneToWorld(const double *vectorsIn, double *vectorsOut, int numVectors) {
    // Simple check validality.
    if (vectorsIn != 0 && vectorsOut != 0 && numVectors != 0) {
        // For each vector.
        for (int i = 0; i < numVectors; i++) {
            vectorsOut[i*3    ] = R11 * vectorsIn[i*3    ] +
                                  R12 * vectorsIn[i*3 + 1] +
                                  R13 * vectorsIn[i*3 + 2] ;
            vectorsOut[i*3 + 1] = R21 * vectorsIn[i*3    ] +
                                  R22 * vectorsIn[i*3 + 1] +
                                  R23 * vectorsIn[i*3 + 2] ;
            vectorsOut[i*3 + 2] = R31 * vectorsIn[i*3    ] +
                                  R32 * vectorsIn[i*3 + 1] +
                                  R33 * vectorsIn[i*3 + 2] ;
        }
    }
}

/* -----------------------------------------------------------
 * Private:
 * -----------------------------------------------------------  */

double VectorRotator::degreeToRadian(double degree) {
    return degree * M_PI / 180.0;
}