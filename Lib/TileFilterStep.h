#ifndef TILE_FILTER_STEP_H
#define TILE_FILTER_STEP_H

#include "ProcessingStep.h"

class TileFilterStep : public ProcessingStep
{
public:
	TileFilterStep();
	virtual ~TileFilterStep();
	
	void Run();
	
};

#endif