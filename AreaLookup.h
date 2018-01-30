#ifndef AREA_LOOKUP_H
#define AREA_LOOKUP_H

#include <string>
#include "Area.h"

class AreaLookup
{
    public:
        AreaLookup();

        Area GetAreaForAddress(string address);

};

#endif
