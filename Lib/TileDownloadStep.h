#ifndef TILE_DOWNLOAD_STEP_H
#define TILE_DOWNLOAD_STEP_H

#include "ProcessingStep.h"
#include "GeoMap.h"
#include <string>

using namespace std;

class TileDownloadStep : public ProcessingStep
{
public:
	TileDownloadStep(string tileName, GeoMap* map);
	virtual ~TileDownloadStep();
	
	virtual void Run();
	
public:
	string _tileName;
	GeoMap* _map;
};

#endif