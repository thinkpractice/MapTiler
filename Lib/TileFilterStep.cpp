#include "TileFilterStep.h"
#include "Rect.h"
#include "StepData.h"

TileFilterStep::TileFilterStep(std::string layerName)
                    :	ProcessingStep(Processing),
                        _layerName(layerName)
{
}

TileFilterStep::~TileFilterStep()
{
}

void TileFilterStep::Run()
{
	while (auto stepData = InQueue()->dequeue())
	{
        vector<Feature> features = stepData->GetMetadataFeatures(_layerName);
		if (features.size() > 0)
		{
			OutQueue()->enqueue(stepData);
		}
		else
		{
			StepData::SetNumberOfTiles(StepData::NumberOfTiles() - 1);
		}
	}
    DoneProcessing();
}
