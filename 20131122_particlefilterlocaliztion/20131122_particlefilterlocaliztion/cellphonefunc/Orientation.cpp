/*------------------CopyLeft(c)  TangLee-----------------*/
/*------------------for referncing code from ric huang*/
#include "Orientation.h"

extern size_t myStrGetTok(const string& str, string& tok, size_t pos, const char del);

//static data memters initialization
float Orientation::_azimuth = 0;
float Orientation::_pitch = 0;
float Orientation::_roll = 0;

float Orientation::get_azimuth() {
    return _azimuth;
}

float Orientation::get_pitch() {
    return _pitch;
}

float Orientation::get_roll() {
    return _roll;
}

Vector3d Orientation::get_direction_V3d () {
    float x = sin( _azimuth * (float)PI / 180 );
    float y = cos( _azimuth * (float)PI / 180 );
    float z = sin( (-1) * _pitch * (float)PI / 180 );
    return Vector3d(x, y, z);
}

// data: [azimuth/pitch/roll]
void Orientation::set_orientation( string data ) {
    string token[3];
    float values[3];
    for (int i = 0; i < 3; i++){
        data = data.substr(myStrGetTok(data, token[i], 0, '/') + 1);
        values[i] = (float)::atof(token[i].c_str());
    }

    set_orientation( values[0], values[1], values[2] );
}

void Orientation::set_orientation( const float azi, const float pit, const float rol ) {
    _azimuth = azi;
    _pitch   = pit;
    _roll    = rol;
}

void Orientation::get_direction_x(double *direction) {
    direction[0] = cos( degreeToRadian(_azimuth) ) * sin( degreeToRadian(_pitch) ) * sin( degreeToRadian(_roll) )
                 - sin( degreeToRadian(_azimuth) ) * cos( degreeToRadian(_roll ) );
    direction[1] = sin( degreeToRadian(_azimuth) ) * sin( degreeToRadian(_pitch) ) * sin( degreeToRadian(_roll) )
                 + cos( degreeToRadian(_azimuth) ) * cos( degreeToRadian(_roll ) );
    direction[2] = cos( degreeToRadian(_pitch  ) ) * sin( degreeToRadian(_roll ) );
}

void Orientation::get_direction_y(double *direction) {
    direction[0] = cos( degreeToRadian(_azimuth) ) * cos( degreeToRadian(_pitch) );
    direction[1] = sin( degreeToRadian(_azimuth) ) * cos( degreeToRadian(_pitch) );
    direction[2] = sin( (-1) * degreeToRadian(_pitch) );
}

void Orientation::get_direction_z(double *direction) {
    direction[0] = cos( degreeToRadian(_azimuth) ) * sin( degreeToRadian(_pitch) ) * cos( degreeToRadian(_roll) )
                 + sin( degreeToRadian(_azimuth) ) * sin( degreeToRadian(_roll ) );
    direction[1] = sin( degreeToRadian(_azimuth) ) * sin( degreeToRadian(_pitch) ) * cos( degreeToRadian(_roll) )
                 + cos( degreeToRadian(_azimuth) ) * sin( degreeToRadian(_roll ) );
    direction[2] = cos( degreeToRadian(_pitch  ) ) * cos( degreeToRadian(_roll ) );
}

double Orientation::degreeToRadian(float degree) {
    return degree * (float)PI / 180;
}