cmake_minimum_required (VERSION 3.8)

project(stb)


set(STB_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/stb)
include_directories( ${STB_SOURCE_DIR})


set(STB_LIBTYPE STATIC)

set(API 
	${STB_SOURCE_DIR}/stb_image_write.h ${STB_SOURCE_DIR}/stb_truetype.h)

set(SOURCES
	${STB_SOURCE_DIR}/stb_image_write.cpp ${STB_SOURCE_DIR}/stb_truetype.cpp)

add_library(stb ${STB_LIBTYPE} ${SOURCES})

set_target_properties(stb PROPERTIES PUBLIC_HEADER "${API}" LINKER_LANGUAGE CXX)