#ifndef LAYER_H
#define LAYER_H

#include <string>
#include "ogrsf_frmts.h"
#include "SpatialReference.h"

using namespace std;

class Layer
{
    public:
        Layer(OGRLayer* layer);
        virtual ~Layer();

        string Name();
        SpatialReference ProjectionReference();

    protected:
        OGRFeatureDefn* FeatureDefinition();

    private:
        OGRLayer* _layer;
};

#endif
