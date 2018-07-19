#ifndef TILE_DOWNLOAD_STEP_H
#define TILE_DOWNLOAD_STEP_H

#include "ProcessingStep.h"
#include "GeoMap.h"
#include <string>

using namespace std;

class TileDownloadStep : public ProcessingStep
{
public:
    TileDownloadStep(string layerName, string layerUrl, int layerIndex);
    TileDownloadStep(string tileName, shared_ptr<GeoMap> map);
	virtual ~TileDownloadStep();
	
	virtual void Run();

public:
	string _tileName;
	shared_ptr<GeoMap> _map;
};

#endif
