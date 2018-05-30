#include "ogrsf_frmts.h"
#include <iostream>
#include "Lib/VectorFile.h"
#include "Lib/MappedVectorFile.h"
#include "Lib/Layer.h"
#include "Lib/Feature.h"
#include "Lib/GeoMapProvider.h"

using namespace std;

template<class InputIterator>
void printFeatures(const InputIterator& begin, const InputIterator& end, int maxFeatures = 10)
{
    int fi = 0;
    for (auto feature = begin; feature != end; ++feature)
    {
        if (fi > maxFeatures)
            break;
        for (auto field : *feature)
        {
            cout << field.Name() << "=" << field.Value() << ",";
        }
        fi++;
        cout << endl;
    }
}

int main(int argc, char** argv)
{
    if (argc < 2)
        return -1;

    string filename(argv[1]);

    GDALAllRegister();

    //filename = "WFS:https://geodata.nationaalgeoregister.nl/inspireadressen/wfs?SERVICE=wfs";
    //filename = "WMS:https://geodata.nationaalgeoregister.nl/inspireadressen/wms?SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&LAYERS=inspireadressen&SRS=EPSG:28992&BBOX=0.0,0.0,277922.729,613046.0";

    VectorFile addresses("WFS:https://geodata.nationaalgeoregister.nl/inspireadressen/wfs?SERVICE=wfs");
    auto addressLayer = addresses[0];
    //printFeatures(addressLayer.begin(), addressLayer.end());

    cout << "address layer name: " << addressLayer->Name() << endl;
//addressLayer.SetSpatialFilter(Area(addressLayer.ProjectionReference(), Point(196272.036571, 323090.567587), Point(197456.875125,320015.575080)));
    addressLayer->SetSpatialFilter(Area(6.009787,50.89009,6.01117,50.8892935));

    printFeatures(addressLayer->begin(), addressLayer->end());

    //vector<Feature> coupledAddresses = addresses.ExecuteSql("select * from inspireadressen left join '../ZonnePanelen/solarpanel_addresses'.addresses on inspireadressen.postcode = addresses.postcode");
    //printFeatures(coupledAddresses.begin(), coupledAddresses.end());

    VectorFile solarpanel_addresses("/home/tjadejong/Documents/CBS/ZonnePanelen/solarpanel_addresses");
    cout << "LayerCount solar_panel addresses = " << solarpanel_addresses.LayerCount() << endl;

    vector<Feature> filteredAddresses = solarpanel_addresses.ExecuteSql("select * from addresses where postcode='6372KV'");
    printFeatures(filteredAddresses.begin(), filteredAddresses.end());

    filename = "WFS:https://geodata.nationaalgeoregister.nl/bag/wfs?SERVICE=wfs";
    VectorFile vectorFile(filename);
    
    cout << "# layers=" << vectorFile.LayerCount() << endl;

    for (auto layer : vectorFile.Layers())
    {
        cout << "layer : " << layer->Name() << endl;
    }

    /*TODO identificatie= 882100000015783, has multipolygon
     * wkt_geom	identificatie	bouwjaar	status	gebruiksdoel	oppervlakte_min	oppervlakte_max	aantal_verblijfsobjecten	actualiteitsdatum
    MultiPolygon (((6.02844800000000003 50.90652500000000202, 6.02854500000000026 50.9065920000000034, 6.02863199999999999 50.9065390000000022, 6.02881100000000014 50.90666199999999719, 6.02848700000000015 50.90685400000000271, 6.02830699999999986 50.9067450000000008, 6.02821599999999957 50.90668300000000102, 6.0282600000000004 50.90665599999999813, 6.02824199999999966 50.90664400000000001, 6.02844800000000003 50.90652500000000202),(6.02840199999999982 50.9066880000000026, 6.02852000000000032 50.90675699999999892, 6.0286489999999997 50.90668300000000102, 6.02852399999999999 50.90659699999999788, 6.02839300000000033 50.90667200000000037, 6.02841000000000005 50.90668300000000102, 6.02840199999999982 50.9066880000000026)))	882100000015783	1720	Pand in gebruik	woonfunctie	194	396	2	
    */

    GeoMapProvider mapProvider(u8"WMTS:https://geodata.nationaalgeoregister.nl/luchtfoto/rgb/wmts/1.0.0/WMTSCapabilities.xml");
    GeoMap* map = mapProvider.Maps()[1];

    MappedVectorFile mappedVectorFile(filename, map->ProjectionReference(), map->MapTransform());
    //auto layer = *vectorFile[1].get();
    auto layer = mappedVectorFile[1];
    
    int numberOfFeatures = 0;
    for (auto it = layer->begin(); it != layer->end(); ++it)
    {
        auto feature = *it;
        numberOfFeatures++;
        if (numberOfFeatures > 10)
            break;
        for (auto field : feature)
        {
            cout << field.Name() << "=" << field.Value() << ",";
        }
        
        cout << "type=" << feature.Geometry().Type() << endl;
        if (feature.Geometry().HasPoint())
        {
            cout << "Point(" << feature.Geometry().GetPoint().X << "," << feature.Geometry().GetPoint().Y << ")" << endl;
        }
        else if (feature.Geometry().HasPolygon())
        {
            cout << "==== External Ring ====" << endl;
            for (auto p : feature.Geometry().GetPolygon().ExternalRing())
            {
                cout << "Point(" << p.X << "," << p.Y << ")" << endl;
            }

            for (auto ring : feature.Geometry().GetPolygon().InternalRings())
            {
                cout << "==== Internal Ring ====" << endl; 
                for (auto p : ring)
                {
                    cout << "Point(" << p.X << "," << p.Y << ")" << endl;
                }
            }
        }
        else if (feature.Geometry().HasMultiPolygon())
        {
            cout << "=== Polygon in MultiPolygon ===" << endl;
            for (auto polygon : feature.Geometry().GetMultiPolygon())
            {
                cout << "==== External Ring ====" << endl;
                for (auto p : polygon.ExternalRing())
                {
                    cout << "Point(" << p.X << "," << p.Y << ")" << endl;
                }

                for (auto ring : polygon.InternalRings())
                {
                    cout << "==== Internal Ring ====" << endl; 
                    for (auto p : ring)
                    {
                        cout << "Point(" << p.X << "," << p.Y << ")" << endl;
                    }
                }
            }
        }

        cout << endl;
    }
    cout << "Number of features: " << numberOfFeatures << endl;
}

