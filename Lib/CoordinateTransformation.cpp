#include "CoordinateTransformation.h"
#include <ogr_spatialref.h>

Point CoordinateTransformation::MapCoordinate(SpatialReference sourceReference,
        SpatialReference targetReference, 
        Point sourceCoordinate)
{
    if (sourceReference.IsSame(targetReference))
        return sourceCoordinate;

    OGRSpatialReference sourceInnerReference = sourceReference.InnerReference();
    OGRSpatialReference targetInnerReference = targetReference.InnerReference();
    OGRCoordinateTransformation *transformation = OGRCreateCoordinateTransformation(&sourceInnerReference, &targetInnerReference);

    double x = sourceCoordinate.X;
    double y = sourceCoordinate.Y;
    transformation->Transform(1, &x, &y);

    return Point(x, y);
}

Area CoordinateTransformation::MapArea(Area other, string epsgCode)
{
    SpatialReference sourceReference = other.ProjectionReference();
    SpatialReference targetReference(epsgCode.c_str());

    Point convertedLeftTop = CoordinateTransformation::MapCoordinate(sourceReference, targetReference, other.LeftTop());
    Point convertedBottomRight = CoordinateTransformation::MapCoordinate(sourceReference, targetReference, other.BottomRight());

    return Area(epsgReference, convertedLeftTop, convertedBottomRight);
}

