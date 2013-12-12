#ifndef UTILITIES_VECTORROTATOR_H
#define UTILITIES_VECTORROTATOR_H

class VectorRotator {
public:
    VectorRotator(void);
    ~VectorRotator(void);
    void setPhoneAngles(double angleX, double angleY, double angleZ);
    void setPhoneAngleZOffset(double offset);
    void fromCameraToWorld(const double *vectorsIn, double *vectorsOut, int numVectors);
    void fromCameraToPhone(const double *vectorsIn, double *vectorsOut, int numVectors);
    void fromPhoneToWorld(const double *vectorsIn, double *vectorsOut, int numVectors);

private:
    // Defined by Android. Unit is radian.
    double angleX;
    double angleY;
    double angleZ;
    double angleZOffset;

    // Rotation matrix
    double R11, R12, R13,
           R21, R22, R23,
           R31, R32, R33;

    double degreeToRadian(double degree);
    double calcLengthVector(double x, double y, double z);
};

#endif