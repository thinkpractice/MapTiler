#include "Point.h"

Point::Point()
    :	Point(0.0, 0.0, 0.0)
{
}

Point::Point(OGRGeometry* geometry)
        :	Geometry(Geometry::PointType)
{
    ParseGeometry(geometry);
}

Point::Point(const std::tuple<double, double>& pointData)
        :	Point(std::get<0>(pointData), std::get<1>(pointData), 0)
{
}

Point::Point (double x, double y, double z)
        :	Geometry(Geometry::PointType),
            X(x), Y(y), Z(z)
{
}

double* Point::Data()
{
    data[0] = X;
    data[1] = Y;
    data[2] = Z;
    return data;
}

Point::operator OGRGeometry*() const
{
    return new OGRPoint(X, Y, Z);
}

Point& Point::operator=(const OGRGeometry *geometry)
{
    ParseGeometry(geometry);
    return *this;
}

Point::operator std::tuple<double, double>() const
{
    return std::make_tuple(X, Y);
}

Point &Point::operator=(const std::tuple<double, double> pointData)
{
    X = std::get<0>(pointData);
    Y = std::get<1>(pointData);
    Z = 0;
    return *this;
}

shared_ptr<Geometry> Point::Transform(Geometry::TransformFunction transformFunction) const
{
    vector<std::tuple<double, double>> originalPoints = {std::make_tuple(X, Y)};
    vector<std::tuple<double, double>> transformedPoints = transformFunction(originalPoints);
    std::tuple<double, double> p = transformedPoints.front();
    return make_shared<Point>(std::get<0>(p), std::get<1>(p));
}

void Point::ParseGeometry(const OGRGeometry *geometry)
{
    Geometry::ParseGeometry(geometry);
    const OGRPoint* point = dynamic_cast<const OGRPoint*>(geometry);
    X = point->getX();
    Y = point->getY();
    Z = point->getZ();
}
