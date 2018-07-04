#ifndef GEOMAP_PROVIDER_H
#define GEOMAP_PROVIDER_H

#include <string>
#include <memory>
#include "GeoMap.h"

using namespace std;

class GeoMapProvider
{
    public:
		GeoMapProvider(string filename);		
        virtual ~GeoMapProvider();
		
        vector<shared_ptr<GeoMap>> Maps();

    private:
        vector<shared_ptr<GeoMap>> RetrieveMaps();

    private:
        string _filename;
        vector<shared_ptr<GeoMap>> _maps;

};

#endif
