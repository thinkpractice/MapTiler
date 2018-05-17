#ifndef TILEPROCESSOR_H
#define TILEPROCESSOR_H

#include <vector>
#include <memory>
#include "GeoMap.h"
#include "TileGrid.h"
#include "ProcessingStep.h"

using namespace std;

class TileProcessor
{
    public:
        TileProcessor(GeoMap* mainRasterMap, int tileWidth = 1024, int tileHeight = 1024);
        TileProcessor(GeoMap* mainRasterMap, const Area& areaToProcess, int tileWidth = 1024, int tileHeight = 1024);
        virtual ~TileProcessor();

        void StartProcessing();
        void AddProcessingStep(ProcessingStep* step);

    private:
        GeoMap* _mainRasterMap;
        Area _areaToProcess;
        TileGrid _tileGrid;

        vector< shared_ptr<ProcessingStep> > _preProcessingSteps;
        vector< shared_ptr<ProcessingStep> > _postProcessingSteps;
};

#endif /* TILEPROCESSOR_H */

