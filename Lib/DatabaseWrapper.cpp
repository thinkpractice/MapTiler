#include "DatabaseWrapper.h"

DatabaseWrapper::DatabaseWrapper(std::shared_ptr<GeoMap> geoMap)
                    :	_map(geoMap)
{
}

int DatabaseWrapper::SaveAreaOfInterest(const Area &areaOfInterest)
{

}

int DatabaseWrapper::SaveTile(int parentAreaId, std::string uuid, const Area &tileArea)
{

}

int DatabaseWrapper::SaveTileFile(int tileId, std::string filename, std::string layerName, int year)
{

}
