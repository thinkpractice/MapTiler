#include "Area.h"

Area::Area(SpatialReference& leftTop, SpatialReference& bottomRight)
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

