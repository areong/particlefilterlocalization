#ifndef SCENE_SCENE_H
#define SCENE_SCENE_H

class Scene {
public:
    Scene(void);
    ~Scene(void);
    int initialize(char *fileScene, double lengthCubeEdge);
    double takeAShot(double xPoint, double yPoint, double zPoint, 
                     double xFront, double yFront, double zFront,
                     double xTop,   double yTop,   double zTop);
    double calcDistanceFromPointAlongLine(double xStart, double yStart, double zStart, 
                                          double xLine, double yLine, double zLine);
    void setDistanceDefiningPointTouchesLine(double distance);
    
    int XYZtoIndexOfCube(double x, double y, double z);

    int printDPointsXYZ();
    double *getRangeOfScene();
    double **getTableCubes();
    int *getNumPointsEachCube();
    int *getNumCubes();

private:
    double *dPointsXYZ;
    int numPoints;
    double dRangeOfScene[6]; // [xMax, xMin, yMax, yMin, zMax, zMin]
    double **tableCubes;
    double lengthCubeEdge;
    int xNumCubes;
    int yNumCubes;
    int zNumCubes;
    int xyNumCubes;
    int *numPointsEachCube;
    double distanceDefiningPointTouchesLine;

    void calcRangeOfScene();
    void createTableCubes();
    double distanceToNearestPointTouchingTheLine(double xStart, double yStart, double zStart, 
                                                 double xLine, double yLine, double zLine);
    double distanceToNearestPointTouchingTheLineInACube(int indexCube,
                                                        double xStart, double yStart, double zStart,
                                                        double xLine, double yLine, double zLine,
                                                        double lengthLine);
};

#endif
