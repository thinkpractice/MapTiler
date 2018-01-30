#ifndef GEOTILE_H
#define GEOTILE_H

#include "Area.h"

class GeoTile
{
    public:
        GeoTile();
        virtual ~GeoTile();

    private:
        Area _area;

};

#endif
