#ifndef COORDINATE_TRANSFORMATION_H
#define COORDINATE_TRANSFORMATION_H

#include "Point.h"
#include "Polygon.h"
#include "MultiPolygon.h"
#include "Area.h"
#include "SpatialReference.h"
#include <vector>
#include <gdal/ogr_spatialref.h>


class CoordinateTransformation
{
    public:
        CoordinateTransformation(const SpatialReference& sourceReference, const SpatialReference& destinationReference);
        virtual ~CoordinateTransformation();

        Point MapCoordinate(Point sourceCoordinate);
        std::vector<Point> MapCoordinates(vector<Point> sourceCoordinates);
        std::vector<std::tuple<double, double>> MapCoordinates(std::vector<std::tuple<double, double>> sourceCoordinates);


        Area MapArea(Area other);
        
        std::shared_ptr<Geometry> MapGeometry(const std::shared_ptr<Geometry> geometry)
        {
            return geometry->Transform([&](const std::vector<std::tuple<double, double>> points) -> std::vector<std::tuple<double, double>> {
                    return MapCoordinates(points);
                    });
        }

    public:
        static Point MapCoordinate(SpatialReference sourceReference, 
                SpatialReference targetReference,
                Point sourceCoordinate);
        static std::vector<Point> MapCoordinates(SpatialReference sourceReference, SpatialReference targetReference, std::vector<Point> sourceCoordinates);
        static Area MapArea(Area other, std::string epsgCode);
        static Area MapArea(Area other, SpatialReference destinationReference);

    private:
        OGRCoordinateTransformation* Transformation();

    private:
        SpatialReference _sourceReference;
        SpatialReference _destinationReference;
        OGRCoordinateTransformation* _transformation;

};

#endif
