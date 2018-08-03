#ifndef POINT_H
#define POINT_H

#include <iostream>
#include <gdal/ogrsf_frmts.h>
#include <tuple>
#include "Geometry.h"

struct Point : public Geometry
{
    double X;
    double Y;
	double Z;
	double data[3];
	
    Point();
    Point(OGRGeometry* geometry);
    Point(const std::tuple<double, double>& pointData);
    Point (double x, double y, double z = 0.0);

    double* Data();

    virtual operator OGRGeometry*() const;
    Point& operator=(const OGRGeometry* geometry);

    virtual operator std::tuple<double, double>() const;
    Point& operator=(const std::tuple<double, double> pointData);

    virtual shared_ptr<Geometry> Transform(Geometry::TransformFunction transformFunction) const;

private:
    void ParseGeometry(const OGRGeometry* geometry);
};

static GeometryTemplateFactory<Point> global_PointFactory(Geometry::PointType);

inline std::ostream& operator<<(std::ostream &strm, const Point &p)
{
    return strm << "Point(" << p.X << "," << p.Y << "," << p.Z << ")";
}

#endif
