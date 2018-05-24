#ifndef MULTI_POLYGON_H
#define MULTI_POLYGON_H 

#include "Polygon.h"
#include <vector>

using namespace std;

class MultiPolygon
{
    public:
        MultiPolygon();
        MultiPolygon(vector<Polygon> polygons);
        virtual ~MultiPolygon();

        void AddPolygon(Polygon polygon);
        vector<Polygon>& Polygons() { return _polygons; };

        using iterator = vector<Polygon>::iterator;
        using const_iterator = vector<Polygon>::const_iterator;

        iterator begin() { return _polygons.begin(); }
        iterator end() { return _polygons.end(); }
        const_iterator begin() const { return _polygons.begin(); }
        const_iterator end() const { return _polygons.end(); }
        const_iterator cbegin() const { return _polygons.cbegin(); }
        const_iterator cend() const { return _polygons.cend(); }

    private:
        vector <Polygon> _polygons;
};

#endif /* ifndef MULTI_POLYGON_H */
