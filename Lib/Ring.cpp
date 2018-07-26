#include "Ring.h"

Ring::Ring()
{
}

Ring::Ring(vector<Point> points)
                :   _points(points)
{
}

Ring::~Ring()
{
}

Ring Ring::Transform(TransformFunction transformFunction)
{
    vector<Point> mappedPoints = transformFunction(Points());
    return Ring(mappedPoints);
}

void Ring::AddPoint(const Point point)
{
    _points.push_back(point);
}

