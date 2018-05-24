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

    virtual Layer operator[](const char* layerName);
    virtual vector<Layer>& Layers();

    virtual vector<Feature> ExecuteSql(const char* sqlStatement);

private:
    VectorFile _vectorFile;
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
        SpatialReference _destinationReference;
        AffineTransform _rasterCoordinateTransform;
}

#endif /* MAPPEDVECTORFILE_H */
