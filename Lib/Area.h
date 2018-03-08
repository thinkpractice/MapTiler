#ifndef AREA_H
#define AREA_H

#include "SpatialReference.h"

class Area
{
    public:
        Area(const SpatialReference& leftTop, const SpatialReference& bottomRight);

        SpatialReference LeftTop();
        SpatialReference BottomRight();

    private:
        SpatialReference _leftTop;
        SpatialReference _bottomRight;

};

#endif
