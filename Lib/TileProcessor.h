#ifndef TILEPROCESSOR_H
#define TILEPROCESSOR_H

#include <vector>
#include <memory>
#include <thread>
#include <string>
#include <map>
#include "GeoMap.h"
#include "TileGrid.h"
#include "ProcessingPipeline.h"

using namespace std;

class TileProcessor
{
    public:
        TileProcessor(shared_ptr<GeoMap> mainRasterMap, int tileWidth, int tileHeight);
        TileProcessor(shared_ptr<GeoMap> mainRasterMap, const Area& areaToProcess, int tileWidth = 1024, int tileHeight = 1024);
        virtual ~TileProcessor();

        void StartProcessing(string outputDirectory, string polygonFilename);
		void AddRasterMap(string mapName, shared_ptr<GeoMap> map);

    private:
        shared_ptr<GeoMap> _mainRasterMap;
		map<string, shared_ptr<GeoMap>> _rasterMaps;
        TileGrid _tileGrid;
        ProcessingPipeline _pipeline;
};

#endif /* TILEPROCESSOR_H */
