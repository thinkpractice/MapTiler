cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 14)

project(MapViewer)

include_directories(/usr/include/gdal)
file(GLOB SRC_FILES *.cpp)

add_executable(MapViewer ${SRC_FILES})
target_link_libraries(MapViewer libgdal.so libGL.so libGLEW.so libglfw.so)
