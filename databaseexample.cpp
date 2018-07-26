#include "ogrsf_frmts.h"
#include <iostream>
#include "Lib/VectorFile.h"
#include "Lib/MappedVectorFile.h"
#include "Lib/Layer.h"
#include "Lib/Feature.h"
#include "Lib/GeoMapProvider.h"

int main(int argc, char** argv)
{
    GDALAllRegister();
    VectorFile databaseFile("PG:host=localhost port=5432 dbname=addresses user=postgres password=tim2839");
    for (auto layer : databaseFile.Layers())
    {
        std::cout << "layer: " << layer->Name() << std::endl;
    }

    auto layer = databaseFile["areaofinterest"];

    for (auto it = layer->begin(); it != layer->end(); ++it)
    {
        auto feature = *it;
        std::cout << "feature_id=" << feature.FeatureId() << std::endl;
        for (auto field : feature)
        {
            std::cout << field.Name() << "=" << field.Value() << ",";
        }

        std::cout << "type=" << feature.Geometry().Type() << std::endl;
        if (feature.Geometry().HasPoint())
        {
            std::cout << feature.Geometry().GetPoint() << std::endl;
        }
        else if (feature.Geometry().HasPolygon())
        {
            std::cout << feature.Geometry().GetPolygon() << std::endl;
        }
        else if (feature.Geometry().HasMultiPolygon())
        {
            std::cout << feature.Geometry().GetMultiPolygon() << std::endl;
        }
    }
    return 0;
}
