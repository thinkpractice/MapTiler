cmake_minimum_required(VERSION 3.5)

file(GLOB SRC_FILES *.cpp)

include_directories(/usr/include/gdal)

add_library(Lib ${SRC_FILES})
target_link_libraries(Lib pthread.so libgdal.so libpng.so libuuid.so libGL.so libGLEW.so libglfw.so libGLU.so Qt5::Core Qt5::Widgets Qt5::Location)
