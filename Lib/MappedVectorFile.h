#ifndef MAPPEDVECTORFILE_H
#define MAPPEDVECTORFILE_H

#include <string>
#include <memory>
#include "Layer.h"
#include "Feature.h"
#include "AffineTransform.h"
#include "CoordinateTransformation.h"
#include "VectorFile.h"
#include "SpatialReference.h"

class MappedVectorFile : public VectorFile
{
public:
    MappedVectorFile(std::string filename, SpatialReference destinationReference, AffineTransform rasterCoordinateTransform, VectorFile::OpenMode mode = ReadOnly);
    virtual ~MappedVectorFile();

    virtual std::shared_ptr<Layer> LayerFor(OGRLayer* layer);

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
        std::shared_ptr<CoordinateTransformation> ProjectionTransformation() const;

    private:
        std::shared_ptr<CoordinateTransformation> _coordinateTransformation;
        SpatialReference _destinationReference;
        AffineTransform _rasterCoordinateTransform;
};

#endif /* MAPPEDVECTORFILE_H */
