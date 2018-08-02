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

std::string StepData::UniqueId()
{
	return _uniqueId;
}

void StepData::SetUniqueId(std::string uniqueId)
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

void StepData::AddTile(std::string tileName, std::shared_ptr< GeoTile > geoTile, int year)
{
    _tiles[tileName] = {year, geoTile};
}

StepData::TileData StepData::GetTile(std::string tileName)
{
	return _tiles[tileName];
}

std::map< std::string, StepData::TileData > StepData::Tiles()
{
	return _tiles;
}

void StepData::AddProcessedTile(std::string tileName, std::shared_ptr< GeoTile > geoTile, int year)
{
    _processedTiles[tileName] = {year, geoTile};
}

StepData::TileData StepData::GetProcessedTile(std::string tileName)
{
	return _processedTiles[tileName];
}

std::map< std::string, StepData::TileData > StepData::ProcessedTiles()
{
	return _processedTiles;
}

void StepData::AddMetadataFeatures(std::string metadataName, const std::vector<Feature> &features)
{
    _metadataFeatures[metadataName] = features;
}

std::map<std::string, std::vector<Feature>> StepData::MetadataFeatures()
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

std::vector<Feature> StepData::GetMetadataFeatures(std::string metadataName)
{
    return _metadataFeatures[metadataName];
}
