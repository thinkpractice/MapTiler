﻿create database deepsolaris;

-- Enable PostGIS (includes raster)
create extension postgis;
-- Enable Topology
create extension postgis_topology;
-- fuzzy matching needed for Tiger
create extension fuzzystrmatch;
-- rule based standardizer
create extension address_standardizer;

create table solarpanel_addresses_orig
(
  panel_id serial primary key,
  postcode character varying(6),
  "number" character varying(10),
  number_add character varying(8),
  building_id character varying(20),
  year_in_use integer,
  date_in_use date
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

alter table addresses_bag add column location_wgs84 geometry(Point, 4326);
alter table addresses_bag add column location_rd geometry(Point, 28992);

update addresses_bag set location_wgs84 = st_SetSrid(st_MakePoint(lon, lat), 4326);
update addresses_bag set location_rd = st_SetSrid(st_MakePoint(x, y), 28992);

CREATE INDEX addresses_bag_gix ON addresses_bag USING GIST ( location_wgs84 );
CREATE INDEX addresses_bag_gix ON addresses_bag USING GIST ( location_rd );

create table AreaOfInterest
(
	area_id serial primary key,
	description varchar(255),
	area geometry(POLYGON, 28992)	
);

CREATE INDEX area_of_interest_gix ON AreaOfInterest USING GIST ( area );

create table Tiles
(
	tile_id serial primary key,
	uuid varchar(37),
	area_id int references AreaOfInterest(area_id),
	area geometry(POLYGON, 28992)
);

CREATE INDEX tiles_gix ON Tiles USING GIST ( area );

create table pv_2017_nl
(
	  pv_id serial primary key,
	  postcode character varying(6),
	  "number" character varying(10),
	  number_add character varying(8),
	  building_id character varying(20),
	  year_in_use integer,
	  date_in_use date,
	  location geometry(POINT, 28992),
	  bag_address_id int references addresses_bag (address_id),
	  solar_panel_id int references solarpanel_addresses_orig(panel_id)
);

CREATE INDEX pv_2017_nl_gix ON pv_2017_nl USING GIST ( location );

insert into pv_2017_nl (postcode, number, number_add, building_id, year_in_use, date_in_use, location, bag_address_id, solar_panel_id)
select ab.postcode, ab.huisnummer, ab.huisnummertoevoeging, ab.adresseerbaarobject, sao.year_in_use, 
	sao.date_in_use, ST_Force2D(ab.geopunt), ab.gid, sao.panel_id 
from solarpanel_addresses_orig as sao
inner join bagactueel.adres_full as ab 
on sao.building_id = ab.adresseerbaarobject and sao.postcode = ab.postcode and CAST(sao.number as numeric) = ab.huisnummer;

create table tile_pv
(
	tile_id int references tiles(tile_id),
	metadata_id int references pv_2017_nl(pv_id),
	constraint tile_pv_pk primary key (tile_id, metadata_id)
);

create table tile_files
(
	file_id serial primary key,
	tile_id int references tiles(tile_id),
	filename varchar(255),
	layerName varchar(255),
	year int
);

create table tile_buildings
(
	tile_id int references tiles(tile_id),
	metadata_id int references bagactueel.pand(gid),
	constraint tile_building_pk primary key (tile_id, metadata_id)
);

create table tile_addresses
(
	tile_id int references tiles(tile_id),
	metadata_id int references bagactueel.adres_full(gid),
	constraint tile_addresses_pk primary key (tile_id, metadata_id)
);
