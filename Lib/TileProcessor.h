#ifndef TILEPROCESSOR_H
#define TILEPROCESSOR_H

#include <vector>
#include <memory>
#include <thread>
#include <string>
#include "GeoMap.h"
#include "TileGrid.h"
#include "ProcessingPipeline.h"

using namespace std;

class TileProcessor
{
    public:
        TileProcessor(GeoMap* mainRasterMap, int tileWidth = 1024, int tileHeight = 1024);
        TileProcessor(GeoMap* mainRasterMap, const Area& areaToProcess, int tileWidth = 1024, int tileHeight = 1024);
        virtual ~TileProcessor();

        void StartProcessing(string outputDirectory, string polygonFilename);

    private:
        GeoMap* _mainRasterMap;
        TileGrid _tileGrid;
        ProcessingPipeline _pipeline;
};

#endif /* TILEPROCESSOR_H */

