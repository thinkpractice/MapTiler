MapTiler
========

MapTiler is a program that helps downloading and preprocessing tiles from a geoservice or geoimage format as for example WMS, WMTS, or GeoTiff. To process the tiles, MapTiler implements a processing pipeline, in which tiles are first downloaded, then preprocessed on the GPU, before written to disk. The idea is that this pipeline will be configurable in the future. MapTiler is implemented in c++ and uses libgdal to provide support for a wide variety of geodata formats. At the time of writing, MapTiler only writes png or GeoTiff files as output. The latter format is the default as it also provides means to save a gps bounding box in the image metadata.


## Dependencies

MapTiler has a number of dependencies that have to be installed before attempting to build the software.
* CMake (>= 3.5),
* libgdal (>= 2.2),
* Qt5 (>= 5.10), in specific the QLocation and QCoreApplication classes,
* libpthread
* libpng
* libuuid
* libGL, libGLEW, libGLU and libglfw

In addition to that, MapTiler is implemented using the C++14 standard. To be compiled, it needs a compiler that support this standard. Any modern gcc or llvm/clang compiler should be sufficient.It has been successfully compiled using GCC 5.0 and GCC 7.0.

## Compilation

MapTiler can be compiled using CMake. In the location that you would like to save and build the code run the following in the terminal:

'
git clone https://github.com/thinkpractice/MapTiler.git
cd MapTiler
cmake .
make
'

## Running MapTiler


