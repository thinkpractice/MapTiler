#ifndef TILEPROCESSOR_H
#define TILEPROCESSOR_H

#include <vector>
#include <memory>
#include <thread>
#include <string>
#include <map>
#include "GeoMap.h"
#include "ProcessingPipeline.h"
#include "MapTilerSettings.h"

using namespace std;

class TileProcessor
{
    public:
        TileProcessor(shared_ptr<GeoMap> mainRasterMap, const Area& areaToProcess, const MapTilerSettings& settings);
        virtual ~TileProcessor();

        void StartProcessing();
		void AddRasterMap(string mapName, shared_ptr<GeoMap> map);

    private:
        shared_ptr<GeoMap> _mainRasterMap;
        Area _areaToProcess;
		map<string, shared_ptr<GeoMap>> _rasterMaps;
        ProcessingPipeline _pipeline;
        MapTilerSettings _settings;
};

#endif /* TILEPROCESSOR_H */
