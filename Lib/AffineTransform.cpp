#include "AffineTransform.h"
#include <cstring>

AffineTransform::AffineTransform(double* transform)
{
    std::memcpy(_transform, transform, 6 * sizeof(double));
}

AffineTransform::~AffineTransform()
{
}

Point AffineTransform::Transform(const Point& point)
{
    Xp = _transform[0] + _transform[1] * point.X + _transform[2] * point.Y;
    Yp = _transform[3] + _transform[4] * point.X + _transform[5] * point.Y;
    return Point(Xp, Yp);
}