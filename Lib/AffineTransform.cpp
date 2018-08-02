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

double AffineTransform::Determinant() const
{
    return _transform[0] * _transform[4] - _transform[1] * _transform[3];
}

bool AffineTransform::IsDegenerate() const
{
    return Determinant() == 0;
}

AffineTransform AffineTransform::Invert() const
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

std::shared_ptr<Geometry> AffineTransform::Transform(std::shared_ptr<Geometry> geometry)

{
    return geometry->Transform([&](vector<std::tuple<double, double>> points) -> vector<std::tuple<double, double>> {
                std::vector<std::tuple<double, double>> transformedPoints;
                for (auto& point : points)
                   transformedPoints.push_back(Transform(point));
                return transformedPoints;
             });
}

Point AffineTransform::ReverseTransform(const Point& geoPoint) const
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

std::shared_ptr<Geometry> AffineTransform::ReverseTransform(const std::shared_ptr<Geometry> geometry) const
{
    return geometry->Transform([&](std::vector<std::tuple<double, double>> points) -> vector<std::tuple<double, double>> {
             vector<std::tuple<double, double>> reverseTransformedPoints;
             for (auto& point : points)
                reverseTransformedPoints.push_back(ReverseTransform(point));
             return reverseTransformedPoints;
    });
}

void AffineTransform::GetTransformMatrix(double* transform) const
{
    std::memcpy(transform, _transform, sizeof(double) * 9);
}

void AffineTransform::ToGdal(double* transform) const
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

AffineTransform AffineTransform::FromAreaAndRect(const Area& area, const Rect& rect)
{
    //TODO check if calculation is correct
    //See if we can derive it with linear algebra
    double width = rect.Width();
    double height = rect.Height();

    double xmin = area.LeftTop().X;
    double ymin = area.LeftTop().Y;
    double xmax = area.BottomRight().X;
    double ymax = area.BottomRight().Y;

    double xres = (xmax - xmin) / double(width);
    double yres = (ymax - ymin) / double(height);

    double transform[9] =
    {
        xres, 0.0, xmin,
        0.0, -yres, ymax,
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
