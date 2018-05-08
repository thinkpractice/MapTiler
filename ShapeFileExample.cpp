#include "ogrsf_frmts.h"
#include <iostream>
#include "Lib/VectorFile.h"
#include "Lib/Layer.h"

using namespace std;

int main(int argc, char** argv)
{
    if (argc < 2)
        return -1;

    string filename(argv[1]);

    GDALAllRegister();

    filename = "WFS:https://geodata.nationaalgeoregister.nl/bag/wfs?SERVICE=wfs";
    //filename = "WFS:https://geodata.nationaalgeoregister.nl/inspireadressen/wfs?SERVICE=wfs";
    //filename = "WMS:https://geodata.nationaalgeoregister.nl/inspireadressen/wms?SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&LAYERS=inspireadressen&SRS=EPSG:28992&BBOX=0.0,0.0,277922.729,613046.0";

    VectorFile vectorFile(filename);
    
    cout << "# layers=" << vectorFile.LayerCount() << endl;
    Layer* layer = vectorFile[1];

    for (auto* layer : vectorFile.Layers())
    {
        cout << "layer : " << layer->Name() << endl;
    }

    int numberOfFeatures = 0;
    for (auto feature : (*layer))
    {
        numberOfFeatures++;
        if (numberOfFeatures > 10)
            break;
        for (auto field : feature)
        {
            cout << field.Name() << "=" << field.Value() << ",";
        }
        cout << endl;
    }
    cout << "Number of features: " << numberOfFeatures << endl;

    /*OGRSpatialReference* spatialRef = poLayer->GetSpatialRef();
    spatialRef->dumpReadable();

    cout << "Here poLayer =" << poLayer << endl;
    OGRFeature *poFeature;
    poLayer->ResetReading();
    while( (poFeature = poLayer->GetNextFeature()) != NULL )
    {
        OGRFeatureDefn *poFDefn = poLayer->GetLayerDefn();
        cout << poFDefn->GetName() << endl;
        
        int iField;

        for( iField = 0; iField < poFDefn->GetFieldCount(); iField++ )
        {
            OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn( iField );
            const char* fieldName = poFieldDefn->GetNameRef();
            if( poFieldDefn->GetType() == OFTInteger )
                printf( "%s=%d,", fieldName, poFeature->GetFieldAsInteger( iField ) );
            else if( poFieldDefn->GetType() == OFTInteger64 )
                printf("%s=" CPL_FRMT_GIB ",", fieldName, poFeature->GetFieldAsInteger64( iField ) );
            else if( poFieldDefn->GetType() == OFTReal )
                printf( "%s=%.3f,", fieldName, poFeature->GetFieldAsDouble(iField) );
            else if( poFieldDefn->GetType() == OFTString )
                printf( "%s=%s,", fieldName, poFeature->GetFieldAsString(iField) );
            else
                printf( "%s=%s,", fieldName, poFeature->GetFieldAsString(iField) );
        }


        OGRGeometry *poGeometry = poFeature->GetGeometryRef();
        cout << "geometry type = " << wkbFlatten(poGeometry->getGeometryType()) << endl;
        if( poGeometry != NULL
                && wkbFlatten(poGeometry->getGeometryType()) == wkbPoint )
        {
            OGRPoint *poPoint = (OGRPoint *) poGeometry;
            printf( "%.3f,%3.f\n", poPoint->getX(), poPoint->getY() );
        }
        else if (poGeometry != NULL && wkbFlatten(poGeometry->getGeometryType()) == wkbPolygon)
        {
            cout << "polygon" << endl;
            OGRPolygon* polygon = (OGRPolygon*)poGeometry;
            OGRLinearRing* ring = polygon->getExteriorRing();
            for (int i = 0; i < ring->getNumPoints(); i++)
            {
                OGRPoint point;
                ring->getPoint(i, &point);
                printf( "%.6f,%.6f\n", point.getX(), point.getY());
            }

        }
        else if (poGeometry != NULL && wkbFlatten(poGeometry->getGeometryType()) == wkbMultiPolygon)
        {
            cout << "multipolygon" << endl;
        }
        else
        {
            printf( "no point geometry\n" );
        }
        OGRFeature::DestroyFeature( poFeature );
        break;
    }*/

    delete layer;
}

