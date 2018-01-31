#ifndef GEOMAP_PROVIDER_H
#define GEOMAP_PROVIDER_H

#include <string>
#include "GeoMap.h"

using namespace std;

class GeoMapProvider
{
    public:
        GeoMapProvider(string filename);
        virtual ~GeoMapProvider();

        vector<GeoMap*> Maps();

    private:
        string _filename;
        vector<GeoMap*> _maps;

};

#endif
