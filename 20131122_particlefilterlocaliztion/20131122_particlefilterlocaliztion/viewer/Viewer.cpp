/* ----------------------------------------
 * Learned from SimpleViewer of OpenNI2.
 * ---------------------------------------- */

#include "Viewer.h"

Viewer::Viewer(char* title) {
    self = this;
    titleWindow = title;
}

Viewer::~Viewer(void) {
    self = NULL;
}

int Viewer::initialize(int argc, char** argv) {
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(500, 400);
	glutCreateWindow (titleWindow);
	
    // 	glutFullScreen();
	//glutSetCursor(GLUT_CURSOR_NONE);

	initOpenGLCallbacks();

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	return 0;
}

void Viewer::runMainLoop() {
    glutMainLoop();
}

/* ----------------------------------------
 * Protected:
 * ---------------------------------------- */

void Viewer::display() {
}

void Viewer::onKey(unsigned char key, int /*x*/, int /*y*/) {
	switch (key) {
	case 27:
		exit (1);
        break;
	}

}

void Viewer::initOpenGLCallbacks() {
	glutKeyboardFunc(glutKeyboard);
	glutDisplayFunc(glutDisplay);
	glutIdleFunc(glutIdle);
}

/* ----------------------------------------
 * Privated:
 * ---------------------------------------- */
Viewer* Viewer::self = NULL;

void Viewer::glutIdle() {
	glutPostRedisplay();
}

void Viewer::glutDisplay() {
	Viewer::self->display();
}

void Viewer::glutKeyboard(unsigned char key, int x, int y) {
	Viewer::self->onKey(key, x, y);
}