#include "MappedVectorFile.h"
#include <iostream>

MappedVectorFile::MappedVectorFile(string filename, SpatialReference destinationReference, AffineTransform rasterCoordinateTransform, VectorFile::OpenMode mode)
                    :   VectorFile(filename, mode),
                        _destinationReference(destinationReference),
                        _rasterCoordinateTransform(rasterCoordinateTransform)
{
}

MappedVectorFile::~MappedVectorFile()
{
}

shared_ptr<Layer> MappedVectorFile::LayerFor(OGRLayer* layer)
{
    return std::make_shared<MappedLayer>(layer, _destinationReference, _rasterCoordinateTransform);
}

MappedLayer::MappedLayer(OGRLayer* layer, SpatialReference destinationReference, AffineTransform rasterCoordinateTransform)
                :   Layer(layer),
                    _destinationReference(destinationReference),
                    _rasterCoordinateTransform(rasterCoordinateTransform)
{
        const SpatialReference projectionReference = ProjectionReference();
        _coordinateTransformation = make_shared<CoordinateTransformation>(projectionReference, _destinationReference);
}

MappedLayer::~MappedLayer()
{
}

std::shared_ptr<Feature> MappedLayer::NextFeature() const
{
    auto feature = Layer::NextFeature();
    if (!feature)
        return nullptr;
    auto mappedGeometry = MapGeometry(ProjectionTransformation(), _rasterCoordinateTransform, feature->GetGeometry());
    feature->SetGeometry(mappedGeometry);
    return feature;
}

std::shared_ptr<Geometry> MappedLayer::MapGeometry(const shared_ptr<CoordinateTransformation> transformation, const std::shared_ptr<Geometry> geometry) const
{
    return transformation->MapGeometry(geometry);
}

std::shared_ptr<Geometry> MappedLayer::MapGeometry(const shared_ptr<CoordinateTransformation> transformation, const AffineTransform affineTransform, const std::shared_ptr<Geometry> geometry) const
{
    auto mappedGeometry = MapGeometry(transformation, geometry);
    return affineTransform.ReverseTransform(mappedGeometry);
}

shared_ptr<CoordinateTransformation> MappedLayer::ProjectionTransformation() const
{
    return _coordinateTransformation;
}
