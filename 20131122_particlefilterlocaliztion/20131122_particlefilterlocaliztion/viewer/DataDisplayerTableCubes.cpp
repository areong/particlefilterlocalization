#include "DataDisplayerTableCubes.h"
#include <windows.h>
#include <glut.h>

DataDisplayerTableCubes::DataDisplayerTableCubes(void) {
}

DataDisplayerTableCubes::~DataDisplayerTableCubes(void) {
}

void DataDisplayerTableCubes::display() {
    // Draw a Red 1x1 Square centered at origin
    glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
        glColor3f(1.0f, 0.0f, 0.0f); // Red
        glVertex2f(-0.5f, -0.5f);    // x, y
        glVertex2f( 0.5f, -0.5f);
        glVertex2f( 0.5f,  0.5f);
        glVertex2f(-0.5f,  0.5f);
    glEnd();
}