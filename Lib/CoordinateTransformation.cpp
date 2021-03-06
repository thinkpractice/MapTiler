#include "CoordinateTransformation.h"
#include <iostream>

CoordinateTransformation::CoordinateTransformation(const SpatialReference& sourceReference, const SpatialReference& destinationReference)
                            :   _sourceReference(sourceReference),
                                _destinationReference(destinationReference),
                                _transformation(nullptr)
{
}

CoordinateTransformation::~CoordinateTransformation()
{
    OGRCoordinateTransformation::DestroyCT(_transformation);
}

Point CoordinateTransformation::MapCoordinate(Point sourceCoordinate)
{
    if (_sourceReference.IsSame(_destinationReference))
        return sourceCoordinate;

    double x = sourceCoordinate.X;
    double y = sourceCoordinate.Y;
    Transformation()->Transform(1, &x, &y);

    Point point(x, y);
    point.SetSpatialReference(_destinationReference);
    return point;
}

vector<Point> CoordinateTransformation::MapCoordinates(vector<Point> sourceCoordinates)
{
    if (_sourceReference.IsSame(_destinationReference))
        return sourceCoordinates;
    
    double xCoordinates[sourceCoordinates.size()];
    double yCoordinates[sourceCoordinates.size()];
    for (size_t i = 0; i < sourceCoordinates.size(); i++)
    {
        xCoordinates[i] = sourceCoordinates[i].X;
        yCoordinates[i] = sourceCoordinates[i].Y;
    }

    Transformation()->Transform(sourceCoordinates.size(), xCoordinates, yCoordinates);

    vector<Point> destinationCoordinates;
    for (size_t i = 0; i < sourceCoordinates.size(); i++)
    {
        destinationCoordinates.push_back(Point(xCoordinates[i], yCoordinates[i]));
    }
    return destinationCoordinates;
}

std::vector<std::tuple<double, double> > CoordinateTransformation::MapCoordinates(std::vector<std::tuple<double, double> > sourceCoordinates)
{
    if (_sourceReference.IsSame(_destinationReference))
        return sourceCoordinates;

    double xCoordinates[sourceCoordinates.size()];
    double yCoordinates[sourceCoordinates.size()];
    for (size_t i = 0; i < sourceCoordinates.size(); i++)
    {
        Point p = sourceCoordinates[i];
        xCoordinates[i] = p.X;
        yCoordinates[i] = p.Y;
    }

    Transformation()->Transform(sourceCoordinates.size(), xCoordinates, yCoordinates);

    vector<std::tuple<double, double>> destinationCoordinates;
    for (size_t i = 0; i < sourceCoordinates.size(); i++)
    {
        destinationCoordinates.push_back(Point(xCoordinates[i], yCoordinates[i]));
    }
    return destinationCoordinates;
}

Area CoordinateTransformation::MapArea(Area other)
{
    Point convertedLeftTop = MapCoordinate(other.LeftTop());
    Point convertedBottomRight = MapCoordinate(other.BottomRight());

    return Area(_destinationReference, convertedLeftTop, convertedBottomRight);
}

Point CoordinateTransformation::MapCoordinate(SpatialReference sourceReference,
        SpatialReference targetReference, 
        Point sourceCoordinate)
{
    return CoordinateTransformation(sourceReference, targetReference).MapCoordinate(sourceCoordinate);
}

vector<Point> CoordinateTransformation::MapCoordinates(SpatialReference sourceReference, SpatialReference targetReference, vector<Point> sourceCoordinates)
{
    return CoordinateTransformation(sourceReference, targetReference).MapCoordinates(sourceCoordinates);
}

Area CoordinateTransformation::MapArea(Area other, string epsgCode)
{
    SpatialReference destinationReference(epsgCode.c_str());
    return MapArea(other, destinationReference);
}

Area CoordinateTransformation::MapArea(Area other, SpatialReference destinationReference)
{
    SpatialReference sourceReference = other.ProjectionReference();

    return CoordinateTransformation(sourceReference, destinationReference).MapArea(other);
}

OGRCoordinateTransformation* CoordinateTransformation::Transformation()
{
    if (!_transformation)
    {
        OGRSpatialReference sourceInnerReference = _sourceReference.InnerReference();
        OGRSpatialReference targetInnerReference = _destinationReference.InnerReference();
        _transformation = OGRCreateCoordinateTransformation(&sourceInnerReference, &targetInnerReference);
    }
    return _transformation;
}

