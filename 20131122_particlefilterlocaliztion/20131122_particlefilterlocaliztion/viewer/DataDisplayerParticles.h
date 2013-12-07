#ifndef VIEWER_DATADISPLAYERPARTICLES_H
#define VIEWER_DATADISPLAYERPARTICLES_H

#include "IDataDisplayer.h"
#include "../particlefilter/ParticleFilter.h"

class DataDisplayerParticles : public IDataDisplayer {
public:
    DataDisplayerParticles(void);
    ~DataDisplayerParticles(void);
    void display();

    void setParticleFilter(ParticleFilter* particleFilterIn);

private:
    ParticleFilter* particleFilter;
    vector<ParticleType*>* sampleVec;
};

#endif