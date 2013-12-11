#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <iostream>
#include <math.h>

using namespace std;

#define PI 3.14159265

class Vector3d {
public:
    inline Vector3d () {}
    inline Vector3d (float X, float Y, float Z): x(X), y(Y), z(Z){} 
    inline Vector3d operator + (const Vector3d& v2) { return Vector3d( x + v2.x, y + v2.y, z+ v2.z ); }
    friend ostream & operator << (ostream& out, const Vector3d& v) {
        out << "( " << v.x << " , " << v.y << " , " << v.z << " )";
        return out;
    }

    inline Vector3d& normalize() {
        float t = sqrt( pow(x, 2) + pow(y, 2) + pow(z, 2) );
        x = x / t;
        y = y / t;
        z = z / t;
        return *this;
    }

    float x;
    float y;
    float z;
};

#endif