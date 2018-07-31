cmake_minimum_required(VERSION 3.5)

file(GLOB SRC_FILES *.cpp)

find_package(glfw3 3.2 REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(/usr/include/gdal)

add_library(Lib ${SRC_FILES})
target_include_directories(Lib PUBLIC ${OPENGL_INCLUDE_DIR})
target_link_libraries(Lib pthread.so libgdal.so libpng.so libuuid.so libGLEW.so ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} glfw Qt5::Core Qt5::Widgets Qt5::Location)
