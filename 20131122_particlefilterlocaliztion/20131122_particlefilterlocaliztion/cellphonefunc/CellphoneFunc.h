#ifndef CELLPHONE_FUNC_H
#define CELLPHONE_FUNC_H

#include "Vector3d.h"

class CellphoneFunc {
public:
    void connectAndReceiveData();
    float getAzimuth();
    float getPitch();
    float getRoll();
    Vector3d getDirection();
    void getDirectionX(double *directionX);
    void getDirectionY(double *directionY);
    void getDirectionZ(double *directionZ);

private:
    static bool startConnection();
    static string exec(char* cmd);
    static void Connect();
};

#endif