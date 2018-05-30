#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include <memory>
#include <functional>
#include <iostream>
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

        Ring GetExternalRing() const;
        vector<Ring> GetInternalRings() const;

        Polygon Transform(TransformFunction transformFunction);
    private:
        Ring _externalRing;
        vector<Ring> _internalRings;
};

inline std::ostream& operator<<(std::ostream &strm, const Polygon::Ring &ring)
{
    strm << "Ring" << endl;
    for (auto& point : ring)
    {
        strm << point << endl;
    }
    return strm;
}

inline std::ostream& operator<<(std::ostream &strm, const Polygon& polygon)
{
    //TODO refactor Get methods.
    strm << "Polygon" << endl;
    strm << "==== External Ring ====" << endl;
    strm << polygon.GetExternalRing() << endl;
    for (auto& ring : polygon.GetInternalRings())
    {
        strm << "==== Internal Ring ====" << endl;
        strm << ring << endl;
    }
    return strm;
}

#endif
