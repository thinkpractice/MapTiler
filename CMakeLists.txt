cmake_minimum_required(VERSION 3.5)

project(MapTiler)

set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_PREFIX_PATH $ENV{HOME}/Qt/5.10.1/gcc_64)
set(CMAKE_PREFIX_PATH $ENV{HOME}/Qt/5.10.1/clang_64)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_AUTOMOC ON)

find_package(GDAL 2.2.3 REQUIRED)
find_package(Qt5 COMPONENTS Core Widgets Location REQUIRED)

include_directories(${GDAL_INCLUDE_DIR} ${Qt5Widgets_INCLUDE_DIRS} ${Qt5Location_INCLUDE_DIRS})

add_subdirectory(Lib)

add_executable(MapTiler MapTiler.cpp)
target_link_libraries(MapTiler Lib Qt5::Core)

add_executable(ShapeFileExample ShapeFileExample.cpp)
target_link_libraries(ShapeFileExample Lib)

add_executable(GLTest GLTest.cpp)
target_link_libraries(GLTest Lib)

add_executable(DatabaseExample databaseexample.cpp)
target_link_libraries(DatabaseExample Lib)
