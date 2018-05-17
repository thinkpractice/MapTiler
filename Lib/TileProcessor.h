#ifndef TILEPROCESSOR_H
#define TILEPROCESSOR_H

#include "GeoMap.h"
#include "TileGrid.h"

class TileProcessor
{
    public:
        TileProcessor(GeoMap* mainRasterMap, int tileWidth = 1024, int tileHeight = 1024);
        TileProcessor(GeoMap* mainRasterMap, const Area& areaToProcess, int tileWidth = 1024, int tileHeight = 1024);
        virtual ~TileProcessor();

        void StartProcessing();
        void AddProcessingStep();

    private:
        GeoMap* _mainRasterMap;
        Area _areaToProcess;
        TileGrid _tileGrid;
};

#endif /* TILEPROCESSOR_H */

