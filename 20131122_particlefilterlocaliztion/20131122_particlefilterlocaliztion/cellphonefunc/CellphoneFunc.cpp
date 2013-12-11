#include "CellphoneFunc.h"
#include <cstring>
#include <iostream>
#include <process.h>
#include <string>
#include <thread>
#include "Client.h"
#include "Orientation.h"

// Windows.h should be included after WinSock2.h, which is in Client.h.
#include <Windows.h>

using namespace std;


void CellphoneFunc::connectAndReceiveData() {
    if (!startConnection()) {
        cout << endl << "Error connecting the device. Try again." << endl;
        return;
    }
        
    _beginthreadex(NULL, 0,  (unsigned int (__stdcall *)(void *))Connect, NULL, 0, NULL);
}

float CellphoneFunc::getAzimuth() {
    return Orientation::get_azimuth();
}

float CellphoneFunc::getPitch() {
    return Orientation::get_pitch();
}

float CellphoneFunc::getRoll() {
    return Orientation::get_roll();
}

Vector3d CellphoneFunc::getDirection() {
    Vector3d v3d = Orientation::get_direction_V3d();
    v3d.normalize();
    return v3d;
}

void CellphoneFunc::getDirectionX(double *directionX) {
    Orientation::get_direction_x(directionX);
}

void CellphoneFunc::getDirectionY(double *directionY) {
    Orientation::get_direction_y(directionY);
}

void CellphoneFunc::getDirectionZ(double *directionZ) {
    Orientation::get_direction_z(directionZ);
}

/* -------------------------------------------------------
 * Private:
 * -------------------------------------------------------  */

bool CellphoneFunc::startConnection() {
    bool succeeded = true;

    char* cmd = "adb.exe forward tcp:38300 tcp:38300 2>&1";
    string adbRes;

    adbRes = exec(cmd);
    cout << adbRes;
    if  ( adbRes.substr(0,9).compare("\'adb.exe\'") == 0){
        cout << " Please put the \"adb.exe\" in the directory of the project. ";

        while ( adbRes.substr(0,9).compare("\'adb.exe\'") == 0);
    }
    
    cout << "adb.exe detected. \nConnecting device";

    while ( (adbRes = exec(cmd)).substr(0, 3).compare("err") == 0 ){
        Sleep(500);
        cout << ".";
    }

    cout << adbRes << endl << endl ;
    cout << "connect device success!" << endl << endl;

    cout << " 1. Please Open the sensor APP on you device" << endl << endl;
    cout << " 2. PRESS the CONNECT button in the APP. " << endl << endl;
    cout << " 3. Finally, press any key to continue......." << endl << endl;
    system("pause");
    
    return succeeded;
}

string CellphoneFunc::exec(char* cmd) {
    FILE* pipe = _popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
        if(fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    _pclose(pipe);
    return result;
}

void CellphoneFunc::Connect() {
    string msg;
    Client client("127.0.0.1");

    if (!client.Start()){
        return;
    }
 
    cout << endl << "Connected!" << endl << endl;

    while(true)
    {
        /* 
        cout << "Send: ";
        getline(cin, msg);
        // Close the connection when user enter "close"
        if (msg.compare("close") == 0)
            break;
        */
        //client.Send((char*)msg.c_str());

        client.Recv();
    }
 
    client.Stop();
 
    getchar();
    return;
}