#include "MappedVectorFile.h"
#include <iostream>

using namespace std;

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
                    _destinationReference(destinationReference),
                    _rasterCoordinateTransform(rasterCoordinateTransform)
{
        const SpatialReference projectionReference = ProjectionReference();
        _coordinateTransformation = make_shared<CoordinateTransformation>(projectionReference, _destinationReference);
}

MappedLayer::~MappedLayer()
{
}

Feature MappedLayer::NextFeature() const
{
    cout << "MappedLayer::NextFeature" << endl;
    Feature feature = Layer::NextFeature();
    feature.Geometry().MapGeometry(ProjectionTransformation(), _rasterCoordinateTransform);
    return feature;
}

shared_ptr<CoordinateTransformation> MappedLayer::ProjectionTransformation() const
{
    return _coordinateTransformation;
}
