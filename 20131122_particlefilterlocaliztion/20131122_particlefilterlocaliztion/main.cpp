#include <stdlib.h>
#include <iostream>
#include "Scene.h"

int main() {
	Scene sceneMain;
	sceneMain.initialize("res/captured.ply", 1);
	//cout << sceneMain.printDPointsXYZ() << endl;
	system("pause");
	return 0;
}