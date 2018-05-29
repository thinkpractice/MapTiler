#ifndef AFFINE_TRANSFORM_H
#define AFFINE_TRANSFORM_H

#include "Point.h"
#include "Polygon.h"
#include "MultiPolygon.h"
#include <vector>

using namespace std;

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
        vector<Point> Transform(vector<Point>& rasterPoints);
        Polygon Transform(Polygon& polygon);
        MultiPolygon Transform(MultiPolygon& multiPolygon);

        Point ReverseTransform(const Point& geoPoint);
        vector<Point> ReverseTransform(vector<Point>& geoPoints);
        Polygon ReverseTransform(Polygon& polygon);
        MultiPolygon ReverseTransform(MultiPolygon& multiPolygon);

        void GetTransformMatrix(double* transform) const;
        
        void ToGdal(double* gdalTransform);

    public:
        static AffineTransform FromGdal(double* gdalTransform);

    private:
        double _transform[9];
};

Point operator*(const AffineTransform& affineTransform, Point p);

#endif
