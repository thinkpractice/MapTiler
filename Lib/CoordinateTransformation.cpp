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

    return Point(x, y);
}

vector<Point> CoordinateTransformation::MapCoordinates(SpatialReference sourceReference, SpatialReference targetReference, vector<Point> sourceCoordinates)
{
    if (sourceReference.IsSame(targetReference))
        return sourceCoordinates;

    
    double xCoordinates[sourceCoordinates.size()];
    double yCoordinates[sourceCoordinates.size()];
    for (size_t i = 0; i < sourceCoordinates.size(); i++)
    {
        xCoordinates[i] = sourceCoordinates[i].X;
        yCoordinates[i] = sourceCoordinates[i].Y;
    }

    OGRSpatialReference sourceInnerReference = sourceReference.InnerReference();
    OGRSpatialReference targetInnerReference = targetReference.InnerReference();
    OGRCoordinateTransformation *transformation = OGRCreateCoordinateTransformation(&sourceInnerReference, &targetInnerReference);
    transformation->Transform(sourceCoordinates.size(), xCoordinates, yCoordinates); 

    vector<Point> destinationCoordinates;
    for (size_t i = 0; i < sourceCoordinates.size(); i++)
    {
        destinationCoordinates.push_back(Point(xCoordinates[i], yCoordinates[i]));
    }
    return destinationCoordinates;
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

