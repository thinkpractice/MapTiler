cmake_minimum_required(VERSION 3.5)

file(GLOB SRC_FILES *.cpp)

add_library(Lib ${SRC_FILES})
target_link_libraries(Lib pthread.so libgdal.so libpng.so Qt5::Widgets Qt5::Location)

