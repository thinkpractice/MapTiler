#include "StepData.h"
#include "Utils.h"

StepData::StepData()
			:	_uniqueId(Utils::UUID())
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

int StepData::NumberOfTiles() const
{
	return _numberOfTiles;
}

void StepData::SetNumberOfTiles(int numberOfTiles)
{
	_numberOfTiles = numberOfTiles;
}

Area StepData::BoundingArea()
{
    //TODO find out how to refactor this, undefined if Tiles() is empty.
    auto geoTile = Tiles().begin()->second;
    return geoTile->BoundingArea();
}

void StepData::SetBoundingRect(const Rect& rect)
{
    _boundingRect = rect;
}

Rect StepData::BoundingRect()
{
	return _boundingRect;
}

void StepData::AddTile(string tileName, shared_ptr< GeoTile > geoTile)
{
	_tiles[tileName] = geoTile;
}

shared_ptr< GeoTile > StepData::GetTile(string tileName)
{
	return _tiles[tileName];
}

map< string, shared_ptr< GeoTile > > StepData::Tiles()
{
	return _tiles;
}

void StepData::AddProcessedTile(string tileName, shared_ptr< GeoTile > geoTile)
{
	_processedTiles[tileName] = geoTile;
}

shared_ptr< GeoTile > StepData::GetProcessedTile(string tileName)
{
	return _processedTiles[tileName];
}

map< string, shared_ptr< GeoTile > > StepData::ProcessedTiles()
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

vector<Feature> StepData::GetMetadataFeatures(string metadataName)
{
    return _metadataFeatures[metadataName];
}
