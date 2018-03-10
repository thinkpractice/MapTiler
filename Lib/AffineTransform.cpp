#include "AffineTransform.h"
#include <cstring>

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

AffineTransform AffineTransform::Invert()
{
    double idet = 1.0 / Determinant();
    double inverseTransform[9] =
    {

    };
    return AffineTransform(inverseTransform);
}

Point AffineTransform::Transform(const Point& rasterPoint)
{
    return (*this) * rasterPoint;
}

Point AffineTransform::ReverseTransform(const Point& geoPoint)
{
    return Invert() * geoPoint;
}

void AffineTransform::GetTransformMatrix(double* transform)
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
