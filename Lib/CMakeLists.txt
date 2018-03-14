cmake_minimum_required(VERSION 3.5)

project(MapViewer)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_PREFIX_PATH $ENV{HOME}/Qt/5.10.1/gcc_64)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Widgets Location REQUIRED)


include_directories(/usr/include/gdal ${Qt5Widgets_INCLUDE_DIRS} ${Qt5Location_INCLUDE_DIRS})

file(GLOB SRC_FILES *.cpp)

add_executable(MapViewer ${SRC_FILES})
target_link_libraries(MapViewer libgdal.so libGL.so libGLEW.so libglfw.so libpng.so Qt5::Widgets Qt5::Location)

