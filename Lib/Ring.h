#ifndef RING_H
#define RING_H

#include <vector>
#include <functional>
#include <initializer_list>
#include <gdal/ogrsf_frmts.h>
#include "Point.h"
#include "Geometry.h"

class Ring : public Geometry<Ring>
{
    public:

        using iterator = vector<Point>::iterator;
        using const_iterator = vector<Point>::const_iterator;

        Ring();
        Ring(vector<Point> points);
        Ring(std::initializer_list<Point> list);
        virtual ~Ring();

        operator OGRGeometry*() const;
        Ring& operator=(OGRGeometry* ring);
        Ring& operator=(OGRLinearRing* ring);

        bool IsClockwise();
        Ring Transform(Geometry<Ring>::TransformFunction transformFunction) const;
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
        bool _clockwise;
};

inline std::ostream& operator<<(std::ostream &strm, const Ring &ring)
{
    strm << "Ring" << endl;
    for (auto& point : ring)
    {
        strm << point << endl;
    }
    return strm;
}

#endif // RING_H
