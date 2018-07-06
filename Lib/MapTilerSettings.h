#ifndef MAPTILERSETTINGS_H
#define MAPTILERSETTINGS_H

#include <string>
#include <map>
#include "Area.h"

using namespace std;

struct MetadataSetting
{
    string metadataFilename;
    int layerIndex;
};

struct MapTilerSettings
{
    MapTilerSettings();

    string mainRasterFilename;
    map<string, string> mapFilenames;
    map<string, MetadataSetting> metadataFilenames;
    string targetDirectory;
    string address;
    string displayUrl;
    int addressOption;
    int tileWidth;
    int tileHeight;
};

#endif // MAPTILERSETTINGS_H
