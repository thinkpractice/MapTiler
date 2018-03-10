#ifndef AFFINE_TRANSFORM_H
#define AFFINE_TRANSFORM_H

#include "Point.h"

class AffineTransform
{
    public:
        AffineTransform();
        AffineTransform(double* transform);
        virtual ~AffineTransform();

        double Determinant();
        bool IsDegenerate();
        AffineTransform Invert();


        Point Transform(const Point& rasterPoint);
        Point ReverseTransform(const Point& geoPoint);
        void GetTransformMatrix(double* transform) const;
        
        void ToGdal(double* gdalTransform);

    public:
        static AffineTransform FromGdal(double* gdalTransform);

    private:
        double _transform[9];
};

Point operator*(const AffineTransform& affineTransform, Point p);

#endif
