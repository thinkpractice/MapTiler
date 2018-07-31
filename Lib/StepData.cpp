#include "StepData.h"
#include "Utils.h"

int StepData::numberOfTiles;

StepData::StepData(const Rect& boundingRect, const Area& boundingArea)
			:	_uniqueId(Utils::UUID()),
				_boundingRect(boundingRect),
				_boundingArea(boundingArea)
{
}

StepData::~StepData()
{
}

string StepData::UniqueId()
{
	return _uniqueId;
}

void StepData::SetUniqueId(string uniqueId)
{
	_uniqueId = uniqueId;
}

int StepData::NumberOfTiles()
{
	return StepData::numberOfTiles;
}

void StepData::SetNumberOfTiles(int numberOfTiles)
{
	StepData::numberOfTiles = numberOfTiles;
}

Area StepData::BoundingArea()
{
    return _boundingArea;
}

void StepData::SetBoundingRect(const Rect& rect)
{
    _boundingRect = rect;
}

Rect StepData::BoundingRect()
{
	return _boundingRect;
}

void StepData::AddTile(string tileName, shared_ptr< GeoTile > geoTile, int year)
{
    _tiles[tileName] = {year, geoTile};
}

StepData::TileData StepData::GetTile(string tileName)
{
	return _tiles[tileName];
}

map< string, StepData::TileData > StepData::Tiles()
{
	return _tiles;
}

void StepData::AddProcessedTile(string tileName, shared_ptr< GeoTile > geoTile, int year)
{
    _processedTiles[tileName] = {year, geoTile};
}

StepData::TileData StepData::GetProcessedTile(string tileName)
{
	return _processedTiles[tileName];
}

map< string, StepData::TileData > StepData::ProcessedTiles()
{
	return _processedTiles;
}

void StepData::AddMetadataFeatures(string metadataName, const vector<Feature> &features)
{
    _metadataFeatures[metadataName] = features;
}

map<string, vector<Feature>> StepData::MetadataFeatures()
{
    return _metadataFeatures;
}

int StepData::AreaId() const
{
    return _areaId;
}

void StepData::SetAreaId(int areaId)
{
    _areaId = areaId;
}

int StepData::TileId() const
{
    return _tileId;
}

void StepData::SetTileId(int tileId)
{
    _tileId = tileId;
}

vector<Feature> StepData::GetMetadataFeatures(string metadataName)
{
    return _metadataFeatures[metadataName];
}
