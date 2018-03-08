#include "Area.h"

Area::Area(const SpatialReference& leftTop, const SpatialReference& bottomRight)
        :   _leftTop(leftTop),
            _bottomRight(bottomRight)
{
}

SpatialReference Area::LeftTop()
{
    return _leftTop;
}

SpatialReference Area::BottomRight()
{
    return _bottomRight;
}

