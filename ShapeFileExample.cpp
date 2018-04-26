#include "ogrsf_frmts.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
    if (argc < 2)
        return -1;
    const char* filename = argv[1];

    cout << "Opening " << filename << endl;

    GDALAllRegister();
    GDALDataset       *poDS;
    //poDS = (GDALDataset*) GDALOpenEx( filename, GDAL_OF_VECTOR, NULL, NULL, NULL );
    poDS = (GDALDataset*) GDALOpen( filename, GA_ReadOnly);
    if( poDS == NULL )
    {
        printf( "Open failed.\n" );
        exit( 1 );
    }
    OGRLayer  *poLayer;
    cout << "# layers=" << poDS->GetLayerCount() << endl;
    poLayer = poDS->GetLayer( 0 );

    OGRSpatialReference* spatialRef = poLayer->GetSpatialRef();
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
            /*for (auto* point : polygon)
            {
                printf( "%.3f,%3.f\n", point->getX(), point->getY());
            }*/

        }
        else
        {
            printf( "no point geometry\n" );
        }
        OGRFeature::DestroyFeature( poFeature );
        break;
    }
    GDALClose( poDS );
}

