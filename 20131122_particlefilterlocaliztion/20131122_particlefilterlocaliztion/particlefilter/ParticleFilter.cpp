/* ---------------------------------------
 * Code modified from Ian Kuo, 2013/12/7.
 * --------------------------------------- */

#include "ParticleFilter.h"
#include <stdlib.h>
#include <time.h>
#include "../scene/Scene.h"

ParticleFilter::ParticleFilter(void) {
}

ParticleFilter::~ParticleFilter(void) {
}

void ParticleFilter::initialize(Scene* scene) {
    //InitializeComponent();

	_sampleNum = 1000;
	_threshold = 0.00000006;
	_variance = 20;

	//_Length	= PBox->Width;
	//_Width	= PBox->Height;
	//_Height	= 0;

	_OldSampleVec = new vector<ParticleType*>();
	_NewSampleVec = new vector<ParticleType*>();

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
	//	double *CubeNodes = tableCubes[i];
    //
	//	while(CubeNodes[3*i + 2] >= 0)
	//	{
	//		double *nodePos =  _Proj->RealProjectToMap(CubeNodes[3*i], CubeNodes[3*i + 1]);
	//		g->DrawRectangle(_BlackPen, nodePos[0], nodePos[1], 1, 1);
	//	}
    //
	//	i++;
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

vector<ParticleType*>* ParticleFilter::getNewSampleVec() {
    return _NewSampleVec;
}

/* ---------------------------------------
 * Private:
 * --------------------------------------- */

void ParticleFilter::update() {

}