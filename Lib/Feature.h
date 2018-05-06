#ifndef FEATURE_H
#define FEATURE_H

#include "ogrsf_frmts.h"

class Feature
{
    public:
        Feature(OGRFeature *feature);
        virtual ~Feature();

    private:
        OGRFeature* _feature;

};

#endif
