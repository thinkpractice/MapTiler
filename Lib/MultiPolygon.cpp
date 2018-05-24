#include "MultiPolygon.h"

MultiPolygon::MultiPolygon()
{
}


MultiPolygon::MultiPolygon(vector<Polygon> polygons)
                :   _polygons(polygons)
{
}

MultiPolygon::~MultiPolygon()
{
}

void MultiPolygon::AddPolygon(Polygon polygon)
{
    _polygons.push_back(polygon);
}

