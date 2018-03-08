#include "CoordinateTransformation.h"
#include "Point.h"
#include <ogr_spatialref.h>

SpatialReference CoordinateTransformation::MapCoordinate(SpatialReference other, string epsgCode)
{
    SpatialReference epsgReference(epsgCode.c_str());
    if (other.IsSame(epsgReference))
        return other;

    OGRSpatialReference sourceReference = other.InnerReference();
    OGRSpatialReference targetReference = epsgReference.InnerReference();
    OGRCoordinateTransformation *transformation = OGRCreateCoordinateTransformation(&sourceReference, &targetReference);

    Point sourceCoordinate = other.Coordinate();
    double x = sourceCoordinate.X;
    double y = sourceCoordinate.Y;
    transformation->Transform(1, &x, &y);

    epsgReference.SetCoordinate(Point(x, y));
    return epsgReference;
}

Area CoordinateTransformation::MapArea(Area other, string epsgCode)
{
    SpatialReference convertedLeftTop = CoordinateTransformation::MapCoordinate(other.LeftTop(), epsgCode);
    SpatialReference convertedBottomRight = CoordinateTransformation::MapCoordinate(other.BottomRight(), epsgCode);

    return Area(convertedLeftTop, convertedBottomRight);
}

