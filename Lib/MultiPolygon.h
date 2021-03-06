#ifndef MULTI_POLYGON_H
#define MULTI_POLYGON_H 

#include <gdal/ogrsf_frmts.h>
#include "Polygon.h"
#include <vector>

using namespace std;

class MultiPolygon : public Geometry
{
    public:
        MultiPolygon();
        MultiPolygon(const OGRGeometry* ogrGeometry);
        MultiPolygon(vector<Polygon> polygons);
        virtual ~MultiPolygon();

        operator OGRGeometry*() const;
        MultiPolygon& operator=(const OGRGeometry* geometry);

        shared_ptr<Geometry> Transform(Geometry::TransformFunction transformFunction) const;
        void AddPolygon(Polygon polygon);
        vector<Polygon>& Polygons() { return _polygons; }

        using iterator = vector<Polygon>::iterator;
        using const_iterator = vector<Polygon>::const_iterator;

        iterator begin() { return _polygons.begin(); }
        iterator end() { return _polygons.end(); }
        const_iterator begin() const { return _polygons.begin(); }
        const_iterator end() const { return _polygons.end(); }
        const_iterator cbegin() const { return _polygons.cbegin(); }
        const_iterator cend() const { return _polygons.cend(); }

    private:
        void ParseGeometry(const OGRGeometry* geometry);

    private:
        vector <Polygon> _polygons;
};

static GeometryTemplateFactory<MultiPolygon> global_MultiPolygonFactory(Geometry::MultiPolygonType);

inline std::ostream& operator<<(std::ostream &strm, const MultiPolygon& multiPolygon)
{
    strm << "MultiPolygon" << endl;
    for (auto& polygon : multiPolygon)
        strm << polygon << endl;
    return strm;
}

#endif /* ifndef MULTI_POLYGON_H */
