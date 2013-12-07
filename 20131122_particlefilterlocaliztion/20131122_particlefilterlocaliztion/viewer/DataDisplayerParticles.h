#ifndef VIEWER_DATADISPLAYERPARTICLES_H
#define VIEWER_DATADISPLAYERPARTICLES_H

#include "IDataDisplayer.h"

class DataDisplayerParticles : public IDataDisplayer {
public:
    DataDisplayerParticles(void);
    ~DataDisplayerParticles(void);
    void display();
};

#endif