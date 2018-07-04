#include "TileDownloadStep.h"

TileDownloadStep::TileDownloadStep(GeoMap* map)
					:	ProcessingStep(PreProcessing),
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
			
			stepData->SetTile(tile);
			OutQueue()->enqueue(stepData);
			numberOfTilesDownloaded++;
			
			if (numberOfTilesDownloaded % 100 == 0)
				cout << "Downloaded " << to_string(numberOfTilesDownloaded) << " out of " << to_string(stepData->NumberOfTiles()) << endl;
		}
		catch (...)
		{
		}
	}
	//Notify process is ended
	OutQueue()->enqueue(nullptr);
}

