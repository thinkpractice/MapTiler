#include "StepData.h"

StepData::StepData()
{
}

StepData::~StepData()
{
}

int StepData::NumberOfTiles() const
{
	return _numberOfTiles;
}

void StepData::SetNumberOfTiles(int numberOfTiles)
{
	_numberOfTiles = numberOfTiles;
}

void StepData::SetBoundingRect(const Rect& rect)
{
	_boundingRect = rect;
}

Rect StepData::BoundingRect()
{
	return _boundingRect;
}

void StepData::SetTile(shared_ptr< GeoTile > geoTile)
{
	_geoTile = geoTile;
}

shared_ptr<GeoTile> StepData::Tile()
{
	return _geoTile;
}

void StepData::SetMaskTile(std::shared_ptr< GeoTile > maskTile)
{
	_maskTile = maskTile;
}

std::shared_ptr< GeoTile > StepData::MaskTile()
{
	return _maskTile;
}

void StepData::SetMaskedTile(std::shared_ptr< GeoTile > maskedTile)
{
	_maskedTile = maskedTile;
}

std::shared_ptr< GeoTile > StepData::MaskedTile()
{
	return _maskedTile;
}






