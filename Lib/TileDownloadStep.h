#ifndef TILE_DOWNLOAD_STEP_H
#define TILE_DOWNLOAD_STEP_H

#include "ProcessingStep.h"
#include "GeoMap.h"

class TileDownloadStep : public ProcessingStep
{
public:
	TileDownloadStep(GeoMap* map);
	virtual ~TileDownloadStep();
	
	virtual void Run();
	
public:
	GeoMap* _map;
};

#endif