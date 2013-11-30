#include <stdlib.h>
#include <iostream>
#include "Scene.h"

int main() {
	Scene sceneMain;
	sceneMain.initialize("res/captured.ply", 1);
	system("pause");
	return 0;
}