#ifndef GEOMAP_PROVIDER_H
#define GEOMAP_PROVIDER_H

#include <string>
#include <memory>
#include <vector>
#include "GeoMap.h"

class GeoMapProvider
{
    public:
    GeoMapProvider(std::string filename, std::vector<std::string> driverOptions = {});
        virtual ~GeoMapProvider();
		
        std::vector<std::shared_ptr<GeoMap>> Maps();
        std::vector<std::string> DriverOptions() const;

    private:
        std::vector<std::shared_ptr<GeoMap>> RetrieveMaps();

    private:
        std::string _filename;
        std::vector<std::string> _driverOptions;
        std::vector<std::shared_ptr<GeoMap>> _maps;

};

#endif
