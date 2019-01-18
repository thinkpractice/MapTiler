#ifndef TILE_DOWNLOAD_STEP_H
#define TILE_DOWNLOAD_STEP_H

#include "ProcessingStep.h"
#include "GeoMap.h"
#include <string>
#include <vector>
#include <memory>

class TileDownloadStep : public ProcessingStep
{
public:
    TileDownloadStep(std::string layerName, std::string layerUrl, int layerIndex, int year, std::vector<std::string> driverOptions);
    TileDownloadStep(std::string tileName, std::shared_ptr<GeoMap> map, int year);
	virtual ~TileDownloadStep();
	
	virtual void Run();

public:
    std::string _tileName;
    std::shared_ptr<GeoMap> _map;
    int _year;
};

#endif
