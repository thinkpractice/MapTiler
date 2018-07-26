#include "Geometry.h"

Geometry::Geometry()
{
}

Geometry::~Geometry()
{
}

Ring Geometry::GetRingFromGdal(OGRLinearRing* ring)
{
    Ring importedRing;
    for (int i = 0; i < ring->getNumPoints(); i++)
    {
        OGRPoint point;
        ring->getPoint(i, &point);
        importedRing.AddPoint(Point(point.getX(), point.getY()));
    }
    return importedRing;
}
