#ifndef GEOTILE_H
#define GEOTILE_H

#include "Rect.h"
#include "Area.h"

class GeoTile
{
    public:
        GeoTile(const Rect& rectangle, const Area& area, int numberOfLayers);
        virtual ~GeoTile();

        size_t NumberOfBytes();
        int NumberOfLayers();
        Rect BoundingRect();
        Area BoundingArea();

        unsigned char* Data();

    private:
        Area _area;
        Rect _rect;
        int _numberOfLayers;
        unsigned char* _data;
};

#endif
