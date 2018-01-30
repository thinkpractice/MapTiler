#include "CoordinateTransformation.h"

SpatialReference CoordinateTransformation::MapCoordinate(SpatialReference other, string epsgCode){
    if (other.hasEpsgCode(epsgCode))
        return other;
    //TODO do coordinate conversion here
}

Area CoordinateTransformation::MapArea(Area other, string epsgCode)
{
    SpatialReference convertedLeftTop = CoordinateTransformation::MapCoordinate(other.LeftTop(), epsgCode);
    SpatialReference convertedBottomRight = CoordinateTransformation::MapCoordinate(other.BottomRight(), epsgCode);

    return Area(convertedLeftTop, convertedBottomRight);
}

