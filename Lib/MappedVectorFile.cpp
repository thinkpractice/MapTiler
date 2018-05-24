#include "MappedVectorFile.h"

MappedVectorFile::MappedVectorFile(string filename, SpatialReference destinationReference, AffineTransform rasterCoordinateTransform)
                    :   VectorFile(filename),
                        _destinationReference(destinationReference),
                        _rasterCoordinateTransform(rasterCoordinateTransform)
{
}

MappedVectorFile::~MappedVectorFile()
{
}

shared_ptr<Layer> MappedVectorFile::LayerFor(OGRLayer* layer)
{
    return make_shared<MappedLayer>(layer, _destinationReference, _rasterCoordinateTransform);
}

MappedLayer::MappedLayer(OGRLayer* layer, SpatialReference destinationReference, AffineTransform rasterCoordinateTransform)
                :   Layer(layer),
                    _coordinateTransformation(nullptr),
                    _destinationReference(destinationReference),
                    _rasterCoordinateTransform(rasterCoordinateTransform)
{
}

MappedLayer::~MappedLayer()
{
}

Feature MappedLayer::NextFeature() const
{
    Feature feature = Layer::NextFeature();
    return MapFeature(feature);
}

Feature MappedLayer::MapFeature(Feature feature) const
{

}

shared_ptr<CoordinateTransformation> MappedLayer::ProjectionTransformation()
{
    if (!_coordinateTransformation.get())
    {
        _coordinateTransformation = make_shared<CoordinateTransformation>(ProjectionReference(), _destinationReference);
    }
    return _coordinateTransformation;
}
