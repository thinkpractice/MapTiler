#ifndef STEP_DATA_H
#define STEP_DATA_H

#include "Rect.h"
#include "GeoTile.h"
#include <memory>

using namespace std;

class StepData
{
public:
	StepData();
	virtual ~StepData();
	
	int NumberOfTiles() const;
	void SetNumberOfTiles(int numberOfTiles);
	
	void SetBoundingRect(const Rect& rect);
	Rect BoundingRect();
	
	void SetTile(shared_ptr<GeoTile> geoTile);
	shared_ptr<GeoTile> Tile();
	
	void SetMaskTile(shared_ptr<GeoTile> maskTile);
	shared_ptr<GeoTile> MaskTile();
	
	void SetMaskedTile(shared_ptr<GeoTile> maskedTile);
	shared_ptr<GeoTile> MaskedTile();
	
private:
	int _numberOfTiles;
	Rect _boundingRect;
	shared_ptr<GeoTile> _geoTile;
	shared_ptr<GeoTile> _maskTile;
	shared_ptr<GeoTile> _maskedTile;
	
};

#endif