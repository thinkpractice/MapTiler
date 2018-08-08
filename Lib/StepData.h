#ifndef STEP_DATA_H
#define STEP_DATA_H

#include "Rect.h"
#include "Area.h"
#include "GeoTile.h"
#include "Feature.h"
#include <memory>
#include <map>
#include <string>
#include <vector>

class StepData
{
private:
    struct TileData
    {
        int year;
        std::shared_ptr<GeoTile> tile;
    };

public:
	StepData(const Rect& boundingRect, const Area& boundingArea);
	virtual ~StepData();
	
    std::string UniqueId();
    void SetUniqueId(std::string uniqueId);
	
	static int NumberOfTiles();
	static void SetNumberOfTiles(int numberOfTiles);
	
    Area BoundingArea();

	void SetBoundingRect(const Rect& rect);
	Rect BoundingRect();
	
    void AddTile(std::string tileName, std::shared_ptr<GeoTile> geoTile, int year);
    TileData GetTile(std::string tileName);
    std::map<std::string, TileData> Tiles();
	
    void AddProcessedTile(std::string tileName, std::shared_ptr<GeoTile> geoTile, int year);
    TileData GetProcessedTile(std::string tileName);
    std::map<std::string, TileData> ProcessedTiles();
	

    void AddMetadataFeatures(std::string metadataName, const std::vector<Feature>& features);
    std::vector<Feature> GetMetadataFeatures(std::string metadataName);
    std::map<std::string, std::vector<Feature>> MetadataFeatures();

    long long AreaId() const;
    void SetAreaId(long long AreaId);

    long long TileId() const;
    void SetTileId(long long TileId);

private:
    std::string _uniqueId;
    static int numberOfTiles;
    Rect _boundingRect;
	Area _boundingArea;
    std::map<std::string, TileData> _tiles;
    std::map<std::string, TileData> _processedTiles;
    std::map<std::string, std::vector<Feature>> _metadataFeatures;

    long long _areaId;
    long long _tileId;
};

#endif
