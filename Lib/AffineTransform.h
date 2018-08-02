#ifndef AFFINE_TRANSFORM_H
#define AFFINE_TRANSFORM_H

#include "Rect.h"
#include "Area.h"
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
        std::shared_ptr<Geometry> Transform(std::shared_ptr<Geometry> geometry);

        Point ReverseTransform(const Point& geoPoint);
        vector<Point> ReverseTransform(vector<Point>& geoPoints);

        std::shared_ptr<Geometry> ReverseTransform(std::shared_ptr<Geometry> point);

        void GetTransformMatrix(double* transform) const;

        void ToGdal(double* gdalTransform) const;

    public:
        static AffineTransform FromGdal(double* gdalTransform);
        static AffineTransform FromAreaAndRect(const Area& area, const Rect& rect);

    private:
        double _transform[9];
};

Point operator*(const AffineTransform& affineTransform, Point p);

#endif
