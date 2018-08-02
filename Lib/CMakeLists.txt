cmake_minimum_required(VERSION 3.5)

file(GLOB SRC_FILES *.cpp)

find_package(glfw3 3.2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GDAL 2.2.3 REQUIRED)

#include_directories(/usr/include/gdal)

add_library(Lib ${SRC_FILES})
target_include_directories(Lib PUBLIC ${OPENGL_INCLUDE_DIR} ${GDAL_INCLUDE_DIR})
target_link_libraries(Lib pthread.so libpng.so libuuid.so libGLEW.so ${GDAL_LIBRARY} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} glfw Qt5::Core Qt5::Widgets Qt5::Location)
