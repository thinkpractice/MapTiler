#ifndef GEOTILE_H
#define GEOTILE_H

#include "Rect.h"
#include "Area.h"

class GeoTile
{
    public:
        GeoTile(const Rect& rectangle, const Area& area, int numberOfLayers);
        virtual ~GeoTile();

        string UniqueId();
        void SetUniqueId(string uniqueId);
        int NumberOfBytes();
        int NumberOfLayers();
        Rect BoundingRect();
        Area BoundingArea();

        unsigned char* Data();
        void SetRasterData(unsigned char** rasterData);

    private:
        Area _area;
        Rect _rect;
        string _uniqueId;
        int _numberOfLayers;
        unsigned char* _data;
};

#endif
