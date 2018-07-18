#include "TileFilterStep.h"
#include "Rect.h"
#include "StepData.h"

TileFilterStep::TileFilterStep()
                    :	ProcessingStep(Processing)
{
}

TileFilterStep::~TileFilterStep()
{
}

void TileFilterStep::Run()
{
	while (auto stepData = InQueue()->dequeue())
	{
		Rect boundingRect = stepData->BoundingRect();
		vector<Feature> features = stepData->GetMetadataFeatures("polygons");
		if (features.size() > 0)
		{
			OutQueue()->enqueue(stepData);
		}
		else
		{
			StepData::SetNumberOfTiles(StepData::NumberOfTiles() - 1);
		}
	}
}
