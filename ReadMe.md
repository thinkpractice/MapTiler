MapTiler
========

MapTiler is a program that helps downloading and preprocessing tiles from a geoservice or geoimage format, as for example WMS, WMTS, or GeoTiff. To process the tiles, MapTiler implements a processing pipeline in which tiles are first downloaded, then preprocessed on the GPU, before written to disk. The idea is that this pipeline will be configurable in the future. MapTiler is implemented in c++ and uses libgdal to provide support for a wide variety of geodata formats. At the time of writing, MapTiler only writes Png or GeoTiff files as output. The latter format is the default as it also provides means to save a GPS bounding box in the image metadata.


## Dependencies

MapTiler has a number of dependencies that have to be installed before attempting to build the software.
* **CMake** (>= 3.5),
* **libgdal** (>= 2.2),
* **Qt5** (>= 5.10), in specific the QLocation and QCoreApplication classes,
* **libpthread**
* **libpng**
* **libuuid**
* **libGL**, **libGLEW**, **libGLU** and **libglfw**

In addition to that, MapTiler is implemented using the C++14 standard. To be compiled, it needs a compiler that support this standard. Any modern gcc or llvm/clang compiler should be sufficient. It has been successfully compiled using GCC 5.0 and GCC 7.0.

## Compilation

MapTiler can be compiled using CMake. In the location that you would like to save and build the code run the following in the terminal:

~~~~
git clone https://github.com/thinkpractice/MapTiler.git
cd MapTiler
cmake .
make
~~~~

## Running MapTiler

To run MapTiler, several options have to specified first. The general syntax to run MapTiler looks like this:

~~~~
./MapTiler [options] rasterurl polygonurl
~~~~

###Options

The following options can be provided to the application:
* -d, --defaulturls,  uses default urls for rasterurl and vectorurl
* -h, --help, displays a help message
* -v, --version, displays version information.
* --address <location>, the <location> (address/city name/region) for which the tiles should be downloaded.
* -a, --addressoption <locationoption>  The location option to choose if the address gives back multiple option (default=first)
* -t, --target-directory <directory>    Copy all the tiles into <directory>.
* -c, --tilewidth <width>               The <width> (number of columns) of the tiles to be written to disk
* -r, --tileheight <height>   The <height> (number of rows) of the tiles to be written to disk

###Arguments

Unless the -d, --defaulturls option (see above) has been specified MapTiler also needs the following arguments:
* rasterurl, url to raster webservice (WMS/WMTS) with the aerial image.
* vectorurl, url to the vector webservice (WFS) with the polygons.

