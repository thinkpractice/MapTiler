#include "TileDownloadStep.h"
#include "StepData.h"
#include "GeoMapProvider.h"
#include "Utils.h"
#include <chrono>
#include <thread>

TileDownloadStep::TileDownloadStep(string layerName, string layerUrl, int layerIndex, int year)
                    :	TileDownloadStep(layerName, Utils::LoadRasterMap(layerUrl, layerIndex), year)
{
}

//TODO remove later
TileDownloadStep::TileDownloadStep(string tileName, shared_ptr<GeoMap> map, int year)
                    :	ProcessingStep(Source),
						_tileName(tileName),
                        _map(map),
                        _year(year)
{
}

TileDownloadStep::~TileDownloadStep()
{
}

void TileDownloadStep::Run()
{
	int numberOfTilesDownloaded = 0;
    while (auto stepData = InQueue()->dequeue())
	{
		try
		{
			auto tile = shared_ptr<GeoTile>(_map->GetTileForRect(stepData->BoundingRect()));
			stepData->AddTile(_tileName, tile);

			OutQueue()->enqueue(stepData);
			numberOfTilesDownloaded++;
			
			if (numberOfTilesDownloaded % 100 == 0)
				cout << "Downloaded " << to_string(numberOfTilesDownloaded) << " out of " << to_string(StepData::NumberOfTiles()) << endl;
			
			if (numberOfTilesDownloaded > 0 && numberOfTilesDownloaded % 100 == 0)
			{
				cout << "Download of " << _tileName << " is sleeping for a while" << endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			}
		}
		catch (...)
		{
		}
	}
    DoneProcessing();
}
