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
    oldSampleVec = particleFilter->getOldSampleVec();
    newSampleVec = particleFilter->getNewSampleVec();

    double sizeHalf = 0.004;
    double x;
    double y;

    // Draw new sample.
    for (unsigned int i = 0; i < newSampleVec->size(); i++) {
        // Calculate window position.
        viewer->projectAPointToWindow((*newSampleVec)[i]->position[0],
                                      (*newSampleVec)[i]->position[1],
                                      (*newSampleVec)[i]->position[2],
                                      1, -1,
                                      1, -1,
                                      &x, &y);


        // Draw.
        glBegin(GL_QUADS);
            glColor3f(0.75f, 0.0f, 1.0f); // Electric Purple
            glVertex2f(x - sizeHalf, y - sizeHalf);
            glVertex2f(x + sizeHalf, y - sizeHalf);
            glVertex2f(x + sizeHalf, y + sizeHalf);
            glVertex2f(x - sizeHalf, y + sizeHalf);
        glEnd();
    }

    // Draw old sample.
    for (unsigned int i = 0; i < oldSampleVec->size(); i++) {
        // If is bad particle, don't draw it.
        if ((*oldSampleVec)[i]->weight == 0)
            continue;

        // Calculate window position.
        viewer->projectAPointToWindow((*oldSampleVec)[i]->position[0],
                                      (*oldSampleVec)[i]->position[1],
                                      (*oldSampleVec)[i]->position[2],
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