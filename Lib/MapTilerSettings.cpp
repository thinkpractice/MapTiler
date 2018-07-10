#include "MapTilerSettings.h"

MapTilerSettings::MapTilerSettings()
                    :   //rasterFilename(u8"WMTS:https://geodata.nationaalgeoregister.nl/luchtfoto/rgb/wmts/1.0.0/WMTSCapabilities.xml"),
                        mainRasterFilename(u8"WMTS:https://geodata.nationaalgeoregister.nl/luchtfoto/rgb/wmts/1.0.0/WMTSCapabilities.xml,layer=2016_ortho25,tilematrixset=EPSG:3857"),
                        mapFilenames({
                            {"ir_2016", u8"WMTS:https://geodata.nationaalgeoregister.nl/luchtfoto/infrarood/wmts/1.0.0/WMTSCapabilities.xml,layer=2016_ortho25IR,tilematrixset=EPSG:3857"},
                            {"rgb_2017", u8"WMTS:https://geodata.nationaalgeoregister.nl/luchtfoto/rgb/wmts/1.0.0/WMTSCapabilities.xml,layer=2017_ortho25,tilematrixset=EPSG:3857"},
                            //{"ir_2017", u8"WMTS:https://geodata.nationaalgeoregister.nl/luchtfoto/infrarood/wmts/1.0.0/WMTSCapabilities.xml,layer=2017_ortho25IR,tilematrixset=EPSG:3857"}
                        }),
                        /*metadataFilenames({
                            {"polygons", {u8"WFS:https://geodata.nationaalgeoregister.nl/bag/wfs?SERVICE=wfs", 1}},
                            {"addresses", {u8"WFS:https://geodata.nationaalgeoregister.nl/inspireadressen/wfs?SERVICE=wfs", 0}},
                        }),*/
                        tileWidth(1024),
                        tileHeight(1024)
{
}
