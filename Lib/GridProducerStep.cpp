#include "GridProducerStep.h"

GridProducerStep::GridProducerStep(std::string layerUrl, int layerIndex, const Area& area, int tileWidth, int tileHeight, std::string persistenceUrl)
                    :	TileProducerStep (layerUrl, layerIndex, area, tileWidth, tileHeight, persistenceUrl),
                        _tileGrid(RectToProcess(), area, tileWidth, tileHeight)
{
}

GridProducerStep::~GridProducerStep()
{
}

void GridProducerStep::CreateTiles(std::shared_ptr<DatabaseWrapper> databasePersistence)
{
    std::cout << "Creating new tiles for area: " << _tileGrid.GridArea().Description() << endl;
    long long areaId = databasePersistence ? databasePersistence->SaveAreaOfInterest(_tileGrid.GridArea()) : 0;
    for (auto& tileRect : _tileGrid)
    {
        CreateTile(databasePersistence, areaId, tileRect, _tileGrid.NumberOfTiles());
    }
}

void GridProducerStep::Run()
{
    std::cout << "Tile width: " << _tileGrid.TileWidth() << std::endl;
    std::cout << "Tile height: " << _tileGrid.TileHeight() << std::endl;
    cout << "TileGrid dimensions=(" << _tileGrid.WidthInTiles() << "," << _tileGrid.HeightInTiles() << ")" << endl;
    cout << "Pixel dimensions" << _tileGrid.PixelDimensions().Left() << "," << _tileGrid.PixelDimensions().Top() << "," << _tileGrid.PixelDimensions().Width() << "," << _tileGrid.PixelDimensions().Height() << endl;
    cout << "Tile Dimensions=(" << _tileGrid.TileWidth() << "," << _tileGrid.TileHeight() << ")" << endl;
    TileProducerStep::Run();
}
