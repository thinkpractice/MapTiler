# Login with postgres user to do import
sudo -i -u postgres

# Import CBS buurt, wijk, and gemeente, provincial data
ogr2ogr -f PostgreSQL PG:dbname=addresses -progress -nlt PROMOTE_TO_MULTI /home/tjadejong/Documents/CBS/BAG/buurt2017/buurt_2017.shp

ogr2ogr -f PostgreSQL PG:dbname=addresses -progress -nlt PROMOTE_TO_MULTI /home/tjadejong/Documents/CBS/BAG/buurt2017/gem_2017.shp

ogr2ogr -f PostgreSQL PG:dbname=addresses -progress -nlt PROMOTE_TO_MULTI /home/tjadejong/Documents/CBS/BAG/buurt2017/wijk_2017.shp

ogr2ogr -f PostgreSQL PG:dbname=addresses -progress -nlt PROMOTE_TO_MULTI /home/tjadejong/Documents/CBS/BAG/cbs_provincies_2012.shp

# Import NRW addresses with SolarPanels
ogr2ogr -f PostgreSQL PG:dbname=addresses -progress -nlt PROMOTE_TO_MULTI /home/tjadejong/Documents/CBS/ZonnePanelen/PV_Anlagen/PV_201712_scharf.shp

# Go back to normal user
exit
