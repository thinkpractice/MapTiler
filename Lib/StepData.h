#ifndef STEP_DATA_H
#define STEP_DATA_H

#include "Rect.h"
#include "Area.h"
#include "GeoTile.h"
#include "Feature.h"
#include <memory>
#include <map>
#include <string>

using namespace std;

class StepData
{
public:
	StepData(const Rect& boundingRect, const Area& boundingArea);
	virtual ~StepData();
	
	string UniqueId();
	void SetUniqueId(string uniqueId);
	
	static int NumberOfTiles();
	static void SetNumberOfTiles(int numberOfTiles);
	
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

    int AreaId() const;
    void SetAreaId(int AreaId);

    int TileId() const;
    void SetTileId(int TileId);

private:
    string _uniqueId;
    static int numberOfTiles;
    Rect _boundingRect;
	Area _boundingArea;
	map<string, shared_ptr<GeoTile>> _tiles;
	map<string, shared_ptr<GeoTile>> _processedTiles;
    map<string, vector<Feature>> _metadataFeatures;

    int _areaId;
    int _tileId;
};

#endif
