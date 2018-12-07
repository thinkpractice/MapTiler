#ifndef AREA_H
#define AREA_H

#include <string>
#include <float.h>
#include "SpatialReference.h"
#include "Point.h"
#include "Polygon.h"

class Area
{
    public:
        Area();
        Area(const SpatialReference& projectionReference, const Point& leftTop, const Point &bottomRight);
        Area(const SpatialReference& projectionReference, const Point& leftTop, const Point &bottomRight, string description);
        Area(double minX, double minY, double maxX, double maxY, string wellKnownGeogCS = "EPSG:4326", string description="");
        Area(const shared_ptr<Geometry> geometry);

        operator shared_ptr<Geometry>() const
        {
            auto polygon = make_shared<Polygon>();
            polygon->SetSpatialReference(ProjectionReference());
            vector<Point> externalRing =
            {
                LeftTop(),
                Point(BottomRight().X, LeftTop().Y),
                BottomRight(),
                Point(LeftTop().X, BottomRight().Y),
                LeftTop()
            };
            polygon->ExternalRing() = externalRing;
            return polygon;
        }

        Area& operator=(const std::shared_ptr<Geometry> geometry)
        {
            SetAreaFromGeometry(geometry);
            return *this;
        }

        SpatialReference ProjectionReference() const;
        void SetProjectionReference(const SpatialReference& projectionReference);
        void SetEPSG(std::string epsg);

        Point LeftTop() const;
        void SetLeftTop(const Point& leftTop);

        Point BottomRight() const;
        void SetBottomRight(const Point& bottomRight);

        void SetDescription(string description);
        string Description() const;

    private:
        void SetAreaFromGeometry(const std::shared_ptr<Geometry> geometry);

    private:
        SpatialReference _spatialReference;
        Point _leftTop;
        Point _bottomRight;
        string _description;

};

#endif
