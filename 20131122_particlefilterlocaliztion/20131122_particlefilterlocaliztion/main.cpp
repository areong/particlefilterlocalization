#include <stdlib.h>
#include <iostream>
#include "Scene.h"

int main() {
	Scene sceneMain;
	sceneMain.initialize("res/captured.ply", 1);
	sceneMain.setDistanceDefiningPointTouchesLine(0.1);
	double distance = sceneMain.takeAShot(-1, 0, 0, -0.5, 0.5, 1, 0, 0, 1);
	cout << distance << endl;
	system("pause");
	return 0;
}