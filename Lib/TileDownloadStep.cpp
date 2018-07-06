#include "TileDownloadStep.h"

TileDownloadStep::TileDownloadStep(string tileName, shared_ptr<GeoMap> map)
					:	ProcessingStep(PreProcessing),
						_tileName(tileName),
						_map(map)
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
				cout << "Downloaded " << to_string(numberOfTilesDownloaded) << " out of " << to_string(stepData->NumberOfTiles()) << endl;
		}
		catch (...)
		{
		}
	}
    DoneProcessing();
}

