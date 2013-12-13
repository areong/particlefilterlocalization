#include "DataDisplayerParticles.h"
#include <glut.h>
#include "Viewer.h"

DataDisplayerParticles::DataDisplayerParticles(void) {
}

DataDisplayerParticles::~DataDisplayerParticles(void) {
}

void DataDisplayerParticles::setViewer(Viewer *viewerIn) {
    viewer = viewerIn;
}

void DataDisplayerParticles::display() {
    // Get sample vector.
    sampleVec = particleFilter->getNewSampleVec();

    double sizeHalf = 0.004;
    double x;
    double y;

    // Draw each sample.
    for (unsigned int i = 0; i < sampleVec->size(); i++) {
        // Calculate window position.
        viewer->projectAPointToWindow((*sampleVec)[i]->position[0],
                                      (*sampleVec)[i]->position[1],
                                      (*sampleVec)[i]->position[2],
                                      1, -1,
                                      1, -1,
                                      &x, &y);


        // Draw.
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