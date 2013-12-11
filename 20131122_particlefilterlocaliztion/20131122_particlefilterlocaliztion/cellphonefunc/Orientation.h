/*------------------CopyLeft(c)  TangLee-----------------*/
/*------------------for referncing code from ric huang*/
#ifndef ORIENTATION_H
#define ORIENTATION_H

#include "Vector3d.h"

class Orientation {
public:    
    static float get_azimuth();
    static float get_pitch();
    static float get_roll();

    static Vector3d get_direction_V3d();

    // data: [azimuth/pitch/roll]
    static void set_orientation( string data );
    
    static void set_orientation( const float azi, const float pit, const float rol );

    static void get_direction_x(double *direction);
    static void get_direction_y(double *direction);
    static void get_direction_z(double *direction);

    static double degreeToRadian(float degree);

private:
    static float _azimuth ;    
    static float _pitch ; 
    static float _roll ;
};

#endif