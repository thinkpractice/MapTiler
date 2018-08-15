select * into filled_tiles
from tiles
where tile_id in
(select distinct(ti.tile_id) from areaofinterest as ai
inner join tiles as ti on ti.area_id = ai.area_id and ai.area_id = 74
inner join tile_buildings as tb on tb.tile_id = ti.tile_id)