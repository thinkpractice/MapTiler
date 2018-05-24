#ifndef MAPPEDVECTORFILE_H
#define MAPPEDVECTORFILE_H

#include <string>
#include "VectorFile.h"
#include "AffineTransform.h"

class MappedVectorFile : public VectorFile
{
public:
    MappedVectorFile(string filename, SpatialReference destinationReference, AffineTransform rasterCoordinateTransform);
    virtual ~MappedVectorFile();

    virtual shared_ptr<Layer> LayerFor(OGRLayer* layer);

private:
    SpatialReference _destinationReference;
    AffineTransform _rasterCoordinateTransform;

};

class MappedLayer : public Layer
{
    public:
        MappedLayer(OGRLayer* layer, SpatialReference destinationReference, AffineTransform rasterCoordinateTransform);
        virtual ~MappedLayer();

    protected:
        virtual Feature NextFeature() const;

    private:
        Feature MapFeature(Feature feature) const;

    private:
        SpatialReference _destinationReference;
        AffineTransform _rasterCoordinateTransform;
};

#endif /* MAPPEDVECTORFILE_H */
