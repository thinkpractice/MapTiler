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
private:
    struct TileData
    {
        int year;
        shared_ptr<GeoTile> tile;
    };


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
	
    void AddTile(string tileName, shared_ptr<GeoTile> geoTile, int year);
    TileData GetTile(string tileName);
    map<string, TileData> Tiles();
	
    void AddProcessedTile(string tileName, shared_ptr<GeoTile> geoTile, int year);
    TileData GetProcessedTile(string tileName);
    map<string, TileData> ProcessedTiles();
	

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
    map<string, TileData> _tiles;
    map<string, TileData> _processedTiles;
    map<string, vector<Feature>> _metadataFeatures;

    int _areaId;
    int _tileId;
};

#endif
