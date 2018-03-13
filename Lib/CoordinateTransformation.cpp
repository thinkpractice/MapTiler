#include "CoordinateTransformation.h"
#include <ogr_spatialref.h>
#include <iostream>

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

    cout << "SourceCoordinate("<< sourceCoordinate.X << "," << sourceCoordinate.Y << "), MapCoordinate(" << x << "," << y << ")";
    return Point(x, y);
}


Area CoordinateTransformation::MapArea(Area other, string epsgCode)
{
    SpatialReference destinationReference(epsgCode.c_str());
    return MapArea(other, destinationReference);
}

Area CoordinateTransformation::MapArea(Area other, SpatialReference destinationReference)
{
    SpatialReference sourceReference = other.ProjectionReference();

    Point convertedLeftTop = CoordinateTransformation::MapCoordinate(sourceReference, destinationReference, other.LeftTop());
    Point convertedBottomRight = CoordinateTransformation::MapCoordinate(sourceReference, destinationReference, other.BottomRight());

    return Area(destinationReference, convertedLeftTop, convertedBottomRight);
}

