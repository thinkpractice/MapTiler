#ifndef TILE_DOWNLOAD_STEP_H
#define TILE_DOWNLOAD_STEP_H

#include "ProcessingStep.h"
#include "GeoMap.h"
#include <string>

using namespace std;

class TileDownloadStep : public ProcessingStep
{
public:
    TileDownloadStep(string layerName, string layerUrl, int layerIndex, int year);
    TileDownloadStep(string tileName, shared_ptr<GeoMap> map, int year);
	virtual ~TileDownloadStep();
	
	virtual void Run();

public:
	string _tileName;
	shared_ptr<GeoMap> _map;
    int _year;
};

#endif
