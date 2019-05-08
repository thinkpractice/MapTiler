#include "TileFromLocationProducerStep.h"

TileFromLocationProducerStep::TileFromLocationProducerStep(std::string mapLayerUrl, int mapLayerIndex, const Area& area, std::shared_ptr<VectorFile> vectorFile,
                                                           std::string locationLayerName, int locationLayerIndex, int tileWidth, int tileHeight, std::string persistenceUrl)
                                :	TileProducerStep(mapLayerUrl, mapLayerIndex, area, tileWidth, tileHeight, persistenceUrl),
                                    _vectorFile(vectorFile),
                                    _locationLayerName(locationLayerName),
                                    _locationLayerIndex(locationLayerIndex)
{
}

TileFromLocationProducerStep::~TileFromLocationProducerStep()
{
}

int TileFromLocationProducerStep::LayerIndex()
{
    return _locationLayerIndex;
}

std::string TileFromLocationProducerStep::LayerName()
{
    return _locationLayerName;
}

shared_ptr<Layer> TileFromLocationProducerStep::GetLayer()
{
    if (LayerIndex() >= 0)
        return _vectorFile->Layers()[LayerIndex()];
    return _vectorFile->operator[](LayerName().c_str());
}

void TileFromLocationProducerStep::CreateTiles(std::shared_ptr<DatabaseWrapper> databasePersistence)
{
    long long areaId = databasePersistence ? databasePersistence->SaveAreaOfInterest(AreaOfInterest()) : 0;

    auto affineTransform = Map()->MapTransform();

    auto layer = GetLayer();
    layer->SetSpatialFilter(AreaOfInterest());
    int numberOfTiles = 0;
    for (auto it = layer->begin(); it != layer->end(); ++it)
    {
        auto feature = *it;
        auto geometry = feature.GetGeometry();
        if (geometry->IsPoint())
        {
            auto mappedGeometry = affineTransform.ReverseTransform(geometry);
            auto mappedPoint = dynamic_pointer_cast<Point>(mappedGeometry);
            double x = mappedPoint->X - TileWidth() / 2.0;
            double y = mappedPoint->Y - TileHeight() / 2.0;

            Rect tileRect(x, y, TileWidth(), TileHeight());
            CreateTile(databasePersistence, areaId, tileRect, numberOfTiles);
            numberOfTiles++;
        }
    }
}
