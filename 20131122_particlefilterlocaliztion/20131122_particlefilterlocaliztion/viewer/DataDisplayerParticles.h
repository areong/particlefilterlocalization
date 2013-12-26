#ifndef VIEWER_DATADISPLAYERPARTICLES_H
#define VIEWER_DATADISPLAYERPARTICLES_H

#include "IDataDisplayer.h"
#include "../particlefilter/ParticleFilter.h"

class DataDisplayerParticles : public IDataDisplayer {
public:
    DataDisplayerParticles(void);
    ~DataDisplayerParticles(void);
    
    void setViewer(Viewer *viewer);
    void display();
    void onKeyPressed(int key);

    void setParticleFilter(ParticleFilter* particleFilterIn);

private:    
    Viewer *viewer;
    ParticleFilter* particleFilter;
    vector<ParticleType*>* oldSampleVec;
    vector<ParticleType*>* newSampleVec;
    ParticleType *meanOldGoodParticle;
};

#endif