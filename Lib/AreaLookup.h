#ifndef AREA_LOOKUP_H
#define AREA_LOOKUP_H

#include <string>
#include "Area.h"

#include <QGeoServiceProvider>
#include <QGeoCodingManager>

class AreaLookup
{
    public:
        AreaLookup();

        Area GetAreaForAddress(string address);

    private:
        QGeoServiceProvider* _geoServiceProvider;
        QGeoCodingManager* _geoCodingManager;
};

#endif
