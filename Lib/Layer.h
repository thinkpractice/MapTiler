#ifndef LAYER_H
#define LAYER_H

#include <string>
#include "ogrsf_frmts.h"
#include "SpatialReference.h"
#include "Feature.h"
#include "FeatureIterator.h"

using namespace std;

class Layer
{
    public:
        Layer(OGRLayer* layer);
        virtual ~Layer();

        string Name();
        SpatialReference ProjectionReference();

        void ResetReading();
        Feature* NextFeature();

        using iterator = FeatureIterator;

        iterator begin() const;
        iterator end() const;

    protected:
        OGRFeatureDefn* FeatureDefinition();

    private:
        OGRLayer* _layer;
};

#endif
