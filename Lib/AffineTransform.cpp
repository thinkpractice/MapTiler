#include "AffineTransform.h"
#include <iostream>
#include <cstring>
#include <functional>

using namespace std;

AffineTransform::AffineTransform()
        :    _transform
            {
                1.0, 0.0, 0.0,
                0.0, 1.0, 0.0,
                0.0, 0.0, 1.0
            }
{
}

AffineTransform::AffineTransform(double* transform)
{
    std::memcpy(_transform, transform, sizeof(double) * 9);
}

AffineTransform::~AffineTransform()
{
}

double AffineTransform::Determinant()
{
    return _transform[0] * _transform[4] - _transform[1] * _transform[3];
}

bool AffineTransform::IsDegenerate()
{
    return Determinant() == 0;
}

AffineTransform AffineTransform::Invert()
{
    if (IsDegenerate())
    {
        cout << "Cannot invert degenerate transform" << endl;
        return AffineTransform();
    }
    double idet = 1.0 / Determinant();
    double ra = _transform[4] * idet;
    double rb = -1.0 * _transform[1] * idet;
    double rd = -1.0 * _transform[3] * idet;
    double re = _transform[0] * idet;
    double inverseTransform[9] =
    {
        ra, rb, -1.0 * _transform[2] * ra - _transform[5] * rb,
        rd, re, -1.0 * _transform[2] * rd - _transform[5] * re,
        0.0, 0.0, 1.0
    };

    return AffineTransform(inverseTransform);
}

Point AffineTransform::Transform(const Point& rasterPoint)
{
    return (*this) * rasterPoint;
}

vector<Point> AffineTransform::Transform(vector<Point>& ring)
{
    vector<Point> transformedPoints;
    for (auto& point : ring)
        transformedPoints.push_back(Transform(point));
    return transformedPoints;
}

Polygon AffineTransform::Transform(Polygon& polygon)
{
    return polygon.Transform([&](vector<Point> points) -> vector<Point>{
                return Transform(points);
             });
}

MultiPolygon AffineTransform::Transform(MultiPolygon& multiPolygon)
{
    return multiPolygon.Transform([&](vector<Point> points) -> vector<Point>{
                return Transform(points);
             });
}

Point AffineTransform::ReverseTransform(const Point& geoPoint)
{
    return Invert() * geoPoint;
}

vector<Point> AffineTransform::ReverseTransform(vector<Point>& geoPoints)
{
    vector<Point> reverseTransformedPoints;
    for (auto& point : geoPoints)
        reverseTransformedPoints.push_back(ReverseTransform(point));
    return reverseTransformedPoints;
}

Polygon AffineTransform::ReverseTransform(Polygon& polygon)
{
    return polygon.Transform([&](vector<Point> points) -> vector<Point> {
             return ReverseTransform(points);
    });
}

MultiPolygon AffineTransform::ReverseTransform(MultiPolygon& multiPolygon)
{
    return multiPolygon.Transform([&](vector<Point> points) -> vector<Point> {
             return ReverseTransform(points);
    });
}

void AffineTransform::GetTransformMatrix(double* transform) const
{
    std::memcpy(transform, _transform, sizeof(double) * 9);
}

void AffineTransform::ToGdal(double* transform)
{
    double gdalTransform[6] =
    {
        _transform[2], _transform[0], _transform[1],
        _transform[5], _transform[3], _transform[4]
    };
    std::memcpy(transform, gdalTransform, 6 * sizeof(double));
}

AffineTransform AffineTransform::FromGdal(double* gdalTransform)
{
    double transform[9] =
    {
        gdalTransform[1], gdalTransform[2], gdalTransform[0],
        gdalTransform[4], gdalTransform[5], gdalTransform[3],
        0.0, 0.0, 1.0
    };
    return AffineTransform(transform);
}

Point operator*(const AffineTransform& affineTransform, Point rasterPoint)
{
    double transform[9];
    affineTransform.GetTransformMatrix(transform);

    double xT = transform[2] + transform[0] * rasterPoint.X + transform[1] * rasterPoint.Y;
    double yT = transform[5] + transform[3] * rasterPoint.X + transform[4] * rasterPoint.Y;
    return Point(xT, yT);
}
