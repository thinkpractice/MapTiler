#ifndef TILE_FILTER_STEP_H
#define TILE_FILTER_STEP_H

#include "ProcessingStep.h"

class TileFilterStep : public ProcessingStep
{
public:
    TileFilterStep(std::string layerName);
	virtual ~TileFilterStep();
	
	void Run();
	
private:
    std::string _layerName;
};

#endif
