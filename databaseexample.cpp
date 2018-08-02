#include <gdal/ogrsf_frmts.h>
#include <iostream>
#include "Lib/VectorFile.h"
#include "Lib/MappedVectorFile.h"
#include "Lib/Layer.h"
#include "Lib/Feature.h"
#include "Lib/GeoMapProvider.h"
#include "Lib/Area.h"
#include "Lib/Polygon.h"

int main(int argc, char** argv)
{
    GDALAllRegister();
    VectorFile databaseFile("PG:host=localhost port=5432 dbname=addresses user=postgres password=tim2839", VectorFile::OpenMode::Update);
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

        std::cout << "type=" << feature.GetGeometry().Type() << std::endl;
        std::cout << feature.GetGeometry().InnerGeometry() << std::endl;
    }

    Area landgraaf(5.9906272, 50.8702199, 6.094184, 50.9347014);
    auto feature = layer->NewFeature();
    feature.SetField("description", "Landgraaf");
    feature.SetGeometry(landgraaf);
    layer->AddFeature(feature);
    layer->Save();

    return 0;
}
