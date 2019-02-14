cmake_minimum_required(VERSION 3.5)

file(GLOB SRC_FILES *.cpp)

find_package(glfw3 3.2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GDAL 2.2.3 REQUIRED)


add_library(LibMapTiler ${SRC_FILES})
target_include_directories(LibMapTiler  PUBLIC ${OPENGL_INCLUDE_DIR} ${GDAL_INCLUDE_DIR})

set(LIBRARIES png uuid GLEW ${GDAL_LIBRARY} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} glfw Qt5::Core Qt5::Widgets Qt5::Location)

if (UNIX AND NOT APPLE)
	set(LIBRARIES ${LIBRARIES} pthread.so)
endif()
target_link_libraries(LibMapTiler ${LIBRARIES})
