# Create Data dir to download all the data
if [ ! -d Data ]; then
    mkdir Data
fi

cd Data
# Download CBS Mapping Material
datadir=$(pwd)
echo $datadir

if [ ! -f buurt_2017.zip ]; then
    wget "https://www.cbs.nl/-/media/_pdf/2017/36/buurt_2017.zip"
    unzip buurt_2017.zip -d buurt2017
fi

if [ ! -f cbs_provincies_2012.shp ]; then
    ogr2ogr -f 'ESRI Shapefile' cbs_provincies_2012.shp WFS:https://geodata.nationaalgeoregister.nl/cbsprovincies/wfs?request=GetCapabilities
fi

# Download bag data
if [ ! -f  bag-adressen-full-laatst.csv.zip ]; then
    wget "https://data.nlextract.nl/bag/csv/bag-adressen-full-laatst.csv.zip"
    unzip bag-adressen-full-laatst.csv.zip -d BAG
fi

sudo chmod -R 774 Data
# Login with postgres user to do import
#sudo -i -u postgres

# Import CBS buurt, wijk, and gemeente, provincial data
sudo -u postgres ogr2ogr -f PostgreSQL PG:dbname=deepsolaris -progress -nlt PROMOTE_TO_MULTI $datadir/buurt2017/buurt_2017.shp

sudo -u postgres ogr2ogr -f PostgreSQL PG:dbname=deepsolaris -progress -nlt PROMOTE_TO_MULTI $datadir/buurt2017/gem_2017.shp

sudo -u postgres ogr2ogr -f PostgreSQL PG:dbname=deepsolaris -progress -nlt PROMOTE_TO_MULTI $datadir/buurt2017/wijk_2017.shp

sudo -u postgres ogr2ogr -f PostgreSQL PG:dbname=deepsolaris -progress -nlt PROMOTE_TO_MULTI $datadir/cbs_provincies_2012.shp

# Import NRW deepsolaris with SolarPanels
#ogr2ogr -f PostgreSQL PG:dbname=deepsolaris -progress -nlt PROMOTE_TO_MULTI $datadir/PV_201712_scharf.shp
sudo -u postgres ogr2ogr -f PostgreSQL PG:dbname=deepsolaris -progress -nlt PROMOTE_TO_MULTI "WFS:https://geodata.nationaalgeoregister.nl/bag/wfs?request=GetCapabilities"
# Go back to normal user
exit
