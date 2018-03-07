cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 14)

project(MapViewer)

include_directories(/usr/include/gdal)
file(GLOB SRC_FILES *.cpp)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets)

add_executable(MapViewer ${SRC_FILES})
target_link_libraries(MapViewer libgdal.so libGL.so libGLEW.so libglfw.so Qt5::Widgets)

