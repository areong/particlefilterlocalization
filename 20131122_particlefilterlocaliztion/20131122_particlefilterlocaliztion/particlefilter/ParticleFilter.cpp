/* ---------------------------------------
 * Code modified from Ian Kuo, 2013/12/7.
 * --------------------------------------- */

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include "ParticleFilter.h"
#include <cmath>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "../scene/Scene.h"
using namespace std;

//#define M_PI       3.14159265358979323846

ParticleFilter::ParticleFilter(void) {
}

ParticleFilter::~ParticleFilter(void) {
}

void ParticleFilter::initialize(Scene* scene) {
    //InitializeComponent();

    _sampleNum = 1000;
    _threshold = 0.00000006;
    //_variance = 20;
    _standardDeviation = 0.01;
    _variance = _standardDeviation * _standardDeviation;
    

    //_Length    = PBox->Width;
    //_Width    = PBox->Height;
    //_Height    = 0;

    _OldSampleVec = new vector<ParticleType*>();
    _NewSampleVec = new vector<ParticleType*>();
    
    _stdDevHstry = new double[3];
    for(int i = 0; i < 3; i++)
        _stdDevHstry[i] = 0;

    srand( (unsigned)time( NULL ) );

    //g = Graphics::FromImage(PBox->Image);

    //_skyBluePen = gcnew Pen( Brushes::DeepSkyBlue);
    //_skyBluePen->Width = 8.0F;

    //_RedPen = gcnew Pen( Brushes::Red);
    //_RedPen->Width = 8.0F;

    //_BlackPen = gcnew Pen( Brushes::Black);
    //_BlackPen->Width = 8.0F;

    sceneMain = scene;

    // Initialize Scene, set file path, and the length of edge of cubes (Unit is meter)
    //(*sceneMain).initialize("res/captured.ply", 1);

    // Set the distance defining whether a point touches a line (Unit is meter)
    //(*sceneMain).setDistanceDefiningPointTouchesLine(0.1);

    // Get the range of scene. Returns [xMax, xMin, yMax, yMin, zMax, zMin].
    _RangeOfScene = sceneMain->getRangeOfScene();

    //_Proj = gcnew Projector(_Length, _Width, _RangeOfScene[0], _RangeOfScene[1], _RangeOfScene[2], _RangeOfScene[3], _RangeOfScene[4], _RangeOfScene[5]);

    double val[] = {_RangeOfScene[0] - _RangeOfScene[1], _RangeOfScene[2] - _RangeOfScene[3]};
    _SceneSize = &val[0];


    // Get the table of cubes.
    // Return:
    //  A double two-dimensional array.
    //  First dimension is the array of cubes.
    //  Second dimension is an array of points' coordinates, i.e., [p1x, p1y, p1z, p2x, p2y, p2z,...]
    // CAUTION: DO NOT CHANGE THE CONTENT OF IT!
    //double **tableCubes = (*sceneMain).getTableCubes();
    //int *numPointsEachCube = (*sceneMain).getNumPointsEachCube();
    //int i = 0;
    //while(tableCubes[i][2] == 0)
    //{
    //    double *CubeNodes = tableCubes[i];
    //
    //    while(CubeNodes[3*i + 2] >= 0)
    //    {
    //        double *nodePos =  _Proj->RealProjectToMap(CubeNodes[3*i], CubeNodes[3*i + 1]);
    //        g->DrawRectangle(_BlackPen, nodePos[0], nodePos[1], 1, 1);
    //    }
    //
    //    i++;
    //}


    for (int i = 0; i < _sampleNum; i++)
    {
        ParticleType* sample = new ParticleType();

        int rMax = RAND_MAX;
                
        //unsigned int x = (double) rand() / (RAND_MAX + 1) * _SceneSize[0] + 1;
        //unsigned int y = (double) rand() / (RAND_MAX + 1) * _SceneSize[1] + 1;
        //unsigned int z = 0;
        double x = (double) rand() / (RAND_MAX + 1) * (_RangeOfScene[0] - _RangeOfScene[1]) + _RangeOfScene[1];
        double y = (double) rand() / (RAND_MAX + 1) * (_RangeOfScene[2] - _RangeOfScene[3]) + _RangeOfScene[3];
        double z = (double) rand() / (RAND_MAX + 1) * (_RangeOfScene[4] - _RangeOfScene[5]) + _RangeOfScene[5];

        sample->position[0] = x;
        sample->position[1] = y;
        sample->position[2] = z;
        sample->azimuth = 0;
        sample->elevation = 0;
        sample->oldweight = (double) 1 / _sampleNum;

        //double *mapPos = _Proj->RealProjectToMap(x, y);

        //g->DrawRectangle(_skyBluePen, mapPos[0], mapPos[1], 1, 1);

        _NewSampleVec->push_back(sample);
    }
}

void ParticleFilter::update() {
    //double *realObsrvLocation = _Proj->MapProjectToReal(e->Location.X, e->Location.Y);
    double realObsrvLocation[2] = {-3, -1};

    //this->ObsrvNode->Location = e->Location;

    //g->Clear(System::Drawing::Color::White);
    //PBox->Refresh();
    
    // Clear memory.
    for (int i = 0; i < _OldSampleVec->size(); i++)
        delete (*_OldSampleVec)[i];
    _OldSampleVec->clear();
    delete _OldSampleVec;

    _OldSampleVec = _NewSampleVec;
    _NewSampleVec = new vector<ParticleType*>();
                
    /*
    int obsrv_distFrwd = ObsrvNode->Location.X;
    int obsrv_distBack = this->Height - obsrv_distFrwd;
    int obsrv_distLeft = ObsrvNode->Location.Y;
    int obsrv_distRight = this->Width - obsrv_distLeft;

    int obsrv_Sum = obsrv_distFrwd + obsrv_distBack + obsrv_distLeft + obsrv_distRight;
    */

    // Calculate the distance from a point to the scene along a line.
    // Parameters:
    //  1-3: Point's x,y,z coordinates.
    //  4-6: Line's direction vector. Not being a unit vector is okay.
    // Return:
    //  The distance. Unit is meter.
    //  Returns -1 if point out of range.
    //double obsrv_x_pos = (*sceneMain).calcDistanceFromPointAlongLine(realObsrvLocation[0], realObsrvLocation[1], 0, 1, 0, 0);
    //double obsrv_y_pos = (*sceneMain).calcDistanceFromPointAlongLine(realObsrvLocation[0], realObsrvLocation[1], 0, 0, 1, 0);
    //double obsrv_z_pos = (*sceneMain).calcDistanceFromPointAlongLine(realObsrvLocation[0], realObsrvLocation[1], 0, 0, 0, 1);
    //double obsrv_Sum = (*sceneMain).calcDistanceFromPointAlongLine(realObsrvLocation[0], realObsrvLocation[1], 0, 0, 0, 0);
    //double obsrv_Sum = obsrv_x_pos + obsrv_y_pos + obsrv_z_pos;

    double normalFactor = 0;

    double meanOfDiff = 0;

    // Calcute the weight to prepare sampling
    for (int i = 0; i < _OldSampleVec->size(); i++)
    {
        ParticleType* sample = (*_OldSampleVec)[i];

        /*
        int smpl_distFrwd = sample->position[0];
        int smpl_distBack = this->Height - smpl_distFrwd;
        int smpl_distLeft = sample->position[1];
        int smpl_distRight = this->Width - smpl_distLeft;

        int smpl_Sum = smpl_distFrwd + smpl_distBack + smpl_distLeft + smpl_distRight;

        double diffFrwd = pow((double)smpl_distFrwd - obsrv_distFrwd, 2);
        double diffBack = pow((double)smpl_distBack - obsrv_distBack, 2);
        double diffLeft = pow((double)smpl_distLeft - obsrv_distLeft, 2);
        double diffRight = pow((double)smpl_distRight - obsrv_distRight, 2);

        double diff = diffFrwd + diffBack + diffLeft + diffRight;
        */

        //double smpl_x_pos = (*sceneMain).calcDistanceFromPointAlongLine(sample->position[0], sample->position[1], sample->position[2], 1, 0, 0);
        //double smpl_y_pos = (*sceneMain).calcDistanceFromPointAlongLine(sample->position[0], sample->position[1], sample->position[2], 0, 1, 0);
        //double smpl_z_pos = (*sceneMain).calcDistanceFromPointAlongLine(sample->position[0], sample->position[1], sample->position[2], 0, 0, 1);
        //double smpl_Sum = (*sceneMain).calcDistanceFromPointAlongLine(sample->position[0], sample->position[1], sample->position[2], sample->azimuth, sample->elevation, 0);
        //double smpl_Sum = smpl_x_pos + smpl_y_pos + smpl_z_pos;

        //double diff = pow(smpl_x_pos - obsrv_x_pos, 2) + pow(smpl_y_pos - obsrv_y_pos, 2) + pow(smpl_z_pos - obsrv_z_pos, 2);
        
        double diff = callbackParticleEvaluation(sample->position[0],
                                                 sample->position[1],
                                                 sample->position[2]);

        //cout << i << ":\t" << diff << endl;
        meanOfDiff += diff;

        double P_fromXtoY = 1.0 / (diff + 0.01);

        sample->weight = P_fromXtoY * 1;

        normalFactor += sample->weight;
    }

    double mean = normalFactor / _OldSampleVec->size();
    meanOfDiff /= _OldSampleVec->size();

    cout << "mean: " << mean << endl;
    cout << "meanOfDiff: " << meanOfDiff << endl;

    double var = 0;
    for (int i = 0; i < _OldSampleVec->size(); i++)
    {
        var += pow((*_OldSampleVec)[i]->weight - mean, 2);
    }
    var /= _OldSampleVec->size();
    double stdDev = sqrt(var);
    _threshold = mean + 0.1 * stdDev;
    
    // Method 1
    //_stdDevHstry[2] = _stdDevHstry[1];
    //_stdDevHstry[1] = _stdDevHstry[0];
    //_stdDevHstry[0] = stdDev;
    //double stdDevChange = (_stdDevHstry[0] - _stdDevHstry[2]) / 2;
    //_variance = 0.98 * _variance * exp(stdDevChange * 1000);

    // Method 2
    //if (meanOfDiff > 0.5 && _variance < 0.2)
    //    _variance *= 1.1;
    //else if (meanOfDiff <= 0.5 && _variance > 0.01)
    //    _variance *= 0.9;

    // Method 3
    if (meanOfDiff > 10)
        _standardDeviation = 2;
    else if (meanOfDiff < 0.1)
        _standardDeviation = 0.02;
    else
        _standardDeviation = meanOfDiff / 5;
    _variance = _standardDeviation * _standardDeviation;

    cout << "_variance: " << _variance << endl;

    normalFactor = 0;
    for (int i = 0; i < _OldSampleVec->size(); i++)
    {
        ParticleType* sample = (*_OldSampleVec)[i];
        if (sample->weight < _threshold)
        sample->weight = 0;
        normalFactor += sample->weight;
    }

    // Normalize the weight
    for (int i = 0; i < _OldSampleVec->size(); i++)
    {
        ParticleType* sample = (*_OldSampleVec)[i];
        sample->weight /= normalFactor;
    }
    
    // Sampling with Gaussian distribution 
    for (int i = 0; i < _OldSampleVec->size(); i++)
    {
        ParticleType* sample = (*_OldSampleVec)[i];
                     
        int gen_smpl_num = _sampleNum * sample->weight;

        if (gen_smpl_num == 0) continue;

        int num = 0; 
                     
        //double* mapPos = _Proj->RealProjectToMap(sample->position[0], sample->position[1]);

        //g->DrawRectangle(_RedPen, mapPos[0], mapPos[1], 1, 1);

        while (num < gen_smpl_num)
        {
            ParticleType* newSmpl = new ParticleType();
            
            double rseed = 0; // To generate r.
            double theta = 0;
            double phi = 0;

            while (rseed == 0) {
                rseed = (double) rand() / (RAND_MAX + 1);
                theta = (double) rand() / (RAND_MAX + 1);
                phi = (double) rand() / (RAND_MAX + 1);
            }
            
            double x = sqrt(-2 * log(rseed)) * _standardDeviation
                     * sin(M_PI * theta) * cos(2 * M_PI * phi)
                     + sample->position[0];
            double y = sqrt(-2 * log(rseed)) * _standardDeviation
                     * sin(M_PI * theta) * sin(2 * M_PI * phi)
                     + sample->position[1];
            double z = sqrt(-2 * log(rseed)) * _standardDeviation
                     * cos(M_PI * theta)
                     + sample->position[2];
            // double x = sqrt(-2 * log(r))...
            //double x = (u-0.5) * 5 * _variance + sample->position[0];
            //double y = (v-0.5) * 5 * _variance + sample->position[1];
            //double z = (w-0.5) * 5 * _variance + sample->position[2];
            
            newSmpl->position[0] = x;
            newSmpl->position[1] = y;
            newSmpl->position[2] = z;
            newSmpl->azimuth = 0;
            newSmpl->elevation = 0;
            newSmpl->oldweight = sample->weight;
            _NewSampleVec->push_back(newSmpl);

            //mapPos = _Proj->RealProjectToMap(sample->position[0], sample->position[1]);

            //g->DrawRectangle(_skyBluePen, mapPos[0], mapPos[1], 1, 1);
            num++;
        }
    }

    //PBox->Refresh();

    
}

vector<ParticleType*>* ParticleFilter::getNewSampleVec() {
    return _NewSampleVec;
}

void ParticleFilter::setCallbackParticleEvaluation(double (*cb)(double, double, double)) {
    callbackParticleEvaluation = cb;
}

/* ---------------------------------------
 * Private:
 * --------------------------------------- */
