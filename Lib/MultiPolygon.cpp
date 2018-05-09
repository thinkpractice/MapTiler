#include "MultiPolygon.h"

MultiPolygon::MultiPolygon()
{
}

MultiPolygon::~MultiPolygon()
{
}

void MultiPolygon::AddPolygon(Polygon polygon)
{
    _polygons.push_back(polygon);
}

