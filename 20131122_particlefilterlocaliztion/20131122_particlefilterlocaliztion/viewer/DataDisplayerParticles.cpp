#include "DataDisplayerParticles.h"
#include <glut.h>

DataDisplayerParticles::DataDisplayerParticles(void) {
}

DataDisplayerParticles::~DataDisplayerParticles(void) {
}

void DataDisplayerParticles::display() {
    // Get sample vector.
    sampleVec = particleFilter->getNewSampleVec();

    double sizeHalf = 0.005;
    double sizeSceneX = 8;
    double sizeSceneY = 7;
    double x;
    double y;

    // Draw each sample.
    for (unsigned int i = 0; i < sampleVec->size(); i++) {
        x = ((*sampleVec)[i]->position[0] - (-4)) / sizeSceneX * 2 - 1;
        y = ((*sampleVec)[i]->position[1] - (-3)) / sizeSceneY * 2 - 1;

        glBegin(GL_QUADS);
            glColor3f(0.0f, 1.0f, 1.0f); // Cyan
            glVertex2f(x - sizeHalf, y - sizeHalf);
            glVertex2f(x + sizeHalf, y - sizeHalf);
            glVertex2f(x + sizeHalf, y + sizeHalf);
            glVertex2f(x - sizeHalf, y + sizeHalf);
        glEnd();
    }
}

void DataDisplayerParticles::setParticleFilter(ParticleFilter* particleFilterIn) {
    particleFilter = particleFilterIn;
}