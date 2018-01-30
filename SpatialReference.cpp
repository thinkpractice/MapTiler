#include "SpatialReference.h"

SpatialReference::SpatialReference(string epsgCode)
                    :   _epsgCode(epsgCode)
{
}


string SpatialReference::EpsgCode()
{
    return _epsgCode;
}

bool SpatialReference::hasEpsgCode(string epsgCode)
{
    //TODO tolower()
    return epsgCode == EpsgCode();
}