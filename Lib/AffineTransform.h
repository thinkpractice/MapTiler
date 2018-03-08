#ifndef AFFINE_TRANSFORM_H
#define AFFINE_TRANSFORM_H

#include "Point.h"

class AffineTransform
{
    public:
        AffineTransform(double* transform);
        virtual ~AffineTransform();

        Point Transform(const Point& point);

    private:
        double _transform[6];
};

#endif
