#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include <memory>
#include <functional>
#include "Point.h"

using namespace std;

class Polygon
{
    public:
        Polygon();
        virtual ~Polygon();

        using TransformFunction = function< vector<Point>(vector<Point>&)>;

        class Ring
        {
            public:
                using iterator = vector<Point>::iterator;
                using const_iterator = vector<Point>::const_iterator;

                Ring();
                Ring(vector<Point> points);
                virtual ~Ring();

                Ring Transform(TransformFunction transformFunction);
                void AddPoint(Point point);
                void SetPoints(vector<Point> points) { _points = points; }
                vector<Point>& Points() { return _points; }
                
                iterator begin() { return _points.begin(); }
                iterator end() { return _points.end(); }
                const_iterator begin() const { return _points.begin(); }
                const_iterator end() const { return _points.end(); }
                const_iterator cbegin() const { return _points.cbegin(); }
                const_iterator cend() const { return _points.cend(); }

            private:
                vector<Point> _points;
        };

        Polygon(Ring externalRing, vector<Ring> internalRings);
        Ring& ExternalRing();
        vector<Ring>& InternalRings();

        Polygon Transform(TransformFunction transformFunction);
    private:
        Ring _externalRing;
        vector<Ring> _internalRings;
};

#endif
