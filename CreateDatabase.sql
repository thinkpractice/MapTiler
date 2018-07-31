﻿create table solarpanel_addresses_orig
(
  panel_id serial primary key,
  postcode character varying(6),
  "number" character varying(10),
  number_add character varying(8),
  building_id character varying(20),
  year_in_use integer,
  date_in_use date,
);

create table addresses_bag
(
	address_id serial primary key,
	openbareruimte varchar(255),
	huisnummer varchar(10),
	huisletter varchar(1),
	huisnummertoevoeging varchar(10),
	postcode varchar(6),
	woonplaats varchar(255),
	gemeente varchar(255),
	provincie varchar(255),
	nummeraanduiding varchar(255),
	verblijfsobjectgebruiksdoel varchar(255),
	oppervlakteverblijfsobject varchar(255),
	verblijfsobjectstatus varchar(255),
	object_id varchar(255),
	object_type varchar(255),
	nevenadres varchar(255),
	pandid varchar(255),
	pandstatus varchar(255),
	pandbouwjaar varchar(255),
	x double precision,
	y double precision,
	lon double precision,
	lat double precision
);

copy addresses_bag(openbareruimte,huisnummer,huisletter,huisnummertoevoeging,
postcode,woonplaats,gemeente,provincie,nummeraanduiding,verblijfsobjectgebruiksdoel,
oppervlakteverblijfsobject,verblijfsobjectstatus,object_id,object_type,nevenadres,
pandid,pandstatus,pandbouwjaar,x,y,lon,lat) from '/home/tjadejong/Documents/CBS/BAG/bagadres-full.csv' delimiter ';' csv header;

alter table addresses_bag add column geom geometry(Point, 4326);

update addresses_bag set geom=st_SetSrid(st_MakePoint(lon, lat), 4326);

CREATE INDEX addresses_bag_gix ON addresses_bag USING GIST ( geom );

create table AreaOfInterest
(
	area_id serial primary key,
	description varchar(255),
	area GEOGRAPHY(POLYGON,4326)
);

CREATE INDEX area_of_interest_gix ON AreaOfInterest USING GIST ( area );

drop table Tiles cascade;
create table Tiles
(
	tile_id serial primary key,
	uuid varchar(37),
	area_id int references AreaOfInterest(area_id),
	area GEOGRAPHY(POLYGON,4326)
);

CREATE INDEX tiles_gix ON Tiles USING GIST ( area );


ALTER TABLE IF EXISTS pv_201712_scharf
RENAME TO pv_2017_de;

ALTER TABLE IF EXISTS solarpanel_addresses
RENAME TO solarpanel_addresses_orig;


CREATE INDEX pv_2017_de_gix ON pv_2017_de USING GIST ( wkb_geometry );

drop table pv_2017_nl;
create table pv_2017_nl
(
	  pv_id serial primary key,
	  postcode character varying(6),
	  "number" character varying(10),
	  number_add character varying(8),
	  building_id character varying(20),
	  year_in_use integer,
	  date_in_use date,
	  location GEOGRAPHY(POINT,4326),
	  bag_address_id int references addresses_bag (address_id),
	  solar_panel_id int references solarpanel_addresses_orig(panel_id)
);

CREATE INDEX pv_2017_nl_gix ON pv_2017_nl USING GIST ( location );

insert into pv_2017_nl (postcode, number, number_add, building_id, year_in_use, date_in_use, location, bag_address_id, solar_panel_id)
select ab.postcode, ab.huisnummer, ab.huisnummertoevoeging, ab.object_id, sao.year_in_use, sao.date_in_use, ab.geom, ab.address_id, sao.panel_id from solarpanel_addresses_orig as sao
inner join addresses_bag as ab 
on sao.building_id = ab.object_id and sao.postcode = ab.postcode and sao.number = ab.huisnummer

create table tile_files
(
	file_id serial primary key,
	tile_id int references tiles(tile_id),
	filename varchar(255),
	layerName varchar(255),
	year int
);

-- number of panels in solarpanel_addresses_orig
select count(*) from solarpanel_addresses_orig
select count(*) from pv_2017_nl

-- multiple addresses per building
select count(*) from addresses_bag
group by object_id
order by count(object_id) desc

-- some of the addresses have a different housenumber/postcode than the object id returned from the bag?
select count(*) from solarpanel_addresses_orig as sao
inner join addresses_bag as ab 
on sao.building_id = ab.object_id
where sao.building_id not in
select * from solarpanel_addresses_orig as sao
inner join addresses_bag as ab 
on sao.building_id = ab.object_id and sao.postcode = ab.postcode and sao.number = ab.huisnummer


select count(building_id) from solarpanel_addresses_orig as sao
inner join addresses_bag as ab 
on sao.building_id = ab.object_id and sao.postcode = ab.postcode and sao.number = ab.huisnummer

select * from address_bag as ab
inner join cbs_provincies_2012 as cp
on ST_Intersects(cp.wkb_geometry, ab.geom)
where cp.objectid = 1 and cp.provincien = 'Limburg';

insert into areaofinterest(description, area)
values ('Heerlen', ST_GeomFromText('POLYGON((50.8184032 5.9163049, 50.9342223 5.9163049, 50.9342223 6.0263794, 50.8184032 6.0263794, 50.8184032 5.9163049))', 4326));