#ifndef RING_H
#define RING_H

#include <vector>
#include <functional>
#include <initializer_list>
#include <gdal/ogrsf_frmts.h>
#include "Point.h"
#include "Geometry.h"

class Ring : public Geometry
{
    public:

        using iterator = vector<Point>::iterator;
        using const_iterator = vector<Point>::const_iterator;

        Ring();
        Ring(vector<Point> points);
        Ring(std::initializer_list<Point> list);
        Ring(std::vector<std::tuple<double, double>> list);
        Ring(OGRGeometry* ring);
        virtual ~Ring();

        operator OGRGeometry*() const;
        Ring& operator=(OGRGeometry* ring);
        Ring& operator=(const OGRLinearRing* ring);

        bool IsClockwise();
        shared_ptr<Geometry> Transform(Geometry::TransformFunction transformFunction) const;
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
        void ParseGeometry(const OGRLinearRing* ring);

    private:
        vector<Point> _points;
        bool _clockwise;
};

static GeometryTemplateFactory<Ring> global_RingFactory(Geometry::RingType);

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
