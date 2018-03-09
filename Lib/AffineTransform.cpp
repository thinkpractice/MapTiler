#include "AffineTransform.h"
#include <cstring>

AffineTransform::AffineTransform(double* transform)
{
    std::memcpy(_transform, transform, 6 * sizeof(double));
}

AffineTransform::~AffineTransform()
{
}

Point AffineTransform::Transform(const Point& rasterPoint)
{
    double Xp = _transform[0] + _transform[1] * rasterPoint.X + _transform[2] * rasterPoint.Y;
    double Yp = _transform[3] + _transform[4] * rasterPoint.X + _transform[5] * rasterPoint.Y;
    return Point(Xp, Yp);
}

Point AffineTransform::ReverseTransform(const Point& geoPoint)
{
    double x = 0;
    double y = 0;
    return Point(x, y);
}

void AffineTransform::GetTransformMatrix(double* transform)
{
    std::memcpy(transform, _transform, 6 * sizeof(double));
}
