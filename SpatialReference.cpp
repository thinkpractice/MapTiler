#include "SpatialReference.h"

SpatialReference::SpatialReference(string epsgCode)
                    :   _epsgCode(epsgCode)
{
}


string SpatialReference::EpsgCode()
{
    return _epsgCode;
}
