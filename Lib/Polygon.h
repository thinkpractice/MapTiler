#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include <memory>
#include "Point.h"

using namespace std;

class Polygon
{
    public:
        Polygon();
        virtual ~Polygon();

        class Ring
        {
            public:
                using iterator = vector<Point>::iterator;
                using const_iterator = vector<Point>::const_iterator;

                Ring();
                Ring(vector<Point> points);
                virtual ~Ring();

                void AddPoint(Point point);
                
                iterator begin() { return _points.begin(); }
                iterator end() { return _points.end(); }
                const_iterator begin() const { return _points.begin(); }
                const_iterator end() const { return _points.end(); }
                const_iterator cbegin() const { return _points.cbegin(); }
                const_iterator cend() const { return _points.cend(); }

            private:
                vector<Point> _points;
        };

        const Ring& ExternalRing() const;
        const Ring& InternalRing() const;

    private:
        Ring _internalRing;
        Ring _externalRing;
};

#endif
