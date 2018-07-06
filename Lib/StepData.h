#ifndef STEP_DATA_H
#define STEP_DATA_H

#include "Rect.h"
#include "GeoTile.h"
#include "Feature.h"
#include <memory>
#include <map>
#include <string>

using namespace std;

class StepData
{
public:
	StepData();
	virtual ~StepData();
	
	string UniqueId();
	void SetUniqueId(string uniqueId);
	
	int NumberOfTiles() const;
	void SetNumberOfTiles(int numberOfTiles);
	
    Area BoundingArea();

	void SetBoundingRect(const Rect& rect);
	Rect BoundingRect();
	
	void AddTile(string tileName, shared_ptr<GeoTile> geoTile);
	shared_ptr<GeoTile> GetTile(string tileName);
	map<string, shared_ptr<GeoTile>> Tiles();
	
	void AddProcessedTile(string tileName, shared_ptr<GeoTile> geoTile);
	shared_ptr<GeoTile> GetProcessedTile(string tileName);
	map<string, shared_ptr<GeoTile>> ProcessedTiles();
	

    void AddMetadataFeatures(string metadataName, const vector<Feature>& features);
    vector<Feature> GetMetadataFeatures(string metadataName);
    map<string, vector<Feature>> MetadataFeatures();

private:
	string _uniqueId;
	int _numberOfTiles;
    Rect _boundingRect;
	map<string, shared_ptr<GeoTile>> _tiles;
	map<string, shared_ptr<GeoTile>> _processedTiles;
    map<string, vector<Feature>> _metadataFeatures;
};

#endif
