#ifndef COORDINATE_TRANSFORMATION_H
#define COORDINATE_TRANSFORMATION_H

#include "Point.h"
#include "Polygon.h"
#include "MultiPolygon.h"
#include "Area.h"
#include "SpatialReference.h"
#include <vector>
#include <ogr_spatialref.h>

using namespace std;

class CoordinateTransformation
{
    public:
        CoordinateTransformation(const SpatialReference& sourceReference, const SpatialReference& destinationReference);
        virtual ~CoordinateTransformation();

        Point MapCoordinate(Point sourceCoordinate);
        vector<Point> MapCoordinates(vector<Point> sourceCoordinates);

        Area MapArea(Area other);
        
        Polygon MapPolygon(Polygon polygon);
        MultiPolygon MapMultiPolygon(MultiPolygon multiPolygon);

    public:
        static Point MapCoordinate(SpatialReference sourceReference, 
                SpatialReference targetReference,
                Point sourceCoordinate);
        static vector<Point> MapCoordinates(SpatialReference sourceReference, SpatialReference targetReference, vector<Point> sourceCoordinates);
        static Area MapArea(Area other, string epsgCode);
        static Area MapArea(Area other, SpatialReference destinationReference);

    private:
        OGRCoordinateTransformation* Transformation();

    private:
        SpatialReference _sourceReference;
        SpatialReference _destinationReference;
        OGRCoordinateTransformation* _transformation;

};

#endif
