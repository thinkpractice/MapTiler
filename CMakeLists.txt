cmake_minimum_required(VERSION 3.5)

project(MapViewer)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_PREFIX_PATH $ENV{HOME}/Qt/5.10.1/gcc_64)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_AUTOMOC ON)

message(STATUS ${CMAKE_PREFIX_PATH})

find_package(Qt5 COMPONENTS Widgets Location REQUIRED)

include_directories(/usr/include/gdal ${Qt5Widgets_INCLUDE_DIRS} ${Qt5Location_INCLUDE_DIRS})

add_subdirectory(Lib)

add_executable(MapViewer MapViewer.cpp)
target_link_libraries(MapViewer Lib)

add_executable(ShapeFileExample ShapeFileExample.cpp)
target_link_libraries(ShapeFileExample Lib)
